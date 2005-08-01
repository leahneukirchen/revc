/* file-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/fmt/cvt.h"
#include "revc/deps/file-sha1.h"
#include "revc/api/file-blob.h"

int
revc_file_blob (const t_uchar * const errname,
                t_uchar * const blob_addr_ret,
                const t_uchar * const path,
                size_t size)
{
  if (revc_file_sha1 (errname, blob_addr_ret, path, size))
    return -1;

  blob_addr_ret[40] = '.';

  cvt_ulong_to_decimal (blob_addr_ret + 41, (t_ulong)size);

  return 0;
}
                



/* arch-tag: Tom Lord Thu Jun 30 12:03:12 2005 (file-blob.c)
 */
