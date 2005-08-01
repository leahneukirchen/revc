/* rmrf.c:
 *
 ****************************************************************
 * Copyright (C) 2003 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/os/errno.h"
#include "hackerlab/os/errno-to-string.h"
#include "hackerlab/char/str.h"
#include "hackerlab/char/str-many.h"
#include "hackerlab/fs/file-names.h"
#include "hackerlab/vu/safe.h"
#include "revc/deps/rmrf.h"



void
revc_rmrf (const t_uchar * const errname,
           const t_uchar * const path)
{
  int errn;
  struct stat stat_buf;

  if (vu_lstat (&errn, path, &stat_buf))
    {
      if (errn == ENOENT)
        return;
      else
        {
          safe_printfmt (2, "%s: I/O error (%s) from lstat for %s\n", errname, errno_to_string (errn), path);
          exit (2);
        }
    }


  if (!S_ISDIR (stat_buf.st_mode))
    {
      safe_unlink (path);
    }
  else
    {
      DIR * dir;
      char * file;

      safe_opendir (&dir, path);

      while (!safe_readdir (&file, dir))
        {
          t_uchar * sub_path = str_alloc_cat_many (0, path, "/", file, str_end);

          if (str_cmp (file, ".") && str_cmp (file, ".."))
            revc_rmrf (errname, sub_path);

          lim_free (0, sub_path);
        }

      safe_closedir (dir);
      safe_rmdir (path);
    }
}



/* arch-tag: Tom Lord Sun Jun 19 16:03:01 2005 (librevc/rmrf.c)
 */
