/* listing-show-sep.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "revc/deps/write.h"
#include "revc/api/listing-show-sep.h"

int
revc__listing_show_sep (const t_uchar * const errname,
                        const t_uchar * const path,
                        size_t const path_size,
                        size_t ign_amt,
                        t_uchar * sep)
{
  invariant (ign_amt < path_size);
  revc_write (errname, path + ign_amt, path_size - ign_amt);
  revc_write (errname, sep, 1);
  return 1;
}



/* arch-tag: Tom Lord Thu Jun 23 11:49:23 2005 (listing-show-sep.c)
 */
