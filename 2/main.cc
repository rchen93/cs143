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

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  //SqlEngine::run(stdin);

  BTLeafNode test;
  RecordId rid;
  int key;

  test.readEntry(0, key, rid);
  fprintf(stderr, "Key: %d\n Pid: %d\n Sid: %d\n", key, rid.pid, rid.sid);  

  return 0;
}
