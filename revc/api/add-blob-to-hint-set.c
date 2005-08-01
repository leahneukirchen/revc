/* add-blob-to-hint-set.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/api/blob-hint.h"
#include "revc/api/add-blob-to-hint-set.h"

void
revc_add_blob_to_hint_set (t_uchar * const set,
                           const t_uchar * const blob)
{
  t_uint a_byte, a_mask;
  t_uint b_byte, b_mask;

  revc_blob_hint (&a_byte, &a_mask, &b_byte, &b_mask, blob);

  set[a_byte] |= a_mask;
  set[b_byte] |= b_mask;
}



/* arch-tag: Tom Lord Sun Jul  3 07:29:08 2005 (add-blob-to-hint-set.c)
 */
