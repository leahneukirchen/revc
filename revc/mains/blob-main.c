/* blob-main.c
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "revc/deps/fmt.h"
#include "revc/deps/usage.h"
#include "revc/api/opts.h"
#include "revc/api/pathlist-from-argv.h"
#include "revc/api/node-blob.h"
#include "revc/api/list-skip.h"
#include "revc/api/constants.h"
#include "revc/api/tree-setup.h"
#include "revc/mains/blob-main.h"

const t_uchar * const revc_manifest_usage = "[-0|--null] node*";

int
revc_blob_main (int argc, char ** argv)
{
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  int zero_sep = 0;
  t_uchar * path_to_root = 0;
  t_uchar * path_from_root = 0;
  t_uchar * manifest = 0;
  size_t manifest_size;
  t_uchar * path_list = 0;
  size_t path_list_size = 0;
  t_uchar * path_list_pos = 0;
  size_t path_list_remain = 0;

  struct revc_opts opts[] =
    {
      { REVC_FLAG_OPT, "-0", "--null", &zero_sep },
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_manifest_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, errname);
      lim_free (0, path_list);
      return answer;
    }

  if (revc_tree_setup (errname,
                       &path_to_root, &path_from_root,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       &manifest, &manifest_size, 0, 0, 0, 0, 0, 0))
    goto bail;

  if (revc_pathlist_from_argv (errname, &path_list, &path_list_size, path_from_root, argc - argx, argv + argx))
    goto bail;

  argx = argc;

  path_list_pos = path_list;
  path_list_remain = path_list_size;

  while (path_list_remain)
    {
      int node_status;
      t_uchar blob_addr[REVC_MAX_BLOB_ADDR_SIZE + 1];

      node_status = revc_node_blob (errname, blob_addr,  path_to_root, path_from_root, path_list_pos);
      if (node_status < 0)
        goto bail;

      if (!node_status)
        {
          revc_fmt (errname, "%s: *\n", path_list_pos);
        }
      else
        {
          revc_fmt (errname, "%s: %s\n", path_list_pos, blob_addr);
        }

      revc_list_skip (&path_list_pos, &path_list_remain);
    }

  answer = 0;
  
  goto leave;
}  




/* arch-tag: Tom Lord Wed Jun 29 08:43:25 2005 (mains/blob-main.c)
 */
