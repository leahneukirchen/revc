/* unzip-string.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

/* includes */

#include "hackerlab/os/zlib.h"
#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/mem/mem.h"
#include "revc/deps/error.h"
#include "revc/api/unzip-string.h"



int
revc_unzip_string (const t_uchar * const errname,
                 t_uchar ** const unzip_ret,
                 size_t * const unzip_size_ret,
                 const t_uchar * const str,
                 size_t const size)
{
  z_stream stream;
  t_uchar buffer[8192];
  t_uchar * answer = 0;
  size_t answer_size = 0;

  /* dangle: i'm sure i'm at least not using zlib optimally, and not
   * sure i'm even using it correctly.
   */

  mem_set0 ((t_uchar *)&stream, sizeof (stream));
  if (Z_OK != inflateInit (&stream))
    {
    bail:
      revc_error (errname, "zlib error\n");
      return -1;
    }

  stream.next_in = (t_uchar *)str; /* zlib is missing a const */
  stream.avail_in = size;

  while (stream.avail_in)
    {
      int x;
      size_t avail_out;

      stream.next_out = buffer;
      stream.avail_out = sizeof (buffer);

      x = inflate(&stream, Z_SYNC_FLUSH);
      if ((Z_OK != x) && (Z_STREAM_END != x))
        goto bail;

      avail_out = sizeof (buffer) - stream.avail_out;
      answer = lim_realloc (0, answer, answer_size + avail_out);
      mem_move (answer + answer_size, buffer, avail_out);
      answer_size += avail_out;
    }

  if (Z_OK != inflateEnd(&stream))
    goto bail;

  if (unzip_ret)
    *unzip_ret = answer;
  else
    lim_free (0, answer);
  answer = 0;

  if (unzip_size_ret)
    *unzip_size_ret = answer_size;
     
  return 0;
}






/* arch-tag: Tom Lord Sun Jul  3 08:07:55 2005 (api/unzip-string.c)
 */
