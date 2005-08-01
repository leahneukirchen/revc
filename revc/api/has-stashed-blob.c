/* has-stashed-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/file-stats.h"
#include "revc/archives/revision-path.h"
#include "revc/api/has-stashed-blob.h"

int
revc_has_stashed_blob (const t_uchar * const errname,
                       const t_uchar * const dir,
                       const t_uchar * const blob)
{
  t_uchar subdir[3];
  t_uchar * subdir_path = 0;
  t_uchar * blob_path = 0;
  t_uchar type;
  int answer = -69;

  str_cpy_n (subdir, blob, 2);

  if (subdir[0] == 0)
    str_cpy (subdir, "XX");

  subdir_path = file_name_in_vicinity (0, dir, subdir);

  blob_path = file_name_in_vicinity (0, subdir_path, blob);

  if (revc_file_stats (errname, &type, 0, 0, blob_path))
    {
    /* bail: */
      answer = -1;
    leave:
      lim_free (0, subdir_path);
      lim_free (0, blob_path);
      return answer;
    }


  if (type == 'f')
    {
      answer = 1;
    }
  else
    {
      answer = 0;
    }

  goto leave;
}




/* arch-tag: Tom Lord Thu Jul  7 20:10:23 2005 (api/has-stashed-blob.c)
 */
