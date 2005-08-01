/* str-to-lines.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__STR_TO_LINES_H
#define INCLUDE__API__STR_TO_LINES_H


#include "hackerlab/machine/types.h"
#include "revc/api/diff-line.h"


/* automatically generated __STDC__ prototypes */
extern int revc_str_to_lines (const t_uchar * const errname,
                              struct revc_diff_line ** ret,
                              const t_uchar * const str,
                              size_t const size);
#endif  /* INCLUDE__API__STR_TO_LINES_H */


/* arch-tag: Tom Lord Tue Jul 12 13:57:29 2005 (str-to-lines.h)
 */
