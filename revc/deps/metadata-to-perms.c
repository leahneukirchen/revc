/* perms-from-metadata.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

/* includes */

#include "hackerlab/os/sys/stat.h"
#include "revc/api/constants.h"
#include "revc/deps/metadata-to-perms.h"



int
revc__metadata_to_perms (const t_uchar * const errname,
                         mode_t * perms_ret,
                         const t_uchar * const meta)
{
  const t_uchar * pos = meta + 4;
  mode_t answer = 0;

  if (*(pos++) != '-')
    answer |= S_IRUSR;
  if (*(pos++) != '-')
    answer |= S_IWUSR;
  if (*(pos++) != '-')
    answer |= S_IXUSR;

  if (*(pos++) != '-')
    answer |= S_IRGRP;
  if (*(pos++) != '-')
    answer |= S_IWGRP;
  if (*(pos++) != '-')
    answer |= S_IXGRP;

  if (*(pos++) != '-')
    answer |= S_IROTH;
  if (*(pos++) != '-')
    answer |= S_IWOTH;
  if (*(pos++) != '-')
    answer |= S_IXOTH;

  if (perms_ret)
    *perms_ret = answer;

  return 0;
}





/* arch-tag: Tom Lord Tue Jun 21 16:06:31 2005 (librevc/perms-from-metadata.c)
 */
