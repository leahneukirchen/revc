/* write.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/write.h"

int
revc_write (const t_uchar * const errname,
            const t_uchar * const buffer,
            size_t amt)
{
  safe_write (1, buffer, amt);
  return 0;
}



/* arch-tag: Tom Lord Thu Jun 23 11:53:21 2005 (write.c)
 */
