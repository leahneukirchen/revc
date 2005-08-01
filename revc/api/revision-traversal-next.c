/* revision-traversal-next.c: 
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
#include "revc/api/is-empty-blob.h"
#include "revc/api/revision-traversal-next.h"

int
revc_revision_traversal_next (const t_uchar * const errname,
                              struct revc_traversal * trav,
                              int skip_subtree)
{
  struct revc_revision_traversal * rtrav = (struct revc_revision_traversal *)trav;
  int is_empty;

  if (trav->eot)
    return 0;

  is_empty = revc_is_empty_blob (errname, trav->blob);
  if (is_empty < 0)
    return -1;

  if (skip_subtree || (trav->meta[0] != 'd') || is_empty)
    {
      /* dangle: check this loop again -- i'm not sure it's right. */

      while (rtrav->stack && ((rtrav->stack->dir_pos + REVC_DIR_ENTRY_SIZE) < rtrav->stack->dir_size))
        {
          struct revc_revision_traversal_stack * s = rtrav->stack;

          lim_free (0, s->dir);
          rtrav->stack = s->prev;
          lim_free (0, s);

          if (rtrav->stack
              && ((rtrav->stack->dir_pos + REVC_DIR_ENTRY_SIZE) < rtrav->stack->dir_size))
            break;
        }
      if (!rtrav->stack)
        {
          trav->eot = 1;
          lim_free (0, trav->node);
          trav->node = 0;
          mem_set0 (trav->blob, sizeof (trav->blob));
          mem_set0 (trav->meta, sizeof (trav->meta));
          return 0;
        }
      else
        {
          rtrav->stack->dir_pos += REVC_DIR_ENTRY_SIZE;
          lim_free (0, trav->node);
          trav->node = str_save_n (0, REVC_DIR_ENTRY_FNAME (rtrav->stack->dir + rtrav->stack->dir_pos), REVC_MAX_FNAME_SIZE);
          str_cpy_n (trav->blob, REVC_DIR_ENTRY_BLOB_ADDR (rtrav->stack->dir + rtrav->stack->dir_pos), REVC_MAX_BLOB_ADDR_SIZE);
          str_cpy_n (trav->meta, REVC_DIR_ENTRY_METADATA (rtrav->stack->dir + rtrav->stack->dir_pos), REVC_MAX_METADATA_SIZE);
          return 1;
        }
    }
  else
    {
      t_uchar * dir;
      size_t dir_size;
      struct revc_revision_traversal_stack * frame = 0;


      if (0 > revc_get_blob (errname, &dir, &dir_size,
                             rtrav->archs, rtrav->revision, rtrav->prereqs, rtrav->prereqs_size,
                             trav->blob,
                             0))
        return -1;

      frame = (struct revc_revision_traversal_stack *)lim_malloc (0, sizeof (*frame));
      frame->prev = rtrav->stack;
      frame->dir = dir;
      dir = 0;
      frame->dir_size = dir_size;
      frame->dir_pos = 0;

      rtrav->stack = frame;

      lim_free (0, trav->node);

      trav->node = str_save_n (0, REVC_DIR_ENTRY_FNAME (rtrav->stack->dir + rtrav->stack->dir_pos), REVC_MAX_FNAME_SIZE);
      str_cpy_n (trav->blob, REVC_DIR_ENTRY_BLOB_ADDR (rtrav->stack->dir + rtrav->stack->dir_pos), REVC_MAX_BLOB_ADDR_SIZE);
      str_cpy_n (trav->meta, REVC_DIR_ENTRY_METADATA (rtrav->stack->dir + rtrav->stack->dir_pos), REVC_MAX_METADATA_SIZE);
      return 1;
    }
}




/* arch-tag: Tom Lord Sat Jul  2 18:55:10 2005 (revision-traversal-next.c)
 */
