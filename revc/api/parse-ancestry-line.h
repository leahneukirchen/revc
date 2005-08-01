/* parse-ancestry-line.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__PARSE_ANCESTRY_LINE_H
#define INCLUDE__API__PARSE_ANCESTRY_LINE_H


#include "hackerlab/machine/types.h"


/* automatically generated __STDC__ prototypes */
extern int revc_parse_ancestry_line (const t_uchar * const errname,
                                     size_t * const ancestor_number_ret,
                                     size_t * const merge_number_ret,
                                     t_uchar ** const revision_ret,
                                     t_uchar * line);
#endif  /* INCLUDE__API__PARSE_ANCESTRY_LINE_H */


/* arch-tag: Tom Lord Sun Jun 26 15:51:55 2005 (parse-ancestry-line.h)
 */
