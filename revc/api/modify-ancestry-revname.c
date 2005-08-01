/* modify-ancestry-revname.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "revc/deps/error.h"
#include "revc/api/check-list.h"
#include "revc/api/list-skip.h"
#include "revc/api/parse-ancestry-line.h"
#include "revc/api/modify-ancestry-revname.h"

int
revc_modify_ancestry_revname (const t_uchar * const errname,
                              t_uchar ** const ancestry,
                              size_t * ancestry_size,
                              t_uchar * new_revname)
{
  t_uchar * old_revname_start = 0;
  size_t old_prefix_size = 0;
  size_t new_revname_size = 0;
  t_uchar * tail_pos = 0;
  size_t tail_remain = 0;
  size_t new_size;
  t_uchar * answer = 0;

  if (revc_check_list (errname, *ancestry, *ancestry_size))
    return -1;

  if (!*ancestry_size)
    {
      revc_error (errname, "empty ancestor\n");
      return -1;
    }

  if (revc_parse_ancestry_line (errname, 0, 0, &old_revname_start, *ancestry))
    return -1;

  old_prefix_size = (old_revname_start - *ancestry);

  new_revname_size = str_length (new_revname);

  tail_pos = *ancestry;
  tail_remain = *ancestry_size;
  revc_list_skip (&tail_pos, &tail_remain);

  new_size = old_prefix_size + new_revname_size + 1 + tail_remain;

  answer = (t_uchar *)lim_malloc (0, new_size);
  mem_move (answer, *ancestry, old_prefix_size);
  mem_move (answer + old_prefix_size, new_revname, new_revname_size);
  answer[old_prefix_size + new_revname_size] = 0;
  mem_move (answer + old_prefix_size + new_revname_size + 1, tail_pos, tail_remain);

  lim_free (0, *ancestry);
  *ancestry = answer;
  *ancestry_size = new_size;

  return 0;
}



/* arch-tag: Tom Lord Mon Jun 27 17:14:05 2005 (modify-ancestry-revname.c)
 */
