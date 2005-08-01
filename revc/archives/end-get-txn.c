/* end-get-txn.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/mem/mem.h"
#include "revc/archives/end-get-txn.h"

int
revc_end_get_txn (const t_uchar * const errname,
                  struct revc_archives * const archs,
                  int get_n)
{
  lim_free (0, archs->_gets[get_n].prereqs);
  mem_set0 ((t_uchar *)&archs->_gets[get_n], sizeof (archs->_gets[get_n]));
  archs->_gets[get_n].next = archs->_first_free_get;
  archs->_first_free_get = get_n;
  return 0;
}



/* arch-tag: Tom Lord Sat Jul  2 17:53:23 2005 (end-get-txn.c)
 */
