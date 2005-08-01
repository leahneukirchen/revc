/* put-from-str.c: 
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
#include "revc/deps/put-from-str.h"

int
revc_put_from_str (const t_uchar * const errname,
                   const t_uchar * const file,
                   const t_uchar * const str,
                   size_t const size,
                   const t_uchar * const optional_metadata)
{
  int fd;
  const t_uchar * meta = (optional_metadata ? optional_metadata : (const t_uchar *)REVC_DEFAULT_FILE_METADATA);
  mode_t mode;

  if (revc__metadata_to_perms (errname, &mode, meta))
    return -1;

  fd = safe_open (file, O_CREAT | O_WRONLY, mode);
  safe_write_retry (fd, str, size);
  safe_close (fd);

  return 0;
}




/* arch-tag: Tom Lord Thu Jun 23 12:58:40 2005 (put-from-str.c)
 */
