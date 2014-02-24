/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "Bruinbase.h"
//#include "SqlEngine.h"
#include "BTreeNode.h"

 /* To redirect virtualbox output to a txt:
 ./bruinbase > output.txt 2>&1
 */

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  //SqlEngine::run(stdin);

  BTLeafNode test;
  BTLeafNode test_sibling;
  RecordId rid;
  int key;
  int eid;
  int siblingKey;
  RC rc;

  rid.pid=7;
  rid.sid=8; 

  fprintf(stderr, "Max: %d\n", test.getMaxCount());

  //test.readEntry(0, key, rid);
  //fprintf(stderr, "Key: %d\nPid: %d\nSid: %d\n", key, rid.pid, rid.sid);  
  //test.locate(2, eid);
  //fprintf(stderr, "Eid: %d\n", eid);
  //test.shift(1);
  //test.insert(2, rid); 
  //test.printNode();

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
  test.printNode();
  fprintf(stderr, "------------------------------------");
  test_sibling.printNode();


  return 0;
}
