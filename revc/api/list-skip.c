/* list-skip.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/char/str.h"
#include "revc/api/list-skip.h"
#include "revc/api/check-list.h"



void
revc_list_skip (t_uchar ** const inv,
                size_t * const len)
{
  invariant (!revc_check_list ((t_uchar *)"revc", *inv, *len));

  if (*len)
    {
      size_t const inc = str_length (*inv) + 1;

      *inv += inc;
      *len -= inc;
    }
}





/* arch-tag: Tom Lord Thu Jun 16 21:39:14 2005 (inventory-skip.c)
 */
