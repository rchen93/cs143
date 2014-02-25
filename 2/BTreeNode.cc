#include "BTreeNode.h"
#include <math.h>

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
	int *intBuffer = (int *)buffer;
    intBuffer[0] = 0;
    intBuffer[255] = -1;
}

BTLeafNode::BTLeafNode(int num, bool skip)
{
	int *intBuffer = (int*) buffer;
	intBuffer[0] = 0;
	intBuffer[255] = -1;

	if (skip == false)
	{
		for (int i = 0; i < num; i++)
		{
			intBuffer[3*i+1] = i;
			intBuffer[3*i+1+1] = i+1;
			intBuffer[3*i+1+2] = i+2;
			intBuffer[0]++;
		}
	}
	else
	{
		for (int i = 0; i < num; i++)
		{
			intBuffer[3*i+1] = i*2;
			intBuffer[3*i+1+1] = i*2+1;
			intBuffer[3*i+1+2] = i*2+2;
			intBuffer[0]++;
		}
	}
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
	// Empty Node or Last Entry insertion
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

 // TO FIX: INSERTION AFTER SPLIT??
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
	double numKeys = (double)(getKeyCount());
	int numMove = (int)(ceil(numKeys/2));
	int numStay = getKeyCount() - numMove;
	int readKey;
	RecordId read_rid;
	PageId pid;

	RC rc;

	//fprintf(stderr, "keys: %d\n", getKeyCount());
	for (int i = (getKeyCount() - 1); i >= numStay; i--)
	{
		//fprintf(stderr, "i: %d\n", i);
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

	sibling.readEntry(0, siblingKey, read_rid);
	readEntry((getKeyCount()-1), readKey, read_rid);

	// if key is larger than original node, put it new node
	if (readKey < key)
	{
		sibling.insert(key, rid);
		sibling.readEntry(0, siblingKey, read_rid);
	}
	// otherwise, it belongs in original node
	else
	{
		insert(key, rid);
		sibling.readEntry(0, siblingKey, read_rid);
	}

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

	if (getKeyCount() <= 0)
    	return RC_INVALID_CURSOR;

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

/**************** LeafNode Helper Functions ***************************************/

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

/***************************** NonLeafNode *******************************((((((*****/


BTNonLeafNode::BTNonLeafNode()
{
	int *intBuffer = (int*) buffer;
	intBuffer[0] = 0;
}


// Makes a LeafNode with num keys filled 
// skip == false: keys differ by 1
// skip == true: keys differ by 2
BTNonLeafNode::BTNonLeafNode(int num, bool skip)
{
	int *intBuffer = (int *) buffer;
	intBuffer[0] = 0;

	if (skip == false)
	{
		for (int i = 0; i < num; i++)
		{
			intBuffer[2*i+2] = i;
			intBuffer[2*i+2-1] = i+1;
			updateKeyCount(true);
		}
	}
	else
	{
		for (int i = 0; i < num; i++)
		{
			intBuffer[2*i+2] = 2*i;
			intBuffer[2*i+2-1] = i+1;
			updateKeyCount(true);
		}
	}
	intBuffer[2*num+1] = 2*num+1;
}


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

 // Returns the maximum number of keys possible for a node
 /*
    * Unspanned:
    * NonLeafNode: [ kc | pid | key | pid | ... | pid ]
    * 1024/4 = 256 int array
    * 256 - 1 - 1 last pid and keycount
    * 254/2= 127 pairs of [pid | key] entries
 */

int BTNonLeafNode::getMaxCount() const
{
	return (PageFile::PAGE_SIZE-sizeof(PageId)-2*sizeof(int))/(2*sizeof(int));
}


/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ 
	int *intBuffer = (int*) buffer;

  	return intBuffer[0]; 

}

/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ 
    if (getKeyCount() >= getMaxCount())
        return RC_NODE_FULL;

    int *intBuffer = (int*) buffer;
    PageId found_pid;
    int pos;
    RC rc; 
    
    rc = locateChildPtr(key, found_pid, pos);
    
    // Empty Node
    if (rc != 0)
    {
        intBuffer[1] = pid;
        intBuffer[2] = key; 
        updateKeyCount(true); 
        return 0; 
    }
    else
    {

        shift(pos); // shifts contents in array to allocate room for new entry
        // fprintf(stderr, "Pos: %d\n", pos);
        intBuffer[pos] = pid;
        intBuffer[pos+1] = key;
        updateKeyCount(true); 
        return 0; 
    }

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
	// +1 for the key we're going to insert
	// maybe change getKeyCount() to maxCount() later because
	// split is only called on maxnode?

	double numKeys = (double)(getKeyCount());
	int numMove = (int)(ceil(numKeys/2));
	int numStay = getKeyCount() - numMove;

	int *intBuffer = (int*) buffer;
	int first_pid = intBuffer[1];
	int first_key = intBuffer[2];
	bool flag = false;				// flag if we have inserted our key

	// Put (key,pid) at front of buffer
	intBuffer[1] = pid;
	intBuffer[2] = key;

	PageId read_pid;
	int read_key;
	RC rc;

	// Insertion Sort
	// WHAT DO I DO WITH LAST PID WhEN I INSERT At END? use flag for now
	for (int i = 0; i < getKeyCount()-1; i++)
	{
		readEntry(i+1, read_pid, read_key);

		// if key we want to insert is larger than the adjacent key, swap them
		if (read_pid < key)
		{
			intBuffer[2*(i+1)+1] = pid;
			intBuffer[2*(i+1)+2] = key;
			intBuffer[2*i+1] = read_pid;
			intBuffer[2*i+2] = read_key;
		}
		else
		{
			flag = true;
			break;
		}
	}

	// Split
	if (flag == true)
	{
		PageId last_pid = intBuffer[2*getKeyCount()+1];
		for (int i = (getKeyCount() - 1); i >= numStay; i--)
		{
			// read entry from original node
			if (rc = readEntry(i, read_pid, read_key) != 0)
			{
				fprintf(stderr, "Here1\n");
				return rc;
			}
			// insert into sibling node
			if (rc = sibling.insert(read_key, read_pid) != 0)
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

		// insert first key back into original buffer
		insert(first_key, first_pid);

		sibling.insertLastPid(last_pid);

		sibling.readEntry(0, read_pid, read_key);
		midKey = read_key;

		return 0;

	}
	// key we want to insert belongs at end
	else
	{

	}

    return 0; 
}

RC BTNonLeafNode::insertLastPid(PageId& pid)
{
	int *intBuffer = (int*) buffer;
	intBuffer[2*getKeyCount()+1] = pid;
	
	return 0;
}


RC BTNonLeafNode::readEntry(const int eid, PageId& pid, int& key)
{
	if (eid < 0 || eid >= getKeyCount())
		return RC_INVALID_CURSOR;

	int *intBuffer = (int*) buffer;

	pid = intBuffer[2*eid+1];
	key = intBuffer[2*eid+2];

	return 0;

}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid, int& pos)
{ 
    int *intBuffer = (int*) buffer;
    int keyCount = getKeyCount();

    if (keyCount <= 0)
        return RC_INVALID_CURSOR;

    for (int i = 0; i < getKeyCount(); i++)
    {
        if (intBuffer[2*i+2] >= searchKey)
        {
            pid = intBuffer[2*i+1];
            pos = 2*i + 1;
            return 0;
        }
    }
    // searchKey is greater than all the keys stored in the node
    pid = intBuffer[2*(getKeyCount()-1)+3];
    pos = 2*(getKeyCount()-1)+3;

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
    int *intBuffer = (int *) buffer;

    intBuffer[1] = pid1;
    intBuffer[2] = key;
    intBuffer[3] = pid2;
    updateKeyCount(true);

    //fprintf(stderr, "Max: %d\n", getMaxCount());

    return 0;
}

