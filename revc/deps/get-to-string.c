/* get-to-string.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/get-to-string.h"

int
revc_get_to_string (const t_uchar * const errname,
                    t_uchar ** const str_ret,
                    size_t * const size_ret,
                    const t_uchar * const file)
{
  int fd;

  if (file)
    fd = safe_open (file, O_RDONLY, 0);
  else
    fd = 0;

  safe_file_to_string (str_ret, size_ret, fd);

  if (file)
    safe_close (fd);

  return 0;
}



/* arch-tag: Tom Lord Thu Jun 23 13:01:14 2005 (get-to-string.c)
 */
