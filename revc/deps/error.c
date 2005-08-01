/* error.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/error.h"

void
revc_error (const t_uchar * const errname,
            const char * fmt,
            ...)
{
  va_list ap;

  safe_printfmt (2, "%s: ", errname);

  va_start (ap, fmt);
  safe_printfmt_va_list (2, fmt, ap);
  va_end (ap);
}






/* arch-tag: Tom Lord Thu Jun 23 10:52:57 2005 (error.c)
 */
