/* zput-from-file.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "revc/deps/get-to-string.h"
#include "revc/api/zput-from-str.h"
#include "revc/api/zput-from-file.h"

int
revc_zput_from_file (const t_uchar * const errname,
                     size_t * file_size_ret,
                     size_t * zip_size_ret,
                     const t_uchar * const path_out,
                     const t_uchar * const path_in)
{
  t_uchar * data = 0;
  size_t size = 0;
  int answer;

  if (revc_get_to_string (errname, &data, &size, path_in))
    return -1;

  answer = revc_zput_from_str (errname, zip_size_ret, path_out, data, size);

  lim_free (0, data);

  if (file_size_ret)
    *file_size_ret = size;

  return answer;
}



/* arch-tag: Tom Lord Sun Jul  3 09:54:17 2005 (api/zput-from-file.c)
 */
