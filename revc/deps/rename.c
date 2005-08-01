/* rename.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/rename.h"

int
revc_rename (const t_uchar * const errname,
             const t_uchar * const from,
             const t_uchar * const to)
{
  safe_rename (from, to);
  return 0;
}



/* arch-tag: Tom Lord Thu Jun 23 14:15:33 2005 (deps/rename.c)
 */
