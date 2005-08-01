/* add-selected-subtrees.c: 
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
#include "hackerlab/fs/file-names.h"
#include "revc/api/traversal-cmp.h"
#include "revc/api/list-skip.h"
#include "revc/api/list-find.h"
#include "revc/api/list-find-subdir.h"
#include "revc/api/add-selected-subtrees.h"


int
revc_add_selected_subtrees (const t_uchar * const errname,
                            t_uchar ** str_ret, size_t * size_ret,
                            t_uchar * const a, size_t a_size,
                            t_uchar * const b, size_t b_size,
                            t_uchar * const b_select, size_t b_select_size)
{
  t_uchar * answer = 0;
  size_t answer_size = 0;

  t_uchar * a_pos = a; size_t a_remain = a_size;
  t_uchar * b_pos = b; size_t b_remain = b_size;
  t_uchar * select_pos = b_select; size_t select_remain = b_select_size;

  while (select_remain || a_remain)
    {
      int trav_cmp = ((select_remain && a_remain)
                      ? revc_traversal_cmp (a_pos, select_pos)
                      : (select_remain
                         ? 1
                         : -1));

      if (trav_cmp < 0)
        {
          size_t len = str_length (a_pos);

          answer = lim_realloc (0, answer, answer_size + len + 1);
          mem_move (answer + answer_size, a_pos, len + 1);
          answer_size += len + 1;
          revc_list_skip (&a_pos, &a_remain);
        }
      else
        {
          int have_both = (select_remain && a_remain);
          int sel_is_prefix = (!have_both ? 0 :  file_name_is_path_prefix (select_pos, a_pos));
          t_uchar * pos_in_b = 0;
          int found_in_b;
          t_uchar * a_subtree = 0;
          size_t a_subtree_size = 0;
          t_uchar * b_subtree = 0;
          size_t b_subtree_size = 0;
          t_uchar * select_subtree = 0;
          size_t select_subtree_size = 0;
          

          if (sel_is_prefix)
            {
              if (revc_list_find_subdir (errname, &a_subtree, &a_subtree_size, a_pos, a_pos, a_size))
                {
                bail:
                  lim_free (0, answer);
                  return -1;
                }
              a_pos = (a_subtree + a_subtree_size);
              a_remain = (a_size - (a_pos - a));
            }

          found_in_b = revc_list_find (errname, &pos_in_b,
                                       b_pos, b_remain,
                                       select_pos,
                                       0,
                                       revc_traversal_cmp);
          if (found_in_b < 0)
            goto bail;


          {
            size_t amt;

            if (revc_list_find_subdir (errname, &b_subtree, &b_subtree_size, select_pos, b_pos, b_remain))
              goto bail;
              
            amt = ((b_subtree + b_subtree_size) - pos_in_b);
            answer = lim_realloc (0, answer, answer_size + amt);
            mem_move (answer + answer_size, pos_in_b, amt);
            answer_size += amt;

            b_pos = b_subtree + b_subtree_size;
            b_remain = (b_size - (b_pos - b));
          }

          if (revc_list_find_subdir (errname, &select_subtree, &select_subtree_size,
                                     select_pos,
                                     select_pos, select_remain))
            goto bail;

          select_pos = select_subtree + select_subtree_size;
          select_remain = (b_select_size - (select_pos - b_select));
        }
    }

  if (str_ret)
    *str_ret = answer;
  else
    lim_free (0, answer);

  if (size_ret)
    *size_ret = answer_size;

  return 0;
}




/* arch-tag: Tom Lord Mon Jul  4 15:12:48 2005 (add-selected-subtrees.c)
 */
