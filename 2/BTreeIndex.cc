/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
    treeHeight = 0;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
	RC rc = pf.open(indexname, mode);

	// Error opening file
	if (rc != 0)
		return rc;

	// Empty file
	if (pf.endPid() == 0)
	{
		rootPid = -1;
		treeHeight = 0;
		char buffer[PageFile::PAGE_SIZE];
		int *intBuffer = (int*) buffer;
		intBuffer[0] = rootPid;
		intBuffer[1] = treeHeight;

		//fprintf(stderr, "Empty: EndPid: %d RootPid: %d Height: %d\n", pf.endPid(), intBuffer[0], intBuffer[1]);
		fprintf(stderr, "Empty: RootPid: %d Height: %d\n", intBuffer[0], intBuffer[1]);
		pf.write(0, buffer);

		//fprintf(stderr, "EndPid: %d\n", pf.endPid());
	}
	// Read contents of file
	else
	{
		char buffer[PageFile::PAGE_SIZE];
		pf.read(0, buffer);
		int *intBuffer = (int*) buffer;
		rootPid = intBuffer[0];
		treeHeight = intBuffer[1];

		fprintf(stderr, "Existing: RootPid: %d Height: %d\n", rootPid, treeHeight);
	}

    return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
	char buffer[PageFile::PAGE_SIZE];
	int *intBuffer = (int*) buffer;
	intBuffer[0] = rootPid;
	intBuffer[1] = treeHeight;

	//fprintf(stderr, "What is this: %d %d %d %d %d\n", intBuffer[0], intBuffer[1], intBuffer[2], intBuffer[3], intBuffer[4]);

	pf.write(0, buffer); 

    return pf.close();
}

// Creates a "root" node for empty tree
RC BTreeIndex::initTree(const int key, const RecordId& rid)
{
	//fprintf(stderr, "Inside initTree\n");
	BTLeafNode *root = new BTLeafNode();

	root->insert(key, rid);
	rootPid = pf.endPid();
	

	fprintf(stderr, "PreRoot Pid: %d\n", rid.pid);
	
	treeHeight = 1;
	root->write(rootPid, pf);

	fprintf(stderr, "RootPid: %d Pid: %d Sid: %d\n", rootPid, rid.pid, rid.sid);
	delete root;

	return 0;

}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
    int siblingKey;
    PageId siblingPid;

    // new index
    if (treeHeight == 0)
    {
    	//fprintf(stderr, "entering initTree at %d\n", treeHeight);
    	return initTree(key, rid);
    }

    if (insertHelper(key, rid, 1, rootPid, siblingKey, siblingPid) != 0)
    {
    	fprintf(stderr, "insertHelper had an error\n");
    	return RC_FILE_WRITE_FAILED;
    }

    // overflow at top level, create new root
    if (siblingKey > 0)
    {
    	fprintf(stderr, "Creating new root\n");
    	BTNonLeafNode *root = new BTNonLeafNode();

    	root->initializeRoot(rootPid, siblingKey, siblingPid);
    	rootPid = pf.endPid();
    	treeHeight++;
    	root->write(rootPid, pf);

    	delete root;
    }

    //fprintf(stderr, "TreeHeight: %d", treeHeight);
    return 0;
}

/*
	Helper for insert
	level: level we are searching the B+ tree on
	pid: PageId of the node we are currently on
	siblingKey: key to insert if there is overflow, otherwise -1
	siblingPid: PageId to insert if there is overflow, otherwise -1
*/
RC BTreeIndex::insertHelper(int key, const RecordId& rid, int level, PageId pid, int& siblingKey, PageId& siblingPid)
{
	siblingKey = -1;

	// Base Case: at leaf node
	if (level == treeHeight)
	{
		BTLeafNode *leaf = new BTLeafNode();
		leaf->read(pid, pf);

		// Overflowed
		if (leaf->insert(key, rid) != 0)
		{
			fprintf(stderr, "Splitting leaves\n");
			BTLeafNode *siblingLeaf = new BTLeafNode();
			leaf->insertAndSplit(key, rid, *siblingLeaf, siblingKey);

			// Adjust PageId pointers
			siblingPid = pf.endPid();
			siblingLeaf->setNextNodePtr(leaf->getNextNodePtr());
			leaf->setNextNodePtr(siblingPid);

			if (siblingLeaf->write(siblingPid, pf) != 0)
			{
				fprintf(stderr, "Sibling could not be written!\n");

				delete leaf;
				delete siblingLeaf;

				return RC_FILE_WRITE_FAILED;
			}

			delete siblingLeaf;
		}

		if(leaf->write(pid, pf) != 0)
		{
			fprintf(stderr, "Leaf could not be written!\n");

			delete leaf;

			return RC_FILE_WRITE_FAILED;
		}

		delete leaf;

		return 0;
	}
	// At nonleaf node
	else
	{
		BTNonLeafNode *nonLeaf = new BTNonLeafNode();
		PageId childPid;
		int pos;

		nonLeaf->read(pid, pf);
		nonLeaf->locateChildPtr(key, childPid, pos);
		insertHelper(key, rid, level+1, childPid, siblingKey, siblingPid);

		// child node overflowed
		if (siblingKey > 0)
		{
			// non-leaf overflowed
			if (nonLeaf->insert(siblingKey, siblingPid) != 0)
			{
				fprintf(stderr, "Splitting non-leaves\n");
				int midKey;
				BTNonLeafNode *siblingNonLeaf = new BTNonLeafNode();

				nonLeaf->insertAndSplit(siblingKey, siblingPid, *siblingNonLeaf, midKey);
				siblingKey = midKey;
				siblingPid = pf.endPid();

				if (siblingNonLeaf->write(siblingPid, pf) != 0)
				{
					fprintf(stderr, "Sibling could not be written!\n");

					delete nonLeaf;
					delete siblingNonLeaf;

					return RC_FILE_WRITE_FAILED;
				}

				delete siblingNonLeaf;
			}
			// there was no overflow in non-leaf
			else {
				fprintf(stderr, "No Overflow in non-leaf\n");
				siblingKey = -1;
			}
			
			nonLeaf->write(pid, pf);

			delete nonLeaf;

			return 0;

		}
	}
}

