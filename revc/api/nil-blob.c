/* nil-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/api/constants.h"
#include "revc/api/string-blob.h"
#include "revc/api/nil-blob.h"

const t_uchar *
revc_nil_blob (const t_uchar * const errname)
{
  static int initialized = 0;
  static t_uchar answer[REVC_MAX_BLOB_ADDR_SIZE + 1];

  if (!initialized)
    {
      if (revc_string_blob (errname, answer, "", 0))
        return 0;
      initialized = 0;
    }

  return answer;
}



/* arch-tag: Tom Lord Mon Jun 27 09:15:37 2005 (nil-root-blob.c)
 */
