/* revision-traversal-end.c: 
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
#include "revc/archives/get-blob.h"
#include "revc/api/revision-traversal-end.h"

int
revc_revision_traversal_end (const t_uchar * const errname,
                             struct revc_traversal ** trav)
{
  struct revc_revision_traversal * rtrav = (struct revc_revision_traversal *)*trav;

  while (rtrav->stack)
    {
      struct revc_revision_traversal_stack * s = rtrav->stack;

      lim_free (0, s->dir);
      rtrav->stack = s->prev;
      lim_free (0, s);
    }
  lim_free (0, (*trav)->node);
  (*trav)->node = 0;
  lim_free (0, *trav);
  *trav = 0;
  return 0;
}




/* arch-tag: Tom Lord Sat Jul  2 19:44:36 2005 (api/revision-traversal-end.c)
 */