/*
 * Find the leaf-node index entry whose key value is larger than or 
 * equal to searchKey, and output the location of the entry in IndexCursor.
 * IndexCursor is a "pointer" to a B+tree leaf-node entry consisting of
 * the PageId of the node and the SlotID of the index entry.
 * Note that, for range queries, we need to scan the B+tree leaf nodes.
 * For example, if the query is "key > 1000", we should scan the leaf
 * nodes starting with the key value 1000. For this reason,
 * it is better to return the location of the leaf node entry 
 * for a given searchKey, instead of returning the RecordId
 * associated with the searchKey directly.
 * Once the location of the index entry is identified and returned 
 * from this function, you should call readForward() to retrieve the
 * actual (key, rid) pair from the index.
 * @param key[IN] the key to find.
 * @param cursor[OUT] the cursor pointing to the first index entry
 *                    with the key value.
 * @return error code. 0 if no error.
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
	BTNonLeafNode* nonLeaf = new BTNonLeafNode(); 
	BTLeafNode* leaf = new BTLeafNode(); 
	PageId pid = rootPid; 
	int pos; 

	fprintf(stderr, "Treeheight: %d\n", treeHeight);

	// search through nonleaf nodes for correct pid
	for (int i = 1; i < treeHeight; i++)
	{
		fprintf(stderr, "Treelevel: %d ", i);
		nonLeaf->read(pid, pf);
		fprintf(stderr, "StartingPid: %d ", pid);
		nonLeaf->locateChildPtr(searchKey, pid, pos);
		fprintf(stderr, "ChildPid: %d\n", pid);
		fprintf(stderr, "Pos in array?: %d\n", pos);
	}

	// pid now points to a leaf node
	cursor.pid = pid; 
	//fprintf(stderr, "PageId: %d\n", cursor.pid);

	// read the leaf
	leaf->read(cursor.pid, pf);

	RC rc = leaf->locate(searchKey, cursor.eid);

	if (rc != 0)
	{
		fprintf(stderr, "Not found in this leaf node\n");
		cursor.pid = leaf->getNextNodePtr();
		fprintf(stderr, "New Cursor.pid: %d\n", cursor.pid);

		if (pid == -1)
		{
			fprintf(stderr, "Key does not exist\n");
			delete leaf; 
			delete nonLeaf; 
			return RC_NO_SUCH_RECORD;
		}
		else
		{
			fprintf(stderr, "Moving to next\n");
			leaf->read(cursor.pid, pf);
			leaf->locate(searchKey, cursor.eid);
		}
	} 

	delete leaf; 
	delete nonLeaf; 

	fprintf(stderr, "EntryId: %d\n", cursor.eid);

    return 0;
}

/*
	Read the root specified at pid
	@param searchKey[IN] the specific key to search for in root
	@param pid[IN] the pid at which root is located at
	@param key[OUT] the key stored at root
	@param[OUT] the left child pointer of key
	@param[OUT] the right child pointer of key
*/
RC BTreeIndex::readRoot(int searchKey, PageId pid, int& key, PageId& left, PageId& right)
{
	BTNonLeafNode *root = new BTNonLeafNode();
	int pos;

	RC rc = root->read(pid, pf);
	if (rc != 0)
	{
		fprintf(stderr, "could not be read\n");
		return RC_FILE_READ_FAILED;
	}
	else
	{
		root->locateChildPtr(searchKey-1, left, pos);
		root->locateChildPtr(searchKey+1, right, pos);
	}
	return 0;

}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{

   BTLeafNode *leaf = new BTLeafNode(); 
   
   leaf -> read(cursor.pid, pf);
   RC rc = leaf -> readEntry(cursor.eid, key, rid);
   if (rc != 0)
   {
   	   fprintf(stderr, "Entry does not exist\n");
   	   delete leaf;
       return rc;
   }   
   fprintf(stderr, "Eid: %d Key: %d Rid: %d\n", cursor.eid, key, rid);
   // last entry in node
   if (cursor.eid == (leaf -> getKeyCount() - 1))
   {
   	   fprintf(stderr, "Moving to next sibling leaf\n");
       cursor.pid = leaf -> getNextNodePtr();
       cursor.eid = 0;
   }
   else
       cursor.eid++;
   //fprintf(stderr, "Updated Eid; %d\n", cursor.eid);
   delete leaf;
   return 0;
}
