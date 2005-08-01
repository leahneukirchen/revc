/* initial-ancestry.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/mem/mem.h"
#include "revc/api/initial-ancestry.h"

#define it "0.0.nil\0"

int
revc_initial_ancestry (const t_uchar * const errname,
                       t_uchar ** const ancestry_ret,
                       size_t * const size_ret)
{
  t_uchar * answer = lim_malloc (0, sizeof (it) - 1);

  mem_move (answer, it, sizeof (it) - 1);

  *ancestry_ret = answer;
  *size_ret = sizeof (it) - 1;

  return 0;
}
                       



/* arch-tag: Tom Lord Mon Jun 27 09:33:35 2005 (initial-ancestry.c)
 */
