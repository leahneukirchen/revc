/* close-archives.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/api/constants.h"
#include "revc/archives/close-archives.h"

int
revc_close_archives (const t_uchar * const errname,
                     struct revc_archives * archs)
{
  mem_set0 ((t_uchar *)archs, sizeof (*archs));
  lim_free (0, archs->_archive_dir);
  return 0;
}




/* arch-tag: Tom Lord Sun Jun 26 17:17:17 2005 (close-archives.c)
 */
