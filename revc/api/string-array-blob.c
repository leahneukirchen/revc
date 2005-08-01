/* string-array-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/fmt/cvt.h"
#include "hackerlab/char/str.h"
#include "libsha1/sha1.h"
#include "revc/deps/error.h"
#include "revc/api/constants.h"
#include "revc/api/string-array-blob.h"

int
revc_string_array_blob (const t_uchar * const errname,
                        t_uchar * const blob_ret,
                        t_uchar ** const data,
                        size_t const n_entries)
{
  struct sha1_ctx ctx;
  size_t x;
  size_t size;
  t_uchar sha1[20];
  t_uchar sha1x[40];
  t_uchar size_str[sizeof (t_ulong) * 4 + 1];
  size_t size_str_len;

  sha1_init_ctx (&ctx);
  size = 0;
  for (x = 0; x < n_entries; ++x)
    {
      size_t amt = 1 + str_length (data[x]);
      size += amt;
      sha1_process_bytes (&ctx, data[x], amt);
    }
  sha1_finish_ctx (sha1, &ctx);

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



/* arch-tag: Tom Lord Sat Jul  9 11:28:07 2005 (api/string-array-blob.c)
 */
