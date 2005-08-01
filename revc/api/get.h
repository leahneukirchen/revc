/* get.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__GET_H
#define INCLUDE__API__GET_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_get (const t_uchar * const errname,
                     struct revc_archives * archs,
                     const t_uchar * const revision,
                     const t_uchar * const output_dir);
#endif  /* INCLUDE__API__GET_H */


/* arch-tag: Tom Lord Sun Jun 26 17:09:56 2005 (get.h)
 */
