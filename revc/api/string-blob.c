/* string-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/fmt/cvt.h"
#include "libsha1/sha1.h"
#include "revc/deps/error.h"
#include "revc/api/constants.h"
#include "revc/api/string-blob.h"

int
revc_string_blob (const t_uchar * const errname,
                  t_uchar * const blob_ret,
                  const t_uchar * const data,
                  size_t size)
{
  t_uchar sha1[20];
  t_uchar sha1x[40];
  t_uchar size_str[sizeof (t_ulong) * 4 + 1];
  size_t size_str_len;

  sha1_buffer (sha1, data, size);
  sha1_to_hex (sha1x, sha1);

  size_str_len = cvt_ulong_to_decimal (size_str, (t_ulong)size);

  if ((40 + 1 + size_str_len) > REVC_MAX_BLOB_ADDR_SIZE)
    {
      revc_error (errname, "blob address too long(?!?)\n");
      return -1;
    }

  if (blob_ret)
    {
      mem_move (blob_ret, sha1x, 40);
      blob_ret[40] = '.';
      mem_move (blob_ret + 41, size_str, size_str_len + 1);
    }

  return 0;
}



/* arch-tag: Tom Lord Sat Jul  2 10:38:10 2005 (string-blob.c)
 */
