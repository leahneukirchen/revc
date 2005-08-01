/* file-is-dir.c:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe-vu-utils.h"
#include "revc/deps/file-is-dir.h"

int
revc_file_is_dir (const t_uchar * const errname,
                            const t_uchar * const path)
{
  return safe_file_is_directory (path);
}



/* arch-tag: Tom Lord Thu Jun 23 12:45:27 2005 (deps/file-is-dir-following.c)
 */
