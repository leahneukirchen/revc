/* commit-needs-file-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "revc/api/add-blob-to-hint-set.h"
#include "revc/api/stash-file-blob.h"
#include "revc/archives/get-blob.h"
#include "revc/archives/commit-needs-file-blob.h"

int
revc_commit_needs_file_blob (const t_uchar * const errname,
                             struct revc_archives * archs,
                             int txn_n,
                             const t_uchar * const blob,
                             const t_uchar * const path)
{
  int already;
  size_t file_size;
  size_t zip_size;

  revc_add_blob_to_hint_set (archs->_commits[txn_n].needs_hint, blob);

  already = revc_get_blob (errname, 0, 0,
                           archs,
                           archs->_gets[archs->_commits[txn_n].get_n].revision,
                           archs->_gets[archs->_commits[txn_n].get_n].prereqs,
                           archs->_gets[archs->_commits[txn_n].get_n].prereqs_size,
                           blob,
                           1);
  if (already < 0)
    return -1;

  if (already)
    return 0;

  if (revc_stash_file_blob (errname, &file_size, &zip_size, archs->_commits[txn_n].tmp_dir, blob, path))
    return -1;

  revc_add_blob_to_hint_set (archs->_commits[txn_n].has_hint, blob);

  archs->_commits[txn_n].total_zip_size += zip_size;
  archs->_commits[txn_n].total_file_size += file_size;
  archs->_commits[txn_n].blob_names = (t_uchar **)lim_realloc (0,
                                                                (void *)archs->_commits[txn_n].blob_names,
                                                                (1 + archs->_commits[txn_n].n_blobs) * sizeof (t_uchar *));
  archs->_commits[txn_n].blob_names[archs->_commits[txn_n].n_blobs] = str_save (0, blob);
  ++archs->_commits[txn_n].n_blobs;
  
  return 0;
}





/* arch-tag: Tom Lord Sun Jul  3 09:50:10 2005 (commit-needs-file-blob.c)
 */
