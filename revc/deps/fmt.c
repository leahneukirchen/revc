/* fmt.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/fmt.h"

void
revc_fmt (const t_uchar * const errname,
          const char * fmt,
          ...)
{
  va_list ap;

  va_start (ap, fmt);
  safe_printfmt_va_list (1, fmt, ap);
  va_end (ap);
}






/* arch-tag: Tom Lord Thu Jun 30 11:23:14 2005 (deps/fmt.c)
 */
