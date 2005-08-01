/* restore-dir.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/restore-dir.h"

int
revc_restore_dir (const t_uchar * const errname,
                  t_revc_cwd_buffer * const buffer)
{
  if (*buffer >= 0)
    {
      safe_fchdir (*buffer);
      safe_close (*buffer);
    }
  *buffer = REVC_CWD_BUFFER_INIT;
  return 0;
}



/* arch-tag: Tom Lord Thu Jun 23 12:23:16 2005 (deps/restore-dir.c)
 */
