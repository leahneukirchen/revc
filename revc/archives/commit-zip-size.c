/* commit-zip-size.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/archives/commit-zip-size.h"

int
revc_commit_zip_size (const t_uchar * const errname,
                      size_t * zsize_ret,
                      struct revc_archives * archs,
                      int txn_n)
{
  if (zsize_ret)
    *zsize_ret = archs->_commits[txn_n].total_zip_size;
  return 0;
}
                      



/* arch-tag: Tom Lord Sat Jul  9 09:22:02 2005 (commit-zip-size.c)
 */
