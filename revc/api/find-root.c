/* find-root.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "revc/deps/error.h"
#include "revc/deps/cwd.h"
#include "revc/deps/file-is-dir.h"
#include "revc/api/find-root.h"



int
revc_find_root (const t_uchar * const errname,
                t_uchar ** const to_root_ret,
                t_uchar ** const from_root_ret)
{
  t_uchar * cwd = 0;
  t_uchar * to_root = 0;
  t_uchar * from_root;
  int not_too_deep;

  cwd = revc_cwd (errname);
  if (!cwd)
    return -1;

  to_root = str_save (0, ".");

  from_root = cwd + str_length (cwd);

  not_too_deep = 256;
  while (not_too_deep--)
    {
      t_uchar * maybe_revc_dir = 0;

      maybe_revc_dir = str_alloc_cat (0, to_root, "/.revc");

      if (revc_file_is_dir (errname, maybe_revc_dir))
        {
          if (from_root_ret)
            {
              if (!*from_root)
                *from_root_ret = str_save (0, ".");
              else
                *from_root_ret = str_alloc_cat (0, "./", from_root);
            }
          if (to_root_ret)
            *to_root_ret = to_root;
          else
            lim_free (0, to_root);
          lim_free (0, cwd);
          return 0;
        }

      lim_free (0, maybe_revc_dir);
      maybe_revc_dir = 0;
      if (!str_cmp (to_root, "."))
        {
          lim_free (0, to_root);
          to_root = str_save (0, "..");
        }
      else
        {
          t_uchar * next = str_alloc_cat (0, to_root, "/..");
          lim_free (0, to_root);
          to_root = next;
        }
      if ((from_root != cwd) && (from_root[-1] == '/'))
        {
          --from_root;
          if (from_root != cwd)
            --from_root;
        }
      while ((from_root != cwd) && (*from_root != '/'))
        --from_root;
      if (*from_root == '/')
        ++from_root;
    }

  lim_free (0, to_root);
  lim_free (0, cwd);

  revc_error (errname, "not in a revc tree (or too deeply nested)\n");

  return -1;
}



/* arch-tag: Tom Lord Thu Jun 16 18:43:46 2005 (find-root.c)
 */
