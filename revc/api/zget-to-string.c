/* zget-to-string.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "revc/deps/get-to-string.h"
#include "revc/api/constants.h"
#include "revc/api/unzip-string.h"
#include "revc/api/zget-to-string.h"

int
revc_zget_to_string (const t_uchar * const errname,
                     t_uchar ** const str_ret,
                     size_t * const size_ret,
                     const t_uchar * const path)
{
  t_uchar * zipped = 0;
  size_t zsize = 0;
  int answer = -69;

  if (revc_get_to_string (errname, &zipped, &zsize, path))
    return -1;

  answer = revc_unzip_string (errname, str_ret, size_ret, zipped, zsize);

  lim_free (0, zipped);
  zipped = 0;
  zsize = 0;

  return answer;
}



/* arch-tag: Tom Lord Sun Jul  3 08:05:12 2005 (api/zget-to-str.c)
 */
