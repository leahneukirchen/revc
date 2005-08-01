/* revtrav-path.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/api/revtrav-path.h"


int
revc_revtrav_path (const t_uchar * const errname,
                   t_uchar ** const path_ret,
                   struct revc_revtrav_state * state)
{
  if (state->eot)
    {
      if (path_ret)
        *path_ret = 0;
      return 0;
    }
  else
    {
      if (path_ret)
        *path_ret = str_save (0, REVC_REVTRAV_PATH (state));
      return 1;
    }
}




/* arch-tag: Tom Lord Tue Jun 28 13:10:39 2005 (api/revtrav-path.c)
 */
