/* commit-blob-contents-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/sort/qsort.h"
#include "hackerlab/char/str.h"
#include "revc/api/qsort-traversal-cmp.h"
#include "revc/api/string-array-blob.h"
#include "revc/archives/commit-blob-contents-blob.h"

static int
cmp (void * va, void * vb, void * ign)
{
  t_uchar * a = *(t_uchar **)va;
  t_uchar * b = *(t_uchar **)vb;

  return str_cmp (a, b);
}

int
revc_commit_blob_contents_blob (const t_uchar * const errname,
                                t_uchar * const blob_blob_ret,
                                struct revc_archives * archs,
                                int txn_n)
{
  quicksort ((void *)archs->_commits[txn_n].blob_names,
             archs->_commits[txn_n].n_blobs,
             sizeof (t_uchar *),
             cmp,
             0);

  if (revc_string_array_blob (errname, blob_blob_ret,
                              archs->_commits[txn_n].blob_names,
                              archs->_commits[txn_n].n_blobs))
    return -1;

  return 0;
}




/* arch-tag: Tom Lord Sat Jul  9 09:33:36 2005 (archives/commit-blob-contents-blob.c)
 */
