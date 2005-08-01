/* find-in-inventory.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/api/list-skip.h"
#include "revc/api/check-list.h"
#include "revc/api/list-find.h"



int
revc_list_find (const t_uchar * const errname,
                t_uchar ** const answer,
                t_uchar * const inv,
                size_t const len,
                t_uchar * name,
                int exact,
                int (*cmp) (const t_uchar * const a,
                            const t_uchar * const b))
{
  t_uchar * pos = inv;
  size_t remain = len;

  if (revc_check_list (errname, inv, len))
    return -1;

  while (remain && (cmp (pos, name) < 0))
    revc_list_skip (&pos, &remain);

  if (remain && (cmp (pos, name) == 0))
    {
      if (answer)
        *answer = pos;
      return 1;
    }
  else if (!exact)
    {
      if (answer)
        *answer = pos;
      return 0;
    }
  else
    {
      return 0;
    }
}





/* arch-tag: Tom Lord Sat Jun 18 09:49:05 2005 (librevc/find-in-inventory.c)
 */
