/* begin-revision-traversal.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__BEGIN_REVISION_TRAVERSAL_H
#define INCLUDE__API__BEGIN_REVISION_TRAVERSAL_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"
#include "revc/api/traversal.h"

struct revc_revision_traversal
{
  struct revc_traversal trav;

  struct revc_archives * archs;
  t_uchar * revision;
  t_uchar * prereqs;
  size_t prereqs_size;

  struct revc_revision_traversal_stack
    {
      struct revc_revision_traversal_stack * prev;
      t_uchar * dir;
      size_t dir_size;
      size_t dir_pos;
    } * stack;
};



/* automatically generated __STDC__ prototypes */
extern int revc_begin_revision_traversal (const t_uchar * const errname,
                                          struct revc_traversal ** trav,
                                          struct revc_archives * archs,
                                          t_uchar * revision);
#endif  /* INCLUDE__API__BEGIN_REVISION_TRAVERSAL_H */


/* arch-tag: Tom Lord Sat Jul  2 18:21:26 2005 (begin-revision-traversal.h)
 */
