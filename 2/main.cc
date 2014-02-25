/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

/* To redirect virtualbox output to a txt:
 ./bruinbase > output.txt 2>&1
*/
 
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeNode.h"

void testRead()
{
  BTLeafNode test(5, false);
  RecordId rid;
  int key;

  fprintf(stderr, "---------Start testRead()-----------\n");

  // General readEntry
  if (test.readEntry(0, key, rid) == 0)
    fprintf(stderr, "Key: %d Pid: %d Sid: %d\n", key, rid.pid, rid.sid);

  // Out of bounds readEntry
  if (test.readEntry(-1, key, rid) != 0)
    fprintf(stderr, "Invalid eid\n");
  if (test.readEntry(5, key, rid) != 0)
    fprintf(stderr, "Invalid eid\n");
}

void testLocate()
{
  BTLeafNode test(5, true);
  RecordId rid;
  int key;
  int eid;

  test.printNode();

  fprintf(stderr, "------------Start testLocate()----------\n");

  // searchKey > key
  test.locate(7, eid);
  if (eid == 4)
  {
    test.readEntry(eid, key, rid);
    fprintf(stderr, "Found! Eid: %d Key: %d\n", eid, key);
  }

  // searchKey == key
  test.locate(4, eid);
  if (eid == 2)
  {
    test.readEntry(eid, key, rid);
    fprintf(stderr, "Found! Eid: %d Key: %d\n", eid, key);
  }

  // cannot find key
  if (test.locate(19, eid) == RC_NO_SUCH_RECORD)
    fprintf(stderr, "Key does not exist!\n");
}

void testInsert()
{
  BTLeafNode test(5, false);
  RecordId rid;
  int key;
  int eid;

  rid.pid = 1000;
  rid.sid = 1000;

  fprintf(stderr, "---------Start testInsert()-----------\n");

  // Insertion at End
  test.insert(6, rid);
  if (test.locate(6, eid) == 0 && eid == 5)
  {
    test.readEntry(eid, key, rid);
    fprintf(stderr, "Found! Eid: %d Key: %d\n", eid, key);
  }

  // Insertion at first position to keep B+ tree sorted
  test.insert(5, rid);
  if (test.locate(5, eid) == 0 && eid == 5)
  {
    test.readEntry(eid, key, rid);
    fprintf(stderr, "Found! Eid: %d Key: %d\n", eid, key);
  }

  // General insertion
  test.insert(-1, rid);
  if (test.locate(5, eid) == 0 && eid == 0)
  {
    test.readEntry(eid, key, rid);
    fprintf(stderr, "Found! Eid: %d Key: %d\n", eid, key);
  }

  test.printNode();

  BTLeafNode empty(0, false);

  // Empty insertion
  empty.insert(1, rid);
  if (empty.locate(1, eid) == 0 && eid == 0)
  {
    empty.readEntry(eid, key, rid);
    fprintf(stderr, "Found! Eid: %d Key: %d\n", eid, key);
  }

  empty.printNode();
}

void testSplit()
{
  BTLeafNode test(10, true);
  BTLeafNode empty(0, false);
  RecordId rid;
  int key;
  int eid;
  int siblingKey;

  rid.pid=1000;
  rid.sid=1000; 

  test.printNode();

  fprintf(stderr, "------------Start testSplit()----------------\n");

  // Split at end of node
  fprintf(stderr, "Before ");
  fprintf(stderr, "Sibling Keys: %d ", empty.getKeyCount());
  fprintf(stderr, "Orig Keys: %d\n", test.getKeyCount());
  test.insertAndSplit(11, rid, empty, siblingKey);
  fprintf(stderr, "After ");
  fprintf(stderr, "Sibling Keys: %d ", empty.getKeyCount());
  fprintf(stderr, "Orig Keys: %d\n", test.getKeyCount());
  fprintf(stderr, "SiblingKey: %d\n", siblingKey);

  test.printNode();
  empty.printNode();


  BTLeafNode test2(5, true);
  BTLeafNode empty2(0, false);

  test2.printNode();

  // General split
  fprintf(stderr, "Before ");
  fprintf(stderr, "Sibling Keys: %d ", empty2.getKeyCount());
  fprintf(stderr, "Orig Keys: %d\n", test2.getKeyCount());
  test2.insertAndSplit(3, rid, empty2, siblingKey);
  fprintf(stderr, "After ");
  fprintf(stderr, "Sibling Keys: %d ", empty2.getKeyCount());
  fprintf(stderr, "Orig Keys: %d\n", test2.getKeyCount());
  fprintf(stderr, "SiblingKey: %d\n", siblingKey);

  test2.printNode();
  empty2.printNode();

  BTLeafNode test3(5, true);
  BTLeafNode empty3(0, false);

  test3.printNode();

  // Split at beginning
  fprintf(stderr, "Before ");
  fprintf(stderr, "Sibling Keys: %d ", empty3.getKeyCount());
  fprintf(stderr, "Orig Keys: %d\n", test3.getKeyCount());
  test3.insertAndSplit(-1, rid, empty3, siblingKey);
  fprintf(stderr, "After ");
  fprintf(stderr, "Sibling Keys: %d ", empty3.getKeyCount());
  fprintf(stderr, "Orig Keys: %d\n", test3.getKeyCount());
  fprintf(stderr, "SiblingKey: %d\n", siblingKey);

  test3.printNode();
  empty3.printNode();

}

// Tests LeafNode functions
void testLeafNode()
{
  testRead();
  testLocate();
  testInsert();
  testSplit();
}

void testLocate2()
{
  BTNonLeafNode root;
  PageId pid1 = 0;
  int key = 3;
  PageId pid2 = 2;
  int pos;
  root.initializeRoot(pid1, key, pid2);

  PageId pid;

  root.locateChildPtr(2, pid, pos);
  if (pid == 0)
  {
    fprintf(stderr, "Found! Pid: %d\n", pid);
  }

  BTNonLeafNode test(5, true);
  if (test.locateChildPtr(3, pid, pos) == 0 && pid == 3)
  {
    fprintf(stderr, "Found! Pid: %d\n", pid);
  }
  
  test.printNode(); 
}

void testInsert2()
{
  BTNonLeafNode test(5, false);
  PageId pid = 1000;

  test.printNode();

  test.insert(6, pid);

  test.printNode();
}


void testNonLeafNode()
{
  //testLocate2();
  testInsert2();
}

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  //SqlEngine::run(stdin);

  //testLeafNode();

  testNonLeafNode();

  fprintf(stderr, "All tests passed!\n");
  return 0;
}
