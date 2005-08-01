/* zput-from-str.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "revc/deps/put-from-str.h"
#include "revc/api/zip-string.h"
#include "revc/api/zput-from-str.h"

int
revc_zput_from_str (const t_uchar * const errname,
                    size_t * zip_size_ret,
                    const t_uchar * const path,
                    const t_uchar * const data,
                    size_t const size)
{
  t_uchar * zipped = 0;
  size_t zsize = 0;
  int answer = -69;

  if (revc_zip_string (errname, &zipped, &zsize, data, size))
    return -1;

  answer = revc_put_from_str (errname, path, zipped, zsize, 0);

  if ((answer >= 0) && zip_size_ret)
    *zip_size_ret = zsize;

  lim_free (0, zipped);
  zipped = 0;
  zsize = 0;

  return answer;
}



/* arch-tag: Tom Lord Sun Jul  3 06:54:37 2005 (zput-from-str.c)
 */
