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

		fprintf(stderr, "Empty: Root: %d Height: %d\n", rootPid, treeHeight);
		pf.write(0, buffer);
	}
	// Read contents of file
	else
	{
		char buffer[PageFile::PAGE_SIZE];
		pf.read(0, buffer);
		int *intBuffer = (int*) buffer;
		rootPid = intBuffer[0];
		treeHeight = intBuffer[1];

		fprintf(stderr, "Existng: Root: %d Height: %d\n", rootPid, treeHeight);
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

	pf.write(0, buffer); 

    return pf.close();
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
    return 0;
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
	BTNonLeafNode* nonleaf = new BTNonLeafNode(); 
	BTLeafNode* leaf = new BTLeafNode(); 
	PageId pid; 
	int pos; 


	// search through nonleaf nodes for currect pid
	for (int i = 1; i < treeHeight; i++)
	{
		nonleaf.locateChildPtr(searchKey, pid, pos);
	}

	cursor.pid = pid; 

	// not sure if right: GO TO LEAF POINTED TO BY PID AND DO LOCATE
	PageFile pf = PageFile(); 
	leaf.read(cursor.pid, pf);
	rc = leaf.locate(key, cursor.eid);

	delete leaf; 
	delete nonleaf; 

	if (rc != 0)
		return RC_NO_SUCH_RECORD; 

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
	//BTLeafNode* leaf; 
   PageFile pf = PageFile(); 
   
   leaf -> read(cursor.pid, pf);
   RC rc = leaf -> readEntry(cursor.eid, key, rid);
   if (rc != 0)
       return rc;
       
   // last entry in node
   if (cursor.eid == (leaf -> getKeyCount() - 1))
   {
       cursor.pid = leaf -> getNextNodePtr();
       cursor.eid = 0;
   }
   else
       cursor.eid++;
   delete leaf;
   return 0;
}
