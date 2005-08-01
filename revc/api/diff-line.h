/* diff-line.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__DIFF_LINE_H
#define INCLUDE__API__DIFF_LINE_H


#include "hackerlab/machine/types.h"

struct revc_diff_line
{
  t_ulong hash;
  size_t start;
  size_t end;
};


/* automatically generated __STDC__ prototypes */
#endif  /* INCLUDE__API__DIFF_LINE_H */


/* arch-tag: Tom Lord Tue Jul 12 13:52:56 2005 (diff-line.h)
 */
