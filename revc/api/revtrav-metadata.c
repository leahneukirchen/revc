/* revtrav-metadata.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/api/revtrav-metadata.h"

int
revc_revtrav_metadata (const t_uchar * const errname,
                       t_uchar * metadata_addr_ret,
                       struct revc_revtrav_state * state)
{
  if (metadata_addr_ret)
    {
      str_cpy (metadata_addr_ret, REVC_REVTRAV_METADATA (state));
    }

  return 0;
}




/* arch-tag: Tom Lord Tue Jun 28 13:09:11 2005 (api/revtrav-metadata.c)
 */
