/* save-dir.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/save-dir.h"

int
revc_save_dir (const t_uchar * const errname,
               t_revc_cwd_buffer * const buffer)
{
  *buffer = safe_open (".", O_RDONLY, 0);
  return 0;
}



/* arch-tag: Tom Lord Thu Jun 23 12:20:21 2005 (deps/save-dir.c)
 */
