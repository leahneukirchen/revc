/* usage.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/usage.h"


void
revc_usage (const t_uchar * const errname,
            const t_uchar * const usage)
{
  safe_printfmt (2, "%s: usage -- %s %s\n", errname, errname, usage);
}





/* arch-tag: Tom Lord Thu Jun 23 10:45:00 2005 (usage.c)
 */
