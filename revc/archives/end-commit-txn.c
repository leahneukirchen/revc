/* end-commit-txn.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/rename.h"
#include "revc/deps/ensure-dir-exists.h"
#include "revc/api/constants.h"
#include "revc/archives/revision-path.h"
#include "revc/archives/revision-signature-filename.h"
#include "revc/archives/end-get-txn.h"
#include "revc/archives/end-commit-txn.h"

/* revc_dangle -- should work with fq revision names */

int
revc_end_commit_txn (const t_uchar * const errname,
                     struct revc_archives * const archs,
                     int const txn_n,
                     const t_uchar * const revision_name,
                     const t_uchar * const ticket_blob,
                     const t_uchar * const commit_ticket_blob)
{
  int answer = -69;
  t_uchar * dir = file_name_directory_file (0, archs->_commits[txn_n].tmp_dir);
  t_uchar * name_dir = revc__revision_path (errname, dir, revision_name);
  t_uchar * signature = revc__revision_signature_filename (errname, ticket_blob, commit_ticket_blob);
  t_uchar * final_name = file_name_in_vicinity (0, name_dir, signature);

  if (!name_dir || !signature)
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, dir);
      lim_free (0, name_dir);
      lim_free (0, signature);
      lim_free (0, final_name);
      return answer;
    }

  if (0 > revc_ensure_dir_exists (errname, name_dir))
    goto bail;

                                                
  if (revc_rename (errname, archs->_commits[txn_n].tmp_dir, final_name))
    goto bail;

  if (revc_end_get_txn (errname, archs, archs->_commits[txn_n].get_n))
    goto bail;

  mem_set0 ((t_uchar *)&archs->_commits[txn_n], sizeof (archs->_commits[txn_n]));
  archs->_commits[txn_n].next = archs->_first_free_commmit;
  archs->_first_free_commmit = txn_n;

  answer = 0;
  goto leave;
}



/* arch-tag: Tom Lord Tue Jun 28 09:05:29 2005 (archives/end-commit-txn.c)
 */
