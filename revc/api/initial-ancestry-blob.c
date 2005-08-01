/* initial-ancestry-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "revc/api/constants.h"
#include "revc/api/string-blob.h"
#include "revc/api/initial-ancestry.h"
#include "revc/api/initial-ancestry-blob.h"


const t_uchar *
revc_initial_ancestry_blob (const t_uchar * const errname)
{
  static int initialized = 0;
  static t_uchar blob[REVC_MAX_BLOB_ADDR_SIZE + 1];

  if (!initialized)
    {
      t_uchar * ancestry = 0;
      size_t size;
      int status;

      if (revc_initial_ancestry (errname, &ancestry, &size))
        return 0;

      status = revc_string_blob (errname, blob, ancestry, size);

      lim_free (0, ancestry);

      if (status < 0)
        return 0;

      initialized = 1;
    }

  return blob;
}




/* arch-tag: Tom Lord Sun Jul  3 12:10:03 2005 (api/initial-ancestry-blob.c)
 */
