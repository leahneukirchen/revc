/* list-merge.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/api/traversal-cmp.h"
#include "revc/api/check-list.h"
#include "revc/api/list-skip.h"
#include "revc/api/list-merge.h"

int
revc_list_merge (const t_uchar * const errname,
                 t_uchar ** const str_ret,
                 size_t  *  const size_ret,
                 t_uchar *  const a,
                 size_t     const a_size,
                 t_uchar *  const b,
                 size_t     const b_size)
{
  t_uchar * answer = lim_malloc (0, a_size + b_size);
  size_t size = 0;

  t_uchar * a_pos = a;
  size_t a_remain = a_size;

  t_uchar * b_pos = b;
  size_t b_remain = b_size;

  if (revc_check_list (errname, a, a_size))
    return -1;

  if (revc_check_list (errname, b, b_size))
    return -1;

  while (a_remain && b_remain)
    {
      int const cmp = revc_traversal_cmp (a_pos, b_pos);
      t_uchar * next = (cmp <= 0 ? a_pos : b_pos);
      size_t next_size = str_length (next) + 1;

      mem_move (answer + size, next, next_size);
      size += next_size;

      if (cmp <= 0)
        revc_list_skip (&a_pos, &a_remain);

      if (cmp >= 0)
        revc_list_skip (&b_pos, &b_remain);
    }

  if (b_remain)
    {
      a_pos = b_pos;
      a_remain = b_remain;
      b_pos = 0;
      b_remain = 0;
    }

  while (a_remain)
    {
      size_t next_size = str_length (a_pos) + 1;

      mem_move (answer + size, a_pos, next_size);
      size += next_size;

      revc_list_skip (&a_pos, &a_remain);
    }

  answer = lim_realloc (0, answer, size);

  *str_ret = answer;
  *size_ret = size;

  return 0;
}
                 



/* arch-tag: Tom Lord Thu Jun 23 14:16:50 2005 (list-merge.c)
 */
