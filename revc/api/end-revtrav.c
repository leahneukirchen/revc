/* end-revtrav.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/api/end-revtrav.h"

void
revc_end_revtrav (const t_uchar * const errname,
                    struct revc_revtrav_state * state,
                    t_uchar * root_blob,
                    t_uchar * root_metadata)
{
  if (!state->eot)
    {
      do
        {
          lim_free (0, REVC_REVTRAV_PATH (state));
          lim_free (0, REVC_REVTRAV_BLOB (state));
          lim_free (0, REVC_REVTRAV_METADATA (state));

          if ((state->tos + 1) == state->dir_stack_size)
            {
              lim_free (0, REVC_REVTRAV_DIR (state));
              --state->dir_stack_size;
            }
        }
      while (state->tos--);
    }

  lim_free (0, state->path_stack);
  lim_free (0, state->blob_stack);
  lim_free (0, state->metadata_stack);
  lim_free (0, state->dir_stack);
  lim_free (0, state->dir_size_stack);
  lim_free (0, state->dir_pos_stack);
  
  mem_set0 ((t_uchar *)state, sizeof (*state));
}



/* arch-tag: Tom Lord Tue Jun 28 15:26:39 2005 (api/end-revtrav.c)
 */
