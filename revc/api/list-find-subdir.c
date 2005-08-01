/* list-find-subdir.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/fs/file-names.h"
#include "revc/api/traversal-cmp.h"
#include "revc/api/check-list.h"
#include "revc/api/list-skip.h"
#include "revc/api/list-find-subdir.h"


/* __STDC__ prototypes for static functions */





int
revc_list_find_subdir (const t_uchar * const errname,
                       t_uchar ** const pos_ret,
                       size_t * size_ret,
                       const t_uchar * const dir,
                       t_uchar * const const list,
                       size_t const size)
{
  t_uchar * pos = list;
  size_t remain = size;
  t_uchar * start = 0;
  t_uchar * end = 0;

  if (revc_check_list (errname, list, size))
    return -1;


  while (remain)
    {
      int cmp = revc_traversal_cmp (dir, pos);

      if (cmp >= 0)
        revc_list_skip (&pos, &remain);
      else
        break;
    }

  start = pos;

  while (remain)
    {
      if (file_name_is_path_prefix (dir, pos))
        revc_list_skip (&pos, &remain);
      else
        break;
    }

  end = pos;

  *pos_ret = start;
  *size_ret = (end - start);

  return 0;
}
                       




/* arch-tag: Tom Lord Thu Jun 23 16:01:20 2005 (list-find-subdir.c)
 */
