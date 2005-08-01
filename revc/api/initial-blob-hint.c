/* initial-blob-hint.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "revc/api/constants.h"
#include "revc/api/nil-blob.h"
#include "revc/api/initial-prereqs-blob.h"
#include "revc/api/initial-ancestry-blob.h"
#include "revc/api/add-blob-to-hint-set.h"
#include "revc/api/initial-blob-hint.h"

int
revc_initial_blob_hint (const t_uchar * const errname,
                        t_uchar * set_ret)
{
  const t_uchar * rb = revc_nil_blob (errname);
  const t_uchar * ia = revc_initial_ancestry_blob (errname);

  if (!rb || !ia)
    return -1;

  mem_set0 (set_ret, REVC_BLOB_HINT_SET_SIZE);

  revc_add_blob_to_hint_set (set_ret, rb);
  revc_add_blob_to_hint_set (set_ret, ia);

  return 0;
}



/* arch-tag: Tom Lord Thu Jul  7 11:35:38 2005 (initial-blob-hint.c)
 */
