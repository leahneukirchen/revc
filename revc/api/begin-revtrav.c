/* begin-revtrav.c: 
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
#include "revc/archives/get-blob.h"
#include "revc/api/begin-revtrav.h"

int
revc_begin_revtrav (const t_uchar * const errname,
                    struct revc_revtrav_state * state,
                    t_uchar * root_blob,
                    t_uchar * root_metadata)
{
  mem_set0 ((t_uchar *)state, sizeof (*state));

  state->path_stack = (t_uchar **)lim_malloc (0, sizeof (t_uchar *));
  state->blob_stack = (t_uchar **)lim_malloc (0, sizeof (t_uchar *));
  state->metadata_stack = (t_uchar **)lim_malloc (0, sizeof (t_uchar *));
  state->dir_stack = (t_uchar **)lim_malloc (0, sizeof (t_uchar *));
  state->dir_size_stack = (size_t *)lim_malloc (0, sizeof (size_t));
  state->dir_pos_stack = (size_t *)lim_malloc (0, sizeof (size_t));

  REVC_REVTRAV_PATH(state) = str_save (0, ".");
  REVC_REVTRAV_BLOB(state) = str_save (0, root_blob);
  REVC_REVTRAV_METADATA(state) = str_save (0, root_metadata);

  return 0;
}



/* arch-tag: Tom Lord Tue Jun 28 12:42:39 2005 (begin-revtrav.c)
 */
