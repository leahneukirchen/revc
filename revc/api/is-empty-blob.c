/* is-empty-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/api/nil-blob.h"
#include "revc/api/is-empty-blob.h"

int
revc_is_empty_blob (const t_uchar * const errname,
                    const t_uchar * const blob)
{
  const t_uchar * n = revc_nil_blob (errname);

  return !str_cmp (n, blob);
}



/* arch-tag: Tom Lord Sat Jul  2 19:30:15 2005 (is-empty-blob.c)
 */
