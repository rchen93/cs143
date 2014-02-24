#include "BTreeNode.h"

using namespace std;

struct BTLeafNode::Entry
{
	int key;
	RecordId rid;
};


/*
LeafNode Buffer
0: KeyCount
1: key
2: pid
3: sid
4: key
5: pid
6: sid
......n: ptr ->
*/

BTLeafNode::BTLeafNode()
{
	// Zero out the buffer
	//memset(buffer, 0, PageFile::PAGE_SIZE);

	int *intBuffer = (int *)buffer;
    //intBuffer[0] = 0;
    intBuffer[255] = -1;
    
/************* REMOVE LATER *********/
	intBuffer[0] = 84;	// keycount
	intBuffer[1] = 0;
	intBuffer[2] = 2;
	intBuffer[3] = 3;

	intBuffer[4] = 4;
	intBuffer[5] = 5;
	intBuffer[6] = 6;

	for (int i = 7; i < 255; i++)
	{
		intBuffer[i] = i;
	}
}

/****** REMOVE LATER *********/
RC BTLeafNode::emptyNode()
{
	int *intBuffer = (int*) buffer;
	intBuffer[255] = -1;
	intBuffer[0] = 0;
	return 0;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{
	return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
	return pf.write(pid, buffer);
}

// Returns the maximum number of keys possible for a node
int BTLeafNode::getMaxCount() const
{
	//return sizeof(PageId);
	return (PageFile::PAGE_SIZE-sizeof(PageId)-sizeof(int))/(sizeof(Entry));
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{ 
	int *intBuffer = (int*) buffer;

  	return intBuffer[0]; 
	/*
	int numKeys = 0;

	// First four bytes in buffer holds number of keys
	memcpy(&numKeys, &buffer, sizeof(int));

	return numKeys;
	*/
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
	if (getKeyCount() >= getMaxCount())
		return RC_NODE_FULL;

	int *intBuffer = (int*) buffer;
	int eid;
	int readKey;
	RecordId read_rid;
	RC rc;

	rc = locate(key, eid);
	if (rc != 0)
	{
		eid = getKeyCount();
		intBuffer[3*eid+1] = key;
		intBuffer[3*eid+1+1] = rid.pid; 
		intBuffer[3*eid+1+2] = rid.sid; 
		updateKeyCount(true); 
		return 0; 
	}
	else
	{
		shift(eid); // shifts contents in array to allocate room for new entry
		intBuffer[3*eid+1] = key;
		intBuffer[3*eid+1+1] = rid.pid; 
		intBuffer[3*eid+1+2] = rid.sid; 
		updateKeyCount(true); 
		return 0; 
	}

}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
	int numMove = getKeyCount()/2;
	int numStay = getKeyCount() - numMove;
	int readKey;
	RecordId read_rid;
	PageId pid;

	RC rc;

	for (int i = getKeyCount() - 1; i >= numStay; i--)
	{
		// read entry from original node
		if (rc = readEntry(i, readKey, read_rid) != 0)
		{
			fprintf(stderr, "Here1\n");
			return rc;
		}
		// insert into sibling node
		if (rc = sibling.insert(readKey, read_rid) != 0)
		{
			fprintf(stderr, "Here2\n");
			return rc;
		}
		// delete entry from original buffer
		if (rc = deleteEntry(i) != 0)
		{
			fprintf(stderr, "Here3\n");
			return rc;
		}
	} 
	// chceck that sibling entry is valid
	if (rc = sibling.readEntry(0, siblingKey, read_rid) != 0)
		return rc;

	// insert into original node
	insert(key, rid);
	setNextNodePointer()

	return 0;
}

RC BTLeafNode::deleteEntry(const int eid)
{
	if (eid >= getKeyCount())
		return RC_NO_SUCH_RECORD;

	int* intBuffer = (int*) buffer;

	intBuffer[3*eid+1] = -1;
	intBuffer[3*eid+1+1] = -1; 
	intBuffer[3*eid+1+2] = -1;

	updateKeyCount(false);

	return 0; 

}

/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ 
	int *intBuffer = (int*) buffer;
	int key;
	RecordId rid;
	RC rc;

	for(int i = 0; i < getKeyCount(); i++)
	{
		if (rc = readEntry(i, key, rid) != 0)
			return rc;
		if (key >= searchKey)
		{
			eid = i;
			return 0;
		}
	}	
	
	return RC_NO_SUCH_RECORD; 
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{ 
	// Invalid entry
	if (eid < 0 || eid >= getKeyCount())
		return RC_INVALID_CURSOR;

	int *intBuffer = (int*) buffer;	

	key = intBuffer[3*eid+1];
	rid.pid = intBuffer[3*eid+1+1];
	rid.sid = intBuffer[3*eid+1+2];

	return 0; 
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ 
	int *intBuffer = (int*) buffer;
	return intBuffer[255];
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
	int *intBuffer = (int*) buffer;
	intBuffer[255] = pid;
	return 0;
}

void BTLeafNode::updateKeyCount(bool increment)
{

  int *intBuffer = (int*) buffer;
  if(increment)
    intBuffer[0]++;
  else
    intBuffer[0]--;
}

/* eid is where we want to insert new entry
	read every entry after eid and shift it */
RC BTLeafNode::shift(const int eid)
{
	if (eid < 0)
		return RC_INVALID_CURSOR;

	
	int *intBuffer = (int*) buffer;
	// manually shift each element in buffer, in blocks of 3, since each entry has 3 elements
	for (int i = 3*getKeyCount(); i >= 3*eid+1; i--) 
	{
		//fprintf(stderr, "%d ", intBuffer[i]);
		intBuffer[i+3] = intBuffer[i]; 	
		intBuffer[i] = -1;
		//fprintf(stderr, "%d ", intBuffer[i+3]);
	}
}

void BTLeafNode::printNode()
{
	int key;
	RecordId rid;

	int numkeys = getKeyCount();
	fprintf(stderr, "Keys: %d\n", numkeys);
	for (int i = 0; i < numkeys; i++)
	{
		readEntry(i, key, rid);
		fprintf(stderr, "Eid: %d Key: %d Pid: %d Sid: %d\n", i, key, rid.pid, rid.sid);
	}
}

struct BTNonLeafNode::Entry
{
	int key;
	PageId pid;
};

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 
	return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ 
	return pf.write(pid, buffer); 
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ 
	int *intBuffer = (int*) buffer;

  	return intBuffer[0]; 
	/*
	int numKeys = 0;

	// First four bytes in buffer holds number of keys
	memcpy(&numKeys, &buffer, sizeof(int));

	return numKeys;
	*/

}

/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ 
	if (getKeyCount() >= max_NonLeaf)
		return RC_NODE_FULL;

	int *intBuffer = (int*) buffer;
	

}

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{ 
	return 0; 
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ 
	return 0; 
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ 
	int *bufferPtr = (int *) buffer;

	*(bufferPtr+1) = pid1;
	*(bufferPtr+2) = key;
	*(bufferPtr+3) = pid2;
	updateKeyCount(true);

	/*
	// Zero out buffer
	memset(buffer, 0, PageFile::PAGE_SIZE);

	int i = 1;

	char* pos = &(buffer[0]);
	pos += sizeof(int);			// keycount
	memcpy(pos, &pid1, sizeof(PageId));
	pos += sizeof(PageId);
	memcpy(pos, &key, sizeof(int));
	pos += sizeof(int);
	memcpy(pos, &pid2, sizeof(PageId));

	memcpy(buffer, &i, sizeof(int));		// keycount = 1
	*/

	return 0;
}

void BTNonLeafNode::updateKeyCount(bool increment)
{

  int *intBuffer = (int*) buffer;
  if(increment)
    intBuffer[0]++;
  else
    intBuffer[0]--;
}