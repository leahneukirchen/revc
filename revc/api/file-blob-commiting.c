/* file-blob-commiting.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/archives/commit-needs-file-blob.h"
#include "revc/api/constants.h"
#include "revc/api/file-blob.h"
#include "revc/api/file-blob-commiting.h"

int
revc_file_blob_committing (const t_uchar * const errname,
                           t_uchar * const blob_ret,
                           struct revc_archives * const archs,
                           int const txn_n,
                           const t_uchar * const path,
                           size_t size)
{
  t_uchar blob_addr[REVC_MAX_BLOB_ADDR_SIZE + 1];

  if (revc_file_blob (errname, blob_addr, path, size))
    return -1;

  if (revc_commit_needs_file_blob (errname, archs, txn_n, blob_addr, path))
    return -1;

  if (blob_ret)
    str_cpy (blob_ret, blob_addr);


  return 0;
}




/* arch-tag: Tom Lord Sat Jul  2 10:01:21 2005 (file-blob-commiting.c)
 */
