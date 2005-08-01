/* begin-commit-txn.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/error.h"
#include "revc/deps/rmrf.h"
#include "revc/deps/ensure-dir-exists.h"
#include "revc/api/constants.h"
#include "revc/archives/begin-get-txn.h"
#include "revc/archives/begin-commit-txn.h"

int
revc_begin_commit_txn (const t_uchar * const errname,
                       struct revc_archives * const archs,
                       const t_uchar * const revname,
                       const t_uchar * const prereqs,
                       size_t prereqs_size)
{
  int answer;

  if (archs->_first_free_commmit < 0)
    {
      revc_error (errname, "maximum number of simultaneous commits exceeded\n");
      return -1;
    }

  answer = archs->_first_free_commmit;
  archs->_first_free_commmit = archs->_commits[answer].next;

  mem_set0 ((t_uchar *)&archs->_commits[answer], sizeof (archs->_commits[answer]));
  archs->_commits[answer].get_n = revc_begin_get_txn (errname, archs, revname, prereqs, prereqs_size);
  if (0 > archs->_commits[answer].get_n)
    return -1;

  /* revc_dangle: not multi-process safe. */
  archs->_commits[answer].tmp_dir = file_name_in_vicinity (0, archs->_archive_dir, ",,commit");
  revc_rmrf (errname, archs->_commits[answer].tmp_dir);
  if (revc_ensure_dir_exists (errname, archs->_commits[answer].tmp_dir))
    return -1;

  return answer;
}



/* arch-tag: Tom Lord Tue Jun 28 09:04:13 2005 (begin-commit-txn.c)
 */
