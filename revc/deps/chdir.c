/* chdir.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/chdir.h"

int
revc_chdir (const t_uchar * const errname,
            const t_uchar * const dir)
{
  safe_chdir (dir);
  return 0;
}



/* arch-tag: Tom Lord Thu Jun 23 12:18:32 2005 (chdir.c)
 */
