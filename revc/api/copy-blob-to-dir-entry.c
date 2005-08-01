/* copy-blob-to-dir-entry.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include "revc/api/constants.h"
#include "revc/api/copy-blob-to-dir-entry.h"

void
revc_copy_blob_to_dir_entry (t_uchar * const entry,
                             t_uchar * const blob)
{
  size_t x;
  t_uchar * e = REVC_DIR_ENTRY_BLOB_ADDR (entry);
  t_uchar * b = blob;

  for (x = 0; ((x < REVC_MAX_BLOB_ADDR_SIZE) && *b); ++x)
    *(e++) = *(b++);

  while ((x++) < REVC_MAX_BLOB_ADDR_SIZE)
    *(e++) = 0;
}



/* arch-tag: Tom Lord Sat Jul  2 10:27:56 2005 (copy-blob-to-dir-entry.c)
 */
