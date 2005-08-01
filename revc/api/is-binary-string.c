/* is-binary-string.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/api/is-binary-string.h"

int
revc_is_binary_string (const t_uchar * const str,
                       size_t size)
{
  size_t amt = ((size > 1024) ? 1024 : size);

  return !!str_chr_index_n (str, amt, 0);
}




/* arch-tag: Tom Lord Tue Jul 12 13:46:35 2005 (is-binary-string.c)
 */
