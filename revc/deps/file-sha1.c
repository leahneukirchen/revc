/* file-sha1.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "libsha1/sha1.h"
#include "revc/deps/file-sha1.h"

int
revc_file_sha1 (const t_uchar * const errname,
                t_uchar * const blob_addr_ret,
                const t_uchar * const path,
                size_t const size)
{
  int fd;
  int answer = -1;
  struct sha1_ctx ctx;

  sha1_init_ctx (&ctx);

  fd = safe_open (path, O_RDONLY, 0);
  while (1)
    {
      t_uchar buffer[8192];
      size_t amt;

      amt = safe_read_retry (fd, buffer, sizeof (buffer));
      if (amt)
        sha1_process_bytes (&ctx, buffer, amt);
      else
        {
          t_uchar sha1[20];
          sha1_finish_ctx (sha1, &ctx);
          sha1_to_hex (blob_addr_ret, sha1);
          answer = 0;
          break;
        }
    }
  safe_close (fd);

  return answer;
}
                



/* arch-tag: Tom Lord Thu Jun 30 15:41:55 2005 (deps/file-sha1.c)
 */
