/* prereqs-skip.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/deps/error.h"
#include "revc/api/constants.h"
#include "revc/api/list-skip.h"
#include "revc/api/prereqs-skip.h"

int
revc_prereqs_skip (const t_uchar * const errname,
                   t_uchar ** const pos,
                   size_t * const remain)
{
  if (!*remain)
    return 0;

  revc_list_skip (pos, remain);

  if (*remain < REVC_BLOB_HINT_SET_SIZE)
    {
      revc_error (errname, "corrupt prereqs list\n");
      return -1;
    }

  *pos += REVC_BLOB_HINT_SET_SIZE;
  *remain -= REVC_BLOB_HINT_SET_SIZE;

  return 0;
}





/* arch-tag: Tom Lord Sun Jul  3 07:58:55 2005 (prereqs-skip.c)
 */
