/* check-list.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/deps/error.h"
#include "revc/api/check-list.h"

int
revc_check_list (const t_uchar * const errname,
                 const t_uchar * const str,
                 size_t size)
{
  if (size && str[size - 1])
    {
      revc_error (errname, "list without terminating 0\n");
      return -1;
    }
  return 0;
}



/* arch-tag: Tom Lord Mon Jun 27 14:15:37 2005 (check-list.c)
 */
