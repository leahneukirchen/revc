/* commit-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/deps/usage.h"
#include "revc/deps/error.h"
#include "revc/archives/archives.h"
#include "revc/archives/init-archives.h"
#include "revc/archives/close-archives.h"
#include "revc/api/opts.h"
#include "revc/api/commit.h"
#include "revc/api/pathlist-from-argv.h"
#include "revc/api/tree-setup.h"
#include "revc/api/revname-from-ancestry.h"
#include "revc/api/modify-ancestry-revname.h"
#include "revc/mains/commit-main.h"

const t_uchar * const revc_commit_usage = "[-r revision] subtree*";

int
revc_commit_main (int argc, char ** argv)
{
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  t_uchar * revname = 0;
  t_uchar * selected_subtrees = 0;
  size_t selected_subtrees_size = 0;
  t_uchar * path_to_root = 0;
  t_uchar * path_from_root = 0;
  t_uchar * manifest = 0;
  size_t manifest_size = 0;
  t_uchar * ignore = 0;
  size_t ignore_size = 0;
  t_uchar * ancestry = 0;
  size_t ancestry_size = 0;
  t_uchar * prereqs = 0;
  size_t prereqs_size = 0;
  struct revc_archives archs = REVC_ARCHIVES_NIL_INIT;

  struct revc_opts opts[] =
    {
      { REVC_ARG_OPT, "-r", "--revision", 0, &revname },
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_commit_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, errname);
      lim_free (0, path_to_root);
      lim_free (0, path_from_root);
      if (revc_close_archives (errname, &archs))
        answer = 2;
      return answer;
    }

  if (revc_tree_setup (errname,
                       &path_to_root, &path_from_root,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       &manifest, &manifest_size, &ignore, &ignore_size, &ancestry, &ancestry_size, &prereqs, &prereqs_size))
    goto bail;


  if (argc != argx)
    {
      if (revc_pathlist_from_argv (errname,
                                   &selected_subtrees, &selected_subtrees_size,
                                   path_from_root,
                                   argc - argx, argv + argx))
        goto bail;
      argx = argc;
    }
  else
    {
      selected_subtrees = str_save (0, ".");
      selected_subtrees_size = 2;
    }

  if (!revname)
    {
      revname = revc_revname_from_ancestry (errname, ancestry, ancestry_size);
      if (!revname)
        goto bail;
    }
  else
    {
      if (revc_modify_ancestry_revname (errname, &ancestry, &ancestry_size, revname))
        goto bail;
    }

  if (revc_init_archives (errname, &archs))
    goto bail;

  if (revc_commit (errname,
                   &archs, revname,
                   path_to_root, path_from_root,
                   manifest, manifest_size,
                   ignore, ignore_size,
                   ancestry, ancestry_size,
                   prereqs, prereqs_size,
                   selected_subtrees, selected_subtrees_size))
    goto bail;

  answer = 0;

  goto leave;
}




/* arch-tag: Tom Lord Sun Jun 26 13:13:24 2005 (commit-main.c)
 */
