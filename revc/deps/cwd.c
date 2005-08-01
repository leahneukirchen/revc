/* cwd.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/fs/cwd.h"
#include "revc/deps/cwd.h"

t_uchar *
revc_cwd (const t_uchar * const errname)
{
  return safe_current_working_directory ();
}




/* arch-tag: Tom Lord Thu Jun 23 12:41:02 2005 (cwd.c)
 */
