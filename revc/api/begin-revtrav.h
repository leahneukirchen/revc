/* begin-revtrav.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__BEGIN_REVTRAV_H
#define INCLUDE__API__BEGIN_REVTRAV_H


#include "hackerlab/machine/types.h"
#include "revc/api/constants.h"

struct revc_revtrav_state
{
  int eot;

  t_uchar ** path_stack;
  t_uchar ** blob_stack;
  t_uchar ** metadata_stack;
  size_t tos;

  t_uchar ** dir_stack;
  size_t * dir_size_stack;
  size_t * dir_pos_stack;
  size_t dir_stack_size;
};

#define REVC_REVTRAV_PATH(STATE) ((STATE)->path_stack[(STATE)->tos])
#define REVC_REVTRAV_BLOB(STATE) ((STATE)->blob_stack[(STATE)->tos])
#define REVC_REVTRAV_METADATA(STATE) ((STATE)->metadata_stack[(STATE)->tos])

#define REVC_REVTRAV_DIR(STATE) ((STATE)->dir_stack[(STATE)->tos])
#define REVC_REVTRAV_DIR_SIZE(STATE) ((STATE)->dir_size_stack[(STATE)->tos])
#define REVC_REVTRAV_DIR_POS(STATE) ((STATE)->dir_pos_stack[(STATE)->tos])

#define REVC_REVTRAV_DIR_ENTRY(STATE) ((STATE)->dir_stack[(STATE)->tos] + (STATE)->dir_pos_stack[(STATE)->tos])


/* automatically generated __STDC__ prototypes */
extern int revc_begin_revtrav (const t_uchar * const errname,
                               struct revc_revtrav_state * state,
                               t_uchar * root_blob,
                               t_uchar * root_metadata);
#endif  /* INCLUDE__API__BEGIN_REVTRAV_H */


/* arch-tag: Tom Lord Tue Jun 28 12:42:36 2005 (begin-revtrav.h)
 */
