/* nil-blob-serialization.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/api/constants.h"
#include "revc/api/string-blob.h"
#include "revc/api/nil-blob-serialization.h"



int
revc_nil_blob_serialization (const t_uchar * const errname,
                             t_uchar ** const serial_ret,
                             size_t * len_ret)
{
  t_uchar blob_addr[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar * answer = 0;

  if (revc_string_blob (errname, blob_addr, "", 0))
    return -1;

  answer = (t_uchar *)lim_malloc (0, 46);
  mem_move (answer, blob_addr, 43);
  mem_move (answer + 43, ".0", 3);

  *serial_ret = answer;
  *len_ret = 46;

  return 0;
}






/* arch-tag: Tom Lord Mon Jun 27 09:45:34 2005 (nil-blob-serialization.c)
 */
