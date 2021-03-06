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
#include "BTreeIndex.h"
#include "RecordFile.h"

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
  fprintf(stderr, "---------------testLocate2()------------\n");
  BTNonLeafNode root;
  PageId pid1 = 0;
  int key = 3;
  PageId pid2 = 2;
  root.initializeRoot(pid1, key, pid2);
  int pos = 0; 

  PageId pid;

  root.locateChildPtr(-1, pid, pos);
  if (pid == 0)
  {
    fprintf(stderr, "Found! Pid: %d\n", pid);
  }

  BTNonLeafNode test(5, true);
  if (test.locateChildPtr(3, pid, pos) == 0 && pid == 2)
  {
    fprintf(stderr, "Found! Pid: %d\n", pid);
  }

  if(test.locateChildPtr(9, pid, pos) == 0 && pid == 5)
  {
    fprintf(stderr, "Found! Pid: %d\n", pid);
  }
  
  test.printNode(); 
}

void testInsert2()
{
  fprintf(stderr, "---------------testInsert2()------------\n");
  // insert at end
  BTNonLeafNode test(5, false);
  test.insert(6, 5);
  test.printNode();

  // insert in middle
  BTNonLeafNode test1(5, true);
  test1.insert(3, 90);
  test1.printNode();

  // insert at beginning
  test1.insert(-2, -2);
  test1.printNode();
}

void testSplit2()
{
  fprintf(stderr, "---------------testSplit2()------------\n");

  int key = 3;
  PageId pid = 1000;
  int midkey;
  BTNonLeafNode empty;
  BTNonLeafNode test(5, true);

  // Split in the middle
  test.printNode();
  test.insertAndSplit(key, pid, empty, midkey);
  fprintf(stderr, "midKey: %d\n", midkey);
  test.printNode();
  empty.printNode();

  fprintf(stderr, "\n");
  
  // Split at beginning
  BTNonLeafNode test2(4, true);
  BTNonLeafNode empty2;

  test2.insertAndSplit(-5, pid, empty2, midkey);
  fprintf(stderr, "midKey: %d\n", midkey);
  test2.printNode();
  empty2.printNode();

  fprintf(stderr, "\n");
  
  // Split at end
  BTNonLeafNode test3(5, true);
  BTNonLeafNode empty3;
  int key3 = 11;
  test3.insertAndSplit(key3, pid, empty3, midkey);
  fprintf(stderr, "midKey: %d\n", midkey);
  test3.printNode();
  empty3.printNode();

}

void testNonLeafNode()
{
  testLocate2();
  testInsert2();
  testSplit2();
}

// Make sure getMaxCount() returns 2 
void testIndex()
{
  RecordFile rf;
  BTreeIndex index;
  IndexCursor cursor;
  RecordId rid;
  int key;
  PageId left;
  PageId right;

  index.open("test.idx", 'w');
  
  rid.pid = 0;
  rid.sid = 0;
  index.insert(8, rid);

  rid.pid = 0;
  rid.sid = 1;
  index.insert(5, rid);

  rid.pid = 0;
  rid.sid = 2;
  index.insert(1, rid);

  fprintf(stderr, "LeafOverflow->NewRoot\n");
  index.readRoot(8, 3, left, right);
  fprintf(stderr, "Key: 8 rootPid: 3 Left: %d Right: %d\n", left, right);   // Left: 1 Right: 2

  rid.pid = 0;
  rid.sid = 3;
  index.insert(7, rid);

  fprintf(stderr, "UpdateRoot\n");
  index.readRoot(7, 3, left, right);
  fprintf(stderr, "Key: 7 rootPid: 3 Left: %d Right: %d\n", left, right);   // Left: 1 Right: 4
  index.readRoot(8, 3, left, right);
  fprintf(stderr, "Key: 8 rootPid: 3 Left: %d Right: %d\n", left, right);   // Left: 4 Right: 2

  rid.pid = 0;
  rid.sid = 4;
  index.insert(3, rid);

  fprintf(stderr, "LeafOverflow->NonLeafOverflow->NewRoot\n");
  index.readRoot(7, 7, left, right);
  fprintf(stderr, "Key: 7 rootPid: 7 Left: %d Right: %d\n", left, right);   // Left: 3 Right: 6
  index.readRoot(5, 3, left, right);
  fprintf(stderr, "Key: 5 rootPid: 3 Left: %d Right: %d\n", left, right);   // Left: 1 Right: 5
  index.readRoot(8, 6, left, right);
  fprintf(stderr, "Key: 8 rootPid: 6 Left: %d Right: %d\n", left, right);   // Left: 4 Right: 2

  rid.pid = 0;
  rid.sid = 5;
  index.insert(12, rid);

  rid.pid = 0;
  rid.sid = 6;
  index.insert(9, rid);

  fprintf(stderr, "LeafOverflow\n");
  index.readRoot(7, 7, left, right);
  fprintf(stderr, "Key: 7 rootPid: 7 Left: %d Right: %d\n", left, right);   // Left: 3 Right: 6
  index.readRoot(5, 3, left, right);
  fprintf(stderr, "Key: 5 rootPid: 3 Left: %d Right: %d\n", left, right);   // Left: 1 Right: 5
  index.readRoot(8, 6, left, right);
  fprintf(stderr, "Key: 8 rootPid: 6 Left: %d Right: %d\n", left, right);   // Left: 4 Right: 2
  index.readRoot(12, 6, left, right);
  fprintf(stderr, "Key: 12 rootPid: 6 Left: %d Right: %d\n", left, right);   // Left: 2 Right: 8

  rid.pid = 0;
  rid.sid = 7;
  index.insert(6, rid);

  int count = 0;
  cursor.pid = 1;
  cursor.eid = 0;

  while(!index.readForward(cursor, key, rid))
  {
    fprintf(stderr, "Key: %d\n", key);
    fprintf(stderr, "UpdatedCursor Pid: %d Eid: %d\n", cursor.pid, cursor.eid);
    count++;
  }
  fprintf(stderr, "Total Keys: %d\n", count);

  index.close();

}

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  SqlEngine::run(stdin);

  //testLeafNode();

  //testNonLeafNode();

  //testIndex(); 
  /*
  BTreeIndex index;
  IndexCursor cursor;
  int key;
  RecordId rid;

  index.open("xsmall.idx", 'w');

  index.locate(0, cursor);

  while(!index.readForward(cursor, key, rid))
  {
    fprintf(stderr, "Key: %d\n", key);
    fprintf(stderr, "UpdatedCursor Pid: %d Eid: %d\n", cursor.pid, cursor.eid);
  }

  index.close();

  */
  return 0;
}
