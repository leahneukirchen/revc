/* blob-addr-skip.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/char-class.h"
#include "revc/deps/error.h"
#include "revc/api/blob-addr-skip.h"

int
revc_blob_addr_skip (const t_uchar * const errname,
                     const t_uchar ** const pos)
{
  size_t l;

  for (l = 0; l < 40; ++l)
    {
      if (!char_is_xdigit (**pos))
        {
        error:
          revc_error (errname, "invalid blob address");
          return -1;
        }
      ++(*pos);
    }

  if (*(*pos) != '.')
    goto error;

  ++(*pos);

  if (!char_is_digit (*(*pos)))
    goto error;

  for (l = 0; l < (8 * 4 + 1); ++l)
    {
      if (!*(*pos))
        return 0;
      if (!char_is_digit (*(*pos)))
        goto error;
    }
  goto error;
}



/* arch-tag: Tom Lord Mon Jun 27 09:10:42 2005 (blob-addr-skip.c)
 */
