/* mkdir.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/metadata-to-perms.h"
#include "revc/api/constants.h"
#include "revc/deps/mkdir.h"

int
revc_mkdir (const t_uchar * const errname,
            const t_uchar * const dir,
            const t_uchar * const optional_metadata)
{
  const t_uchar * meta = (optional_metadata ? optional_metadata : (const t_uchar *)REVC_DEFAULT_DIR_METADATA);
  mode_t mode;

  if (revc__metadata_to_perms (errname, &mode, meta))
    return -1;
  
  safe_mkdir (dir, mode);
  return 0;
}




/* arch-tag: Tom Lord Thu Jun 23 12:53:10 2005 (mkdir.c)
 */
