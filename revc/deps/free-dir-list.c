/* free-dir-list.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/arrays/ar.h"
#include "revc/deps/free-dir-list.h"

void
revc_free_dir_list (t_uchar *** const list)
{
  size_t x;

  for (x = 0; x < ar_size ((void *)*list, 0, sizeof (t_uchar *)); ++x)
    {
      lim_free (0, (*list)[x]);
    }

  ar_free ((void **)list, 0);
}



/* arch-tag: Tom Lord Thu Jul 14 12:26:35 2005 (free-dir-list.c)
 */
