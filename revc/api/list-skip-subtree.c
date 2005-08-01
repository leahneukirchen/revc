/* list-skip-subtree.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/fs/file-names.h"
#include "revc/api/check-list.h"
#include "revc/api/list-skip.h"
#include "revc/api/list-skip-subtree.h"



void
revc_list_skip_subtree (t_uchar ** const inv,
                        size_t * const len)
{
  t_uchar * const top = *inv;

  invariant (!revc_check_list ((t_uchar *)"revc", *inv, *len));

  while (*len)
    {
      if (!file_name_is_path_prefix (top, *inv))
        break;
      revc_list_skip (inv, len);
    }
}





/* arch-tag: Tom Lord Mon Jul 11 10:54:31 2005 (api/list-skip-subtree.c)
 */
