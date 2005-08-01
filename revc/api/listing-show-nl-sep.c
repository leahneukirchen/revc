/* listing-show-nl-sep.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/api/listing-show-sep.h"
#include "revc/api/listing-show-nl-sep.h"

int
revc_listing_show_nl_sep (const t_uchar * const errname,
                          const t_uchar * const path,
                          size_t const path_size,
                          const t_uchar * const cwd_path_from_root,
                          void * ign)
{
  return revc__listing_show_sep (errname, path, path_size, 1 + str_length (cwd_path_from_root), "\n");
}



/* arch-tag: Tom Lord Thu Jun 23 11:52:28 2005 (librevc/listing-show-nl-sep.c)
 */
