/* str-to-lines.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "hackerlab/hash/hash-utils.h"
#include "hackerlab/arrays/ar.h"
#include "revc/api/str-to-lines.h"

int
revc_str_to_lines (const t_uchar * const errname,
                   struct revc_diff_line ** ret,
                   const t_uchar * const str,
                   size_t const size)
{
  struct revc_diff_line * answer = 0;
  const t_uchar * pos = str;
  size_t remain = size;

  while (remain)
    {
      const t_uchar * end = str_chr_index_n (pos, remain, '\n');
      struct revc_diff_line * entry = (struct revc_diff_line *)ar_push ((void **)&answer, 0, sizeof (*entry));
      
      if (!end)
        end = str + size - 1;

      entry->start = (pos - str);
      entry->end = (end + 1 - str);
      entry->hash = hash_mem (pos, (end + 1 - pos));

      remain -= (end + 1 - pos);
      pos += (end + 1 - pos);
    }

  if (!ret)
    lim_free (0, answer);
  else
    *ret = answer;

  return 0;
}



/* arch-tag: Tom Lord Tue Jul 12 13:57:44 2005 (str-to-lines.c)
 */
