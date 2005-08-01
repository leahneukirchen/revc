/* put-commit-prereqs.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/fs/file-names.h"
#include "revc/api/zput-from-str.h"
#include "revc/archives/put-commit-prereqs.h"

int
revc_put_commit_prereqs (const t_uchar * const errname,
                         struct revc_archives * archs,
                         int txn_n,
                         const t_uchar * const blob,
                         const t_uchar * const prereqs,
                         size_t const prereqs_size)
{
  t_uchar * prereqs_path = file_name_in_vicinity (0, archs->_commits[txn_n].tmp_dir, "prereqs");
  int status = revc_zput_from_str (errname, 0, prereqs_path, prereqs, prereqs_size);

  lim_free (0, prereqs_path);
  return status;
}



/* arch-tag: Tom Lord Thu Jul  7 10:54:32 2005 (archives/put-commit-prereqs.c)
 */
