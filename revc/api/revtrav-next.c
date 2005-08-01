/* revtrav-next.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/error.h"
#include "revc/archives/get-blob.h"
#include "revc/api/revtrav-next.h"

int
revc_revtrav_next (const t_uchar * const errname,
                   struct revc_revtrav_state * state,
                   struct revc_archives * const archs,
                   const t_uchar * const revision,
                   t_uchar * const prereqs,
                   size_t const prereqs_size,
                   int skip_subdir)
{
  /* next is the next node in a pre-order traversal.
   */

  if (state->eot)
    return 0;

  if (skip_subdir)
    goto treat_as_leaf_node;

 retry:

  if (REVC_REVTRAV_METADATA(state)[0] != 'd')
    {
    treat_as_leaf_node:
      /* leaf node! */
      if (state->tos == 0)
        {
          state->eot = 1;
          return 0;
        }
      else
        {
        pop_retry:
          if ((state->tos + 1) == state->dir_stack_size)
            {
              lim_free (0, REVC_REVTRAV_DIR(state));
              --state->dir_stack_size;
            }
          lim_free (0, REVC_REVTRAV_PATH (state));
          lim_free (0, REVC_REVTRAV_BLOB (state));
          lim_free (0, REVC_REVTRAV_METADATA (state));
          if (state->tos)
            {
              --state->tos;
              /* REVC_REVTRAV_DIR_POS(state) += REVC_DIR_ENTRY_SIZE; */
              goto retry;
            }
          else
            {
              state->eot = 1;
              return 0;
            }
        }
    }
  else
    {
      /* directory! */

      if ((state->tos + 1) > state->dir_stack_size)
        {
          /* not descended into yet */

          state->dir_stack = (t_uchar **)lim_realloc (0, state->dir_stack, (state->tos + 1) * sizeof (t_uchar *));
          state->dir_size_stack = (size_t *)lim_realloc (0, state->dir_size_stack, (state->tos + 1) * sizeof (size_t));
          state->dir_pos_stack = (size_t *)lim_realloc (0, state->dir_pos_stack, (state->tos + 1) * sizeof (size_t));
          state->dir_stack_size = state->tos + 1;

          if (0 > revc_get_blob (errname, 
                                 &REVC_REVTRAV_DIR (state),
                                 &REVC_REVTRAV_DIR_SIZE (state),
                                 archs,
                                 revision,
                                 prereqs, prereqs_size,
                                 REVC_REVTRAV_BLOB (state),
                                 0))
            return -1;

          REVC_REVTRAV_DIR_POS (state) = 0;

          if (REVC_REVTRAV_DIR_SIZE (state) == 0)
            goto pop_retry;
        }
      else
        {
          /* somewhere in the middle of (or at the end of) this dir */
          if ((REVC_REVTRAV_DIR_POS (state) + REVC_DIR_ENTRY_SIZE) == REVC_REVTRAV_DIR_SIZE (state))
            goto pop_retry;
      
          if ((REVC_REVTRAV_DIR_POS (state) + REVC_DIR_ENTRY_SIZE) > REVC_REVTRAV_DIR_SIZE (state))
            {
              revc_error (errname,
                          "corrupt directory blob (for %s, %s)\n",
                          REVC_REVTRAV_BLOB (state),
                          REVC_REVTRAV_PATH (state));
              return -1;
            }

          REVC_REVTRAV_DIR_POS (state) += REVC_DIR_ENTRY_SIZE;          
        }

      {
        t_uchar * const blob = str_save_n (0,
                                           REVC_DIR_ENTRY_BLOB_ADDR (REVC_REVTRAV_DIR_ENTRY (state)),
                                           REVC_MAX_BLOB_ADDR_SIZE); 
        t_uchar * const meta = str_save_n (0,
                                           REVC_DIR_ENTRY_METADATA (REVC_REVTRAV_DIR_ENTRY (state)),
                                           REVC_MAX_METADATA_SIZE);
        t_uchar * const path = file_name_in_vicinity (0,
                                                      REVC_REVTRAV_PATH (state),
                                                      REVC_DIR_ENTRY_FNAME (REVC_REVTRAV_DIR_ENTRY (state)));

        state->path_stack = lim_realloc (0, state->path_stack, (state->tos + 2) * sizeof (t_uchar *));
        state->blob_stack = lim_realloc (0, state->blob_stack, (state->tos + 2) * sizeof (t_uchar *));
        state->metadata_stack = lim_realloc (0, state->metadata_stack, (state->tos + 2) * sizeof (t_uchar *));

        ++state->tos;

        REVC_REVTRAV_PATH (state) = path;
        REVC_REVTRAV_BLOB (state) = blob;
        REVC_REVTRAV_METADATA (state) = meta;
        return 1;
      }
    }
}





/* arch-tag: Tom Lord Tue Jun 28 13:19:35 2005 (revtrav-next.c)
 */
