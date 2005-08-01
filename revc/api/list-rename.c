/* list-rename.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "revc/api/list-find.h"
#include "revc/api/check-list.h"
#include "revc/api/traversal-cmp.h"
#include "revc/api/list-find-subdir.h"
#include "revc/api/list-rename.h"


/* __STDC__ prototypes for static functions */
static size_t insert_renamed (t_uchar * const out,
                              t_uchar * const subdir,
                              size_t const subdir_size,
                              const t_uchar * const from,
                              const t_uchar * const to);


int
revc_list_rename (const t_uchar  *       const errname,
                  t_uchar        **      const str_ret,
                  size_t         *       const size_ret,
                  t_uchar        *       const str,
                  size_t                 const size,
                  t_uchar        *       const from,
                  t_uchar        *       const to)
{
  int from_to_cmp = -69;
  t_uchar * from_start = 0;
  size_t from_start_head_size = 0;
  size_t from_start_tail_size = 0;
  t_uchar * from_subdir_start = 0;
  size_t from_subdir_size = 0;
  t_uchar * from_end = 0;
  t_uchar * to_start = 0;
  size_t to_start_head_size = 0;
  size_t to_start_tail_size = 0;
  t_uchar * to_subdir_start = 0;
  size_t to_subdir_size = 0;
  t_uchar * to_end = 0;
  size_t n_renamed;
  ssize_t from_size_change;
  ssize_t answer_size;
  t_uchar * answer = 0;
  t_uchar * left_of_both = 0;
  size_t left_of_both_size = 0;
  t_uchar * middle = 0;
  size_t middle_size = 0;
  t_uchar * right_of_both = 0;
  size_t right_of_both_size = 0;
  int insert_before_middle = -69;
  size_t fill_pos;
  int tmp;
  
  if (revc_check_list (errname, str, size))
    return -1;
  
  from_to_cmp = revc_traversal_cmp (from, to);

  tmp = revc_list_find (errname, &from_start, str, size, from, 1, revc_traversal_cmp);
  if (tmp < 0)
    return -1;
  else if (!tmp || !from_to_cmp)
    {
      t_uchar * answer = lim_malloc (0, size);
      mem_move (answer, str, size);
      *str_ret = answer;
      *size_ret = size;
      return 0;
    }

  from_start_head_size = (from_start - str);
  from_start_tail_size = (size - (from_start - str));

  if (revc_list_find_subdir (errname,
                             &from_subdir_start, &from_subdir_size,
                             from,
                             from_start, from_start_tail_size))
    return -1;

  from_end = from_subdir_start + from_subdir_size;

  if (from_to_cmp < 0)
    {
      if (0 > revc_list_find (errname, &to_start, from_start, from_start_tail_size, to, 0, revc_traversal_cmp))
        return -1;
    }
  else
    {
      if (0 > revc_list_find (errname, &to_start, str, from_start_head_size, to, 0, revc_traversal_cmp))
        return -1;
    }

  to_start_head_size = (to_start - str);
  to_start_tail_size = (size - (to_start - str));

  if (revc_list_find_subdir (errname,
                             &to_subdir_start, &to_subdir_size,
                             to,
                             to_start, to_start_tail_size))
    return -1;

  to_end = to_subdir_start + to_subdir_size;

  n_renamed = mem_occurrences (from_start, 0, (from_end - from_start));
  from_size_change = n_renamed * ((ssize_t)str_length (to) - (ssize_t)str_length (from));

  answer_size = (size_t)((ssize_t)size + from_size_change);
  answer = lim_malloc (0, answer_size);

  if (from_to_cmp < 0)
    {
      left_of_both = str;
      left_of_both_size = from_start_head_size;
    }
  else
    {
      left_of_both = str;
      left_of_both_size = to_start_head_size;
    }

  if (to_end < from_start)
    {
      middle = to_end;
      middle_size = from_start - to_end;
      insert_before_middle = 1;
    }
  else if (from_end < to_start)
    {
      middle = from_end;
      middle_size = to_start - from_end;
      insert_before_middle = 0;
    }
  else
    {
      middle = 0;
      middle_size = 0;
      insert_before_middle = 1;
    }

  if (to_end > from_end)
    {
      right_of_both = to_end;
      right_of_both_size = (size - (to_end - str));
    }
  else
    {
      right_of_both = from_end;
      right_of_both_size = (size - (from_end - str));
    }

  mem_move (answer, left_of_both, left_of_both_size);

  fill_pos = left_of_both_size;

  if (insert_before_middle)
    fill_pos += insert_renamed (answer + fill_pos, from_start, from_end - from_start, from, to);

  mem_move (answer + fill_pos, middle, middle_size);
  fill_pos += middle_size;

  if (!insert_before_middle)
    fill_pos += insert_renamed (answer + fill_pos, from_start, from_end - from_start, from, to);

  mem_move (answer + fill_pos, right_of_both, right_of_both_size);
  fill_pos += right_of_both_size;

  *str_ret = answer;
  *size_ret = fill_pos;

  return 0;
}


static size_t
insert_renamed (t_uchar * const out,
                t_uchar * const subdir,
                size_t const subdir_size,
                const t_uchar * const from,
                const t_uchar * const to)
{
  size_t const f_len = str_length (from);
  size_t const t_len = str_length (to);
  t_uchar * in_pos = subdir;
  t_uchar * in_end = subdir + subdir_size;
  t_uchar * out_pos = out;

  while (in_pos < in_end)
    {
      mem_move (out_pos, to, t_len);
      str_cpy (&out_pos[t_len], in_pos + f_len);
      out_pos = out_pos + str_length (out_pos) + 1;
      in_pos = in_pos + str_length (in_pos) + 1;
    }

  return out_pos - out;
}




/* arch-tag: Tom Lord Fri Jun 24 12:53:54 2005 (list-rename.c)
 */
