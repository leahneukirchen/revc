/* qsort-traversal-cmp.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/machine/types.h"
#include "revc/api/qsort-traversal-cmp.h"

int
revc_qsort_traversal_cmp (void * va, void * vb, void * ign)
{
  t_uchar * a = *(t_uchar **)va;
  t_uchar * b = *(t_uchar **)vb;

  while (*a && *b && (*a == *b))
    {
      ++a;
      ++b;
    }

  if (!*a)
    return -1;

  if (!*b)
    return 1;

  if (*a == '/')
    return -1;

  if (*b == '/')
    return 1;

  if (*a < *b)
    return -1;

  return 1;
}




/* arch-tag: Tom Lord Thu Jun 23 13:54:57 2005 (qsort-traversal-cmp.c)
 */
