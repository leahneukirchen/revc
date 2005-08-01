/* revtrav-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/api/revtrav-blob.h"

int
revc_revtrav_blob (const t_uchar * const errname,
                   t_uchar * blob_addr_ret,
                   struct revc_revtrav_state * state)
{
  if (blob_addr_ret)
    {
      str_cpy (blob_addr_ret, REVC_REVTRAV_BLOB (state));
    }

  return 0;
}




/* arch-tag: Tom Lord Tue Jun 28 13:00:51 2005 (revtrav-blob.c)
 */
