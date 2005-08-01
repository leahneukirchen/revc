/* adjust-inventory-main.c
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "revc/deps/rmrf.h"
#include "revc/deps/put-from-str.h"
#include "revc/deps/rename.h"
#include "revc/api/opts.h"
#include "revc/api/list-skip.h"
#include "revc/api/tree-setup.h"
#include "revc/api/pathlist-from-argv.h"
#include "revc/api/list-merge.h"
#include "revc/api/list-subtract.h"
#include "revc/mains/adjust-inventory-main.h"


const t_uchar * const revc_adjust_inventory_usage = "name ...";

int
revc_adjust_inventory_main (int argc, char ** argv,
                            int const add_to_manifest,
                            int const add_to_ignore)
{
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  t_uchar * path_to_root = 0;
  t_uchar * path_from_root = 0;
  t_uchar * path_to_manifest_file = 0;
  t_uchar * path_to_manifest_tmp_file = 0;
  t_uchar * path_to_ignore_file = 0;
  t_uchar * path_to_ignore_tmp_file = 0;
  t_uchar * manifest_list = 0;
  size_t manifest_list_size = 0;
  t_uchar * ignore_list = 0;
  size_t ignore_list_size = 0;
  t_uchar * add_list = 0;
  size_t add_list_size = 0;
  t_uchar * new_manifest_list = 0;
  size_t new_manifest_list_size = 0;
  t_uchar * new_ignore_list = 0;
  size_t new_ignore_list_size = 0;
  
  struct revc_opts opts[] =
    {
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_adjust_inventory_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, errname);
      lim_free (0, path_to_root);
      lim_free (0, path_from_root);
      lim_free (0, path_to_manifest_file);
      lim_free (0, path_to_manifest_tmp_file);
      lim_free (0, path_to_ignore_file);
      lim_free (0, path_to_ignore_tmp_file);
      lim_free (0, manifest_list);
      lim_free (0, ignore_list);
      lim_free (0, add_list);
      lim_free (0, new_manifest_list);
      lim_free (0, new_ignore_list);
      return answer;
    }

  if (revc_tree_setup (errname,
                       &path_to_root, &path_from_root,
                       &path_to_manifest_file, &path_to_manifest_tmp_file,
                       &path_to_ignore_file, &path_to_ignore_tmp_file,
                       0, 0, 0, 0, 
                       &manifest_list, &manifest_list_size,
                       &ignore_list, &ignore_list_size,
                       0, 0,
                       0, 0))
    goto bail;

  if (revc_pathlist_from_argv (errname,
                               &add_list, &add_list_size,
                               path_from_root,
                               argc - argx, argv + argx))
    goto bail;

  argx = argc;

  if (add_to_manifest)
    {
      if (revc_list_merge (errname,
                           &new_manifest_list, &new_manifest_list_size,
                           manifest_list, manifest_list_size,
                           add_list, add_list_size))
        goto bail;
    }
  else
    {
      if (revc_list_subtract (errname,
                              &new_manifest_list, &new_manifest_list_size,
                              manifest_list, manifest_list_size,
                              add_list, add_list_size))
        goto bail;
    }

  if (add_to_ignore)
    {
      if (revc_list_merge (errname,
                           &new_ignore_list, &new_ignore_list_size,
                           ignore_list, ignore_list_size,
                           add_list, add_list_size))
        goto bail;
    }
  else
    {
      if (revc_list_subtract (errname,
                              &new_ignore_list, &new_ignore_list_size,
                              ignore_list, ignore_list_size,
                              add_list, add_list_size))
        goto bail;
    }

  revc_rmrf (errname, path_to_manifest_tmp_file);
  revc_rmrf (errname, path_to_ignore_tmp_file);

  if (revc_put_from_str (errname, path_to_manifest_tmp_file, new_manifest_list, new_manifest_list_size, 0))
    goto bail;
  if (revc_put_from_str (errname, path_to_ignore_tmp_file, new_ignore_list, new_ignore_list_size, 0))
    goto bail;

  if (revc_rename (errname, path_to_manifest_tmp_file, path_to_manifest_file))
    goto bail;
  if (revc_rename (errname, path_to_ignore_tmp_file, path_to_ignore_file))
    goto bail;

  answer = 0;

  goto leave;
}  



/* arch-tag: Tom Lord Thu Jun 23 13:20:04 2005 (mains/adjust-inventory-main.c)
 */


