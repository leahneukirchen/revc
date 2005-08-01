/* commit-txn-has-hint.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/archives/commit-txn-has-hint.h"


t_uchar *
revc_commit_txn_has_hint (const t_uchar * const errname,
                          struct revc_archives * archs,
                          int txn_n)
{
  return archs->_commits[txn_n].has_hint;
}



/* arch-tag: Tom Lord Thu Jul  7 10:47:09 2005 (commit-txn-has-hint.c)
 */
