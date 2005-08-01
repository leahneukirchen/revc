/* zip-string.c: 
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
#include "revc/api/zip-string.h"



int
revc_zip_string (const t_uchar * const errname,
                 t_uchar ** const zip_ret,
                 size_t * const zip_size_ret,
                 const t_uchar * const str,
                 size_t const size)
{
  int answer = -69;
  z_stream stream;
  t_uchar * zip = 0;
  size_t zip_size = 0;
  int s;

  /* dangle: i'm sure i'm at least not using zlib optimally, and not
   * sure i'm even using it correctly.
   */

  mem_set0 ((t_uchar *)&stream, sizeof (stream));
  if (Z_OK != deflateInit (&stream, Z_BEST_COMPRESSION))
    {
    zlib_error:
      revc_error (errname, "zlib error\n");
    bail:
      answer = -1;
    leave:
      lim_free (0, zip);
      return answer;
    }

  stream.next_in = (t_uchar *)str; /* zlib is missing a const */
  stream.avail_in = size;

  zip = lim_malloc (0, size + 1024);
  stream.next_out = zip;
  stream.avail_out = size + 1024;

  s = deflate (&stream, Z_FINISH);

  if ((s != Z_STREAM_END) && (s != Z_OK))
    goto zlib_error;

  zip = lim_realloc (0, zip, (size + 1024) - stream.avail_out);
  zip_size = (size + 1024) - stream.avail_out;

  if (Z_OK != deflateEnd(&stream))
    goto bail;

  if (zip_ret)
    *zip_ret = zip;
  else
    lim_free (0, zip);
  zip = 0;

  if (zip_size_ret)
    *zip_size_ret = zip_size;
     
  answer = 0;
  goto leave;
}






/* arch-tag: Tom Lord Tue Jun 21 13:23:50 2005 (zip-string.c)
 */
