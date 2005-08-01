/* revc-rename.c
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/machine/types.h"
#include "hackerlab/arrays/ar.h"
#include "hackerlab/char/str.h"
#include "hackerlab/vu/safe.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/usage.h"
#include "revc/deps/rmrf.h"
#include "revc/deps/put-from-str.h"
#include "revc/deps/rename.h"
#include "revc/api/opts.h"
#include "revc/api/tree-setup.h"
#include "revc/api/list-rename.h"
#include "revc/mains/rename-main.h"




const t_uchar * const revc_rename_usage = "from to";

int
revc_rename_main (int argc, char ** argv)
{
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  t_uchar * from = 0;
  t_uchar * to = 0;
  t_uchar * from_path = 0;
  t_uchar * to_path = 0;
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
  t_uchar * new_manifest_list = 0;
  size_t new_manifest_list_size = 0;
  t_uchar * new_ignore_list = 0;
  size_t new_ignore_list_size = 0;
  
  struct revc_opts opts[] =
    {
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_rename_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, errname);
      lim_free (0, from);
      lim_free (0, to);
      lim_free (0, path_to_root);
      lim_free (0, path_from_root);
      lim_free (0, path_to_manifest_file);
      lim_free (0, path_to_manifest_tmp_file);
      lim_free (0, path_to_ignore_file);
      lim_free (0, path_to_ignore_tmp_file);
      lim_free (0, manifest_list);
      lim_free (0, ignore_list);
      lim_free (0, new_manifest_list);
      lim_free (0, new_ignore_list);
      return answer;
    }

  if ((argx + 2) != argc)
    {
      revc_usage (errname, revc_rename_usage);
      goto bail;
    }

  from = str_save (0, argv[argx]);
  to = str_save (0, argv[argx + 1]);
  argx += 2;

  if (revc_tree_setup (errname,
                       &path_to_root, &path_from_root,
                       &path_to_manifest_file, &path_to_manifest_tmp_file,
                       &path_to_ignore_file, &path_to_ignore_tmp_file,
                       0, 0, 0, 0,
                       &manifest_list, &manifest_list_size,
                       &ignore_list, &ignore_list_size,
                       0, 0, 0, 0))
    goto bail;

  from_path = str_alloc_cat_many (0, path_from_root, "/", from, str_end);
  to_path = str_alloc_cat_many (0, path_from_root, "/", to, str_end);

  if (revc_list_rename (errname,
                        &new_manifest_list, &new_manifest_list_size,
                        manifest_list, manifest_list_size,
                        from_path, to_path))
    goto bail;

  if (revc_list_rename (errname,
                        &new_ignore_list, &new_ignore_list_size,
                        ignore_list, ignore_list_size,
                        from_path, to_path))
    goto bail;


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




/* arch-tag: Tom Lord Sat Jun 18 09:19:52 2005 (cmds/revc-rename.c)
 */


