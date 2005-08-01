/* commit-n-blobs.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/archives/commit-n-blobs.h"

int
revc_commit_n_blobs (const t_uchar * const errname,
                      size_t * n_ret,
                      struct revc_archives * archs,
                      int txn_n)
{
  if (n_ret)
    *n_ret = archs->_commits[txn_n].n_blobs;
  return 0;
}
                      



/* arch-tag: Tom Lord Sat Jul  9 12:26:30 2005 (archives/commit-n-blobs.c)
 */
