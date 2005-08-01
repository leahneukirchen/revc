/* revtrav-next.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__REVTRAV_NEXT_H
#define INCLUDE__API__REVTRAV_NEXT_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"
#include "revc/api/begin-revtrav.h"


/* automatically generated __STDC__ prototypes */
extern int revc_revtrav_next (const t_uchar * const errname,
                              struct revc_revtrav_state * state,
                              struct revc_archives * const archs,
                              const t_uchar * const revision,
                              t_uchar * const prereqs,
                              size_t const prereqs_size,
                              int skip_subdir);
#endif  /* INCLUDE__API__REVTRAV_NEXT_H */


/* arch-tag: Tom Lord Tue Jun 28 13:19:51 2005 (api/revtrav-next.h)
 */
