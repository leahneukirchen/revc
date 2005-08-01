/* stashed-blob-to-str.c: 
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
#include "revc/api/zget-to-string.h"
#include "revc/api/stashed-blob-to-str.h"

int
revc_stashed_blob_to_str (const t_uchar * const errname,
                          t_uchar ** const str_ret,
                          size_t * const size_ret,
                          const t_uchar * const dir,
                          const t_uchar * const blob)
{
  t_uchar subdir[3];
  t_uchar * subdir_path = 0;
  t_uchar * blob_path = 0;
  int answer = -69;

  /* dangle -- the api around 'stash' is not quite right */

  str_cpy_n (subdir, blob, 2);
  if (subdir[0] == 0)
    str_cpy (subdir, "XX");

  subdir_path = file_name_in_vicinity (0, dir, subdir);
  blob_path = file_name_in_vicinity (0, subdir_path, blob);

  answer = revc_zget_to_string (errname, str_ret, size_ret, blob_path);

  lim_free (0, subdir_path);
  lim_free (0, blob_path);
  return answer;
}




/* arch-tag: Tom Lord Thu Jul  7 20:04:14 2005 (api/stashed-blob-to-str.c)
 */
