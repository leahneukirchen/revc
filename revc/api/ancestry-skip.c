/* ancestry-skip.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/api/list-skip.h"
#include "revc/api/ancestry-skip.h"

void
revc_ancestry_skip (t_uchar ** const pos,
                    size_t * const remain)
{
  revc_list_skip (pos, remain);
}




/* arch-tag: Tom Lord Thu Jul  7 12:11:07 2005 (ancestry-skip.c)
 */