/**************************** NonLeafNode Helper Functions ******************/
void BTNonLeafNode::updateKeyCount(bool increment)
{

  int *intBuffer = (int*) buffer;
  if(increment)
    intBuffer[0]++;
  else
    intBuffer[0]--;
}

void BTNonLeafNode::printNode()
{
	int *intBuffer = (int*) buffer;
	int numkeys = getKeyCount();
	fprintf(stderr, "Keys: %d\n", numkeys);

	int pid;
	int key;

	for (int i = 0; i < numkeys; i++)
	{
		pid = intBuffer[2*i+2-1];
		key = intBuffer[2*i+2];
		fprintf(stderr, "Pid: %d Key: %d\n", pid, key);
	}
	fprintf(stderr, "Last Pid: %d\n", intBuffer[2*numkeys+1]);
	
}

RC BTNonLeafNode::shift(const int pos)
{
    if (pos < 0)
        return RC_INVALID_CURSOR;

    
    int *intBuffer = (int*) buffer;
    // manually shift each element in buffer, in blocks of 2, since each entry has 2 elements
    for (int i = 2*getKeyCount()+1; i >= pos; i--) 
    {
        //fprintf(stderr, "%d ", intBuffer[i]);
        intBuffer[i+2] = intBuffer[i];  
        intBuffer[i] = -1;
        //fprintf(stderr, "%d ", intBuffer[i+3]);
    }
}

RC BTNonLeafNode::deleteEntry(const int eid)
{
	if (eid >= getKeyCount())
		return RC_NO_SUCH_RECORD;

	int* intBuffer = (int*) buffer;

	intBuffer[2*eid+1] = -1;
	intBuffer[2*eid+1+1] = -1; 

	updateKeyCount(false);

	return 0; 

}