/* traversal-cmp.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/machine/types.h"
#include "revc/api/traversal-cmp.h"

int
revc_traversal_cmp (const t_uchar * const a_str,
                    const t_uchar * const b_str)
{
  const t_uchar * a = a_str;
  const t_uchar * b = b_str;

  if (!a_str && !b_str)
    return 0;
  else if (!a_str)
    return -1;
  else if (!b_str)
    return 1;

  while (*a && *b && (*a == *b))
    {
      ++a;
      ++b;
    }

  if (!*a && !*b)
    return 0;

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




/* arch-tag: Tom Lord Thu Jun 23 14:19:33 2005 (api/traversal-cmp.c)
 */
