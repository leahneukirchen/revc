/* copy-fname-to-dir-entry.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include "revc/api/constants.h"
#include "revc/api/copy-fname-to-dir-entry.h"

void
revc_copy_fname_to_dir_entry (t_uchar * const entry,
                             t_uchar * const fname)
{
  size_t x;
  t_uchar * e = REVC_DIR_ENTRY_FNAME (entry);
  t_uchar * b = fname;

  for (x = 0; ((x < REVC_MAX_FNAME_SIZE) && *b); ++x)
    *(e++) = *(b++);

  while ((x++) < REVC_MAX_FNAME_SIZE)
    *(e++) = 0;
}



/* arch-tag: Tom Lord Sat Jul  2 10:55:41 2005 (api/copy-fname-to-dir-entry.c)
 */
