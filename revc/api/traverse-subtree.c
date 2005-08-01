/* traverse-subtree.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/arrays/ar.h"
#include "hackerlab/sort/qsort.h"
#include "hackerlab/char/str.h"
#include "revc/deps/chdir.h"
#include "revc/deps/list-cwd.h"
#include "revc/deps/save-dir.h"
#include "revc/deps/restore-dir.h"
#include "revc/deps/file-is-dir.h"
#include "revc/api/traverse-subtree.h"


/* __STDC__ prototypes for static functions */

static int traverse (const t_uchar * const errname,
                     const t_uchar * const path_from_root,
                     t_revc_traverse_fn fn,
                     void * closure,
                     t_uchar ** const mfst, size_t * mpos,
                     t_uchar ** const ign, size_t * ipos);
static int qsort_str_cmp (void * vap, void * vbp, void * ign);



int
revc_traverse_subtree (const t_uchar * const errname,
                       const t_uchar * const path_from_root,
                       t_revc_traverse_fn fn,
                       void * closure,
                       t_uchar ** const mfst, size_t * mpos,
                       t_uchar ** const ign, size_t * ipos)
{
  if (traverse (errname,
                path_from_root,
                fn, closure,
                mfst, mpos,
                ign, ipos))
    return -1;

  return fn (errname, closure, path_from_root, 0, 0, mfst, mpos, ign, ipos);
}


static int
traverse (const t_uchar * const errname,
          const t_uchar * const path_from_root,
          t_revc_traverse_fn fn,
          void * closure,
          t_uchar ** const mfst, size_t * mpos,
          t_uchar ** const ign, size_t * ipos)
{
  t_uchar ** dir_listing = 0;
  size_t x;
  int status = 0;

  if (revc_list_cwd (errname, &dir_listing))
    return -1;

  quicksort (dir_listing, ar_size ((void *)dir_listing, 0, sizeof (t_uchar *)), sizeof (t_uchar *), qsort_str_cmp, 0);

  for (x = 0; x < ar_size ((void *)dir_listing, 0, sizeof (t_uchar *)); ++x)
    {
      t_uchar * parent_slash = 0;
      t_uchar * node_path = 0;
      int traverse_subtree;

      parent_slash = str_alloc_cat (0, path_from_root, "/");
      node_path = str_alloc_cat (0, parent_slash, dir_listing[x]);
      lim_free (0, parent_slash);
      parent_slash = 0;
      
      traverse_subtree = fn (errname, closure, path_from_root, node_path, dir_listing[x], mfst, mpos, ign, ipos);
      if (traverse_subtree < 0)
        status = -1;

      if (traverse_subtree && (status >= 0) && node_path && revc_file_is_dir (errname, dir_listing[x]))
        {
          t_revc_cwd_buffer cwdb = REVC_CWD_BUFFER_INIT;

          if (revc_save_dir (errname, &cwdb))
            status = -1;
          else if (   revc_chdir (errname, dir_listing[x])
                   || traverse (errname, node_path, fn, closure, mfst, mpos, ign, ipos))
            {
              (void)revc_restore_dir (errname, &cwdb);
              status = -1;
            }
          else if (revc_restore_dir (errname, &cwdb))
            status = -1;
        }

      lim_free (0, node_path);
      node_path = 0;

      if (status < 0)
        break;
    }

  for (x = 0; x < ar_size ((void *)dir_listing, 0, sizeof (t_uchar *)); ++x)
    {
      lim_free (0, dir_listing[x]);
    }
  ar_free ((void **)&dir_listing, 0);

  return status;
}



static int
qsort_str_cmp (void * vap, void * vbp, void * ign)
{
  t_uchar * a = *(t_uchar **)vap;
  t_uchar * b = *(t_uchar **)vbp;
  return str_cmp (a, b);
}




/* arch-tag: Tom Lord Thu Jun 16 19:15:58 2005 (traverse-subtree.c)
 */
