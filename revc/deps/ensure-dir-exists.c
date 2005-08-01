/* ensure-dir-exists.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/vu/safe.h"
#include "hackerlab/vu/safe-vu-utils.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/ensure-dir-exists.h"



int
revc_ensure_dir_exists (const t_uchar * const errname,
                        const t_uchar * name)
{
  if (safe_file_is_directory_following (name))
    {
      return 0;
    }

  {
    t_uchar * parent = 0;

    parent = file_name_directory_file (0, name);
    if (parent)
      revc_ensure_dir_exists (errname, parent);

    if (!safe_file_is_directory_following (name))
      safe_mkdir (name, 0777);

    lim_free (0, parent);

    return 0;
  }
}





/* arch-tag: Tom Lord Sun Jun 19 14:35:28 2005 (ensure-dir-exists.c)
 */
