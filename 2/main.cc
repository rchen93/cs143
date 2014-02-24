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


void testSuite()
{
  //testRead();
  //testLocate();
 // testInsert();
  testSplit();
}

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  //SqlEngine::run(stdin);

  testSuite();
 /* BTLeafNode test;
  BTLeafNode test_sibling;
  RecordId rid;
  int key;
  int eid;
  int siblingKey;
  RC rc;

  rid.pid=7;
  rid.sid=8; 

  test_sibling.emptyNode();
  //test_sibling.printNode();
  fprintf(stderr, "Keys Before sibling: %d\n", test_sibling.getKeyCount());
  fprintf(stderr, "Keys Before orig: %d\n", test.getKeyCount());
  rc = test.insertAndSplit(125, rid, test_sibling, siblingKey);
  //fprintf(stderr, "RC: %d\n", rc);
  fprintf(stderr, "Sibkey: %d\n", siblingKey);
  fprintf(stderr, "Keys After sibling: %d\n", test_sibling.getKeyCount());
  fprintf(stderr, "Keys After orig: %d\n", test.getKeyCount());
  /*
  test_sibling.readEntry(0, key, rid);
  fprintf(stderr, "Key: %d\nPid: %d\nSid: %d\n", key, rid.pid, rid.sid);  
  test_sibling.readEntry(1, key, rid);
  fprintf(stderr, "Key: %d\nPid: %d\nSid: %d\n", key, rid.pid, rid.sid); 
  test.readEntry(41, key, rid);
  fprintf(stderr, "Key: %d\nPid: %d\nSid: %d\n", key, rid.pid, rid.sid); 
  test.readEntry(43, key, rid);
  fprintf(stderr, "Key: %d\nPid: %d\nSid: %d\n", key, rid.pid, rid.sid);
  test.readEntry(44, key, rid);
  fprintf(stderr, "Key: %d\nPid: %d\nSid: %d\n", key, rid.pid, rid.sid);
  */
 /* test.printNode();
  fprintf(stderr, "------------------------------------");
  test_sibling.printNode();*/

  fprintf(stderr, "All tests passed!\n");
  return 0;
}
