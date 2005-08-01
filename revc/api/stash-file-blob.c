/* stash-file-blob.c: 
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
#include "revc/deps/ensure-dir-exists.h"
#include "revc/api/zput-from-file.h"
#include "revc/api/stash-file-blob.h"

int
revc_stash_file_blob (const t_uchar * const errname,
                      size_t * file_size_ret,
                      size_t * zip_size_ret,
                      const t_uchar * const dir,
                      const t_uchar * const blob,
                      const t_uchar * const path)
{
  t_uchar subdir[3];
  t_uchar * subdir_path = 0;
  t_uchar * blob_path = 0;
  int answer = -69;

  str_cpy_n (subdir, blob, 2);
  if (subdir[0] == 0)
    str_cpy (subdir, "XX");

  subdir_path = file_name_in_vicinity (0, dir, subdir);
  blob_path = file_name_in_vicinity (0, subdir_path, blob);

  if (revc_ensure_dir_exists (errname, subdir_path))
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, subdir_path);
      lim_free (0, blob_path);
      return answer;
    }

  if (revc_zput_from_file (errname, file_size_ret, zip_size_ret, blob_path, path))
    goto bail;

  answer = 0;
  goto leave;
}




/* arch-tag: Tom Lord Sun Jul  3 09:52:20 2005 (api/stash-file-blob.c)
 */
