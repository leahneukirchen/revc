/* tree-setup.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "revc/deps/get-to-string.h"
#include "revc/api/zget-to-string.h"
#include "revc/api/find-root.h"
#include "revc/api/check-list.h"
#include "revc/api/tree-setup.h"

int
revc_tree_setup  (const t_uchar * const errname,
                  t_uchar ** const path_to_root_ret,
                  t_uchar ** const path_from_root_ret,
                  t_uchar ** const path_to_manifest_file_ret,
                  t_uchar ** const path_to_manifest_tmp_file_ret,
                  t_uchar ** const path_to_ignore_file_ret,
                  t_uchar ** const path_to_ignore_tmp_file_ret,
                  t_uchar ** const path_to_ancestry_file_ret,
                  t_uchar ** const path_to_ancestry_tmp_file_ret,
                  t_uchar ** const path_to_prereqs_file_ret,
                  t_uchar ** const path_to_prereqs_tmp_file_ret,
                  t_uchar ** const manifest_list_ret,
                  size_t  *  const manifest_list_size_ret,
                  t_uchar ** const ignore_list_ret,
                  size_t  *  const ignore_list_size_ret,
                  t_uchar ** const ancestry_list_ret,
                  size_t  *  const ancestry_list_size_ret,
                  t_uchar ** const prereqs_list_ret,
                  size_t  *  const prereqs_list_size_ret)
{
  t_uchar * path_to_root = 0;
  t_uchar * path_from_root = 0;
  t_uchar * path_to_manifest_file = 0;
  t_uchar * path_to_manifest_tmp_file = 0;
  t_uchar * path_to_ignore_file = 0;
  t_uchar * path_to_ignore_tmp_file = 0;
  t_uchar * path_to_ancestry_file = 0;
  t_uchar * path_to_ancestry_tmp_file = 0;
  t_uchar * path_to_prereqs_file = 0;
  t_uchar * path_to_prereqs_tmp_file = 0;
  t_uchar * manifest_list = 0;
  size_t manifest_list_size = 0;
  t_uchar * ignore_list = 0;
  size_t ignore_list_size = 0;
  t_uchar * ancestry_list = 0;
  size_t ancestry_list_size = 0;
  t_uchar * prereqs_list = 0;
  size_t prereqs_list_size = 0;

  int answer = 2;

  if (revc_find_root (errname, &path_to_root, &path_from_root))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, path_to_root);
      lim_free (0, path_from_root);
      lim_free (0, path_to_manifest_file);
      lim_free (0, path_to_manifest_tmp_file);
      lim_free (0, path_to_ignore_file);
      lim_free (0, path_to_ignore_tmp_file);
      lim_free (0, path_to_ancestry_file);
      lim_free (0, path_to_ancestry_tmp_file);
      lim_free (0, path_to_prereqs_file);
      lim_free (0, path_to_prereqs_tmp_file);
      lim_free (0, manifest_list);
      lim_free (0, ignore_list);
      lim_free (0, ancestry_list);
      lim_free (0, prereqs_list);

      return answer;
    }

  if (path_to_root_ret)
    *path_to_root_ret = str_save (0, path_to_root);

  if (path_from_root_ret)
    *path_from_root_ret = str_save (0, path_from_root);

  if (path_to_manifest_file_ret || manifest_list_ret || manifest_list_size_ret)
    {
      path_to_manifest_file = str_alloc_cat (0, path_to_root, "/.revc/manifest");

      if (path_to_manifest_file_ret)
        *path_to_manifest_file_ret = str_save (0, path_to_manifest_file);

      if (manifest_list_ret || manifest_list_size_ret)
        {
          if (revc_get_to_string (errname, &manifest_list, &manifest_list_size, path_to_manifest_file))
            goto bail;

          if (revc_check_list (errname, manifest_list, manifest_list_size))
            return -1;

          if (manifest_list_ret)
            {
              *manifest_list_ret = manifest_list;
              manifest_list = 0;
            }

          if (manifest_list_size_ret)
            *manifest_list_size_ret = manifest_list_size;
        }
    }

  if (path_to_ignore_file_ret || ignore_list_ret || ignore_list_size_ret)
    {
      path_to_ignore_file = str_alloc_cat (0, path_to_root, "/.revc/ignore");

      if (path_to_ignore_file_ret)
        *path_to_ignore_file_ret = str_save (0, path_to_ignore_file);

      if (ignore_list_ret || ignore_list_size_ret)
        {
          if (revc_get_to_string (errname, &ignore_list, &ignore_list_size, path_to_ignore_file))
            goto bail;

          if (revc_check_list (errname, ignore_list, ignore_list_size))
            return -1;

          if (ignore_list_ret)
            {
              *ignore_list_ret = ignore_list;
              ignore_list = 0;
            }

          if (ignore_list_size_ret)
            *ignore_list_size_ret = ignore_list_size;
        }
    }

  if (path_to_ancestry_file_ret || ancestry_list_ret || ancestry_list_size_ret)
    {
      path_to_ancestry_file = str_alloc_cat (0, path_to_root, "/.revc/ancestry");

      if (path_to_ancestry_file_ret)
        *path_to_ancestry_file_ret = str_save (0, path_to_ancestry_file);

      if (ancestry_list_ret || ancestry_list_size_ret)
        {
          if (revc_get_to_string (errname, &ancestry_list, &ancestry_list_size, path_to_ancestry_file))
            goto bail;

          if (revc_check_list (errname, ancestry_list, ancestry_list_size))
            return -1;

          if (ancestry_list_ret)
            {
              *ancestry_list_ret = ancestry_list;
              ancestry_list = 0;
            }

          if (ancestry_list_size_ret)
            *ancestry_list_size_ret = ancestry_list_size;
        }
    }

  if (path_to_prereqs_file_ret || prereqs_list_ret || prereqs_list_size_ret)
    {
      path_to_prereqs_file = str_alloc_cat (0, path_to_root, "/.revc/prereqs");

      if (path_to_prereqs_file_ret)
        *path_to_prereqs_file_ret = str_save (0, path_to_prereqs_file);

      if (prereqs_list_ret || prereqs_list_size_ret)
        {
          if (revc_zget_to_string (errname, &prereqs_list, &prereqs_list_size, path_to_prereqs_file))
            goto bail;

          if (revc_check_list (errname, prereqs_list, prereqs_list_size))
            return -1;

          if (prereqs_list_ret)
            {
              *prereqs_list_ret = prereqs_list;
              prereqs_list = 0;
            }

          if (prereqs_list_size_ret)
            *prereqs_list_size_ret = prereqs_list_size;
        }
    }

  if (path_to_manifest_tmp_file_ret)
    {
      path_to_manifest_tmp_file = str_alloc_cat (0, path_to_root, "/.revc/,,manifest");
      *path_to_manifest_tmp_file_ret = str_save (0, path_to_manifest_tmp_file);
    }

  if (path_to_ignore_tmp_file_ret)
    {
      path_to_ignore_tmp_file = str_alloc_cat (0, path_to_root, "/.revc/,,ignore");
      *path_to_ignore_tmp_file_ret = str_save (0, path_to_ignore_tmp_file);
    }

  if (path_to_ancestry_tmp_file_ret)
    {
      path_to_ancestry_tmp_file = str_alloc_cat (0, path_to_root, "/.revc/,,ancestry");
      *path_to_ancestry_tmp_file_ret = str_save (0, path_to_ancestry_tmp_file);
    }

  if (path_to_prereqs_tmp_file_ret)
    {
      path_to_prereqs_tmp_file = str_alloc_cat (0, path_to_root, "/.revc/,,prereqs");
      *path_to_prereqs_tmp_file_ret = str_save (0, path_to_prereqs_tmp_file);
    }

  answer = 0;
  goto leave;
}




/* arch-tag: Tom Lord Mon Jun 27 14:32:38 2005 (api/tree-setup.c)
 */
