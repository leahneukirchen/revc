/* list-cwd.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/arrays/ar.h"
#include "hackerlab/vu/safe.h"
#include "revc/deps/list-cwd.h"

int
revc_list_cwd (const t_uchar * const errname,
               t_uchar *** list_ret)
{
  t_uchar ** answer = 0;
  DIR * dir;

  safe_opendir (&dir, ".");

  while (1)
    {
      char * fname = 0;
      t_uchar ** place;

      safe_readdir (&fname, dir);
      if (!fname)
        break;

      if (!str_cmp (fname, ".") || !str_cmp (fname, ".."))
        {
          lim_free (0, fname);
          continue;
        }

      place = (t_uchar **)ar_push ((void **)&answer, 0, sizeof (t_uchar *));
      *place = fname;
    }

  safe_closedir (dir);

  *list_ret = answer;
  return 0;
}



/* arch-tag: Tom Lord Thu Jun 23 13:09:13 2005 (list-cwd.c)
 */
