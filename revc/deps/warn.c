/* warn.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/warn.h"

void
revc_warn (const t_uchar * const errname,
            const char * fmt,
            ...)
{
  va_list ap;

  safe_printfmt (2, "(warning) %s: ", errname);

  va_start (ap, fmt);
  safe_printfmt_va_list (2, fmt, ap);
  va_end (ap);
}






/* arch-tag: Tom Lord Sat Jul  2 12:08:16 2005 (deps/warn.c)
 */
