/* debug.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/debug.h"

void
revc_debug (const t_uchar * const errname,
            const char * fmt,
            ...)
{
  va_list ap;

  safe_printfmt (1, "%s: ", errname);

  va_start (ap, fmt);
  safe_printfmt_va_list (1, fmt, ap);
  va_end (ap);
}






/* arch-tag: Tom Lord Mon Jun 27 11:41:45 2005 (deps/debug.c)
 */
