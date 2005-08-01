/* begin-get-txn.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/deps/error.h"
#include "revc/api/constants.h"
#include "revc/archives/begin-get-txn.h"

int
revc_begin_get_txn (const t_uchar * const errname,
                    struct revc_archives * const archs,
                    const t_uchar * const revname,
                    const t_uchar * const prereqs,
                    size_t prereqs_size)
{
  int answer;

  if (archs->_first_free_get < 0)
    {
      revc_error (errname, "maximum number of simultaneous gets exceeded\n");
      return -1;
    }

  answer = archs->_first_free_get;

  archs->_gets[answer].prereqs = (t_uchar *)lim_malloc (0, prereqs_size);
  mem_move (archs->_gets[answer].prereqs, prereqs, prereqs_size);
  archs->_gets[answer].prereqs_size = prereqs_size;
  archs->_gets[answer].next = 0;
  archs->_first_free_get = archs->_gets[answer].next;
  return answer;
}



/* arch-tag: Tom Lord Sat Jul  2 17:28:13 2005 (begin-get-txn.c)
 */
