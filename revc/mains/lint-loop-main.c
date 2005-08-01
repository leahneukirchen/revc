/* lint-loop-main.c
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/deps/save-dir.h"
#include "revc/deps/restore-dir.h"
#include "revc/deps/chdir.h"
#include "revc/deps/usage.h"
#include "revc/api/opts.h"
#include "revc/api/tree-setup.h"
#include "revc/api/lint.h"
#include "revc/api/listing-show-zero-sep.h"
#include "revc/api/listing-show-nl-sep.h"
#include "revc/api/listing-show-plus-zero-sep.h"
#include "revc/api/listing-show-plus-nl-sep.h"
#include "revc/api/listing-show-minus-zero-sep.h"
#include "revc/api/listing-show-minus-nl-sep.h"
#include "revc/mains/lint-loop-main.h"



const t_uchar * revc_lint_loop_usage = "[-0|--null] [dir]";

int
revc_lint_loop_main (int argc, char ** argv,
                     int show_new,
                     int show_missing)
{
  t_revc_cwd_buffer cwdb = REVC_CWD_BUFFER_INIT;
  t_uchar * errname = 0;
  int answer = 2;
  int argx;
  int zero_sep = 0;
  int lint_unclean = 0;

  struct revc_opts opts[] =
    {
      { REVC_FLAG_OPT, "-0", "--null", &zero_sep },
      { REVC_END_OPTS, }
    };

  t_uchar * dir = 0;
  t_uchar * path_to_root = 0;
  t_uchar * path_from_root = 0;
  t_uchar * manifest_list = 0;
  size_t manifest_list_size = 0;
  t_uchar * ignore_list = 0;
  size_t ignore_list_size = 0;

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_lint_loop_usage))
    {
    bail:
      answer = 2;

    leave:
      lim_free (0, errname);
      lim_free (0, dir);
      lim_free (0, path_to_root);
      lim_free (0, path_from_root);
      lim_free (0, manifest_list);
      lim_free (0, ignore_list);
      if (revc_restore_dir (errname, &cwdb))
        answer = 2;
      return answer;
    }

  if (argx < argc)
    {
      dir = str_save (0, argv[argx]);
      ++argx;
    }
  else
    {
      dir = str_save (0, ".");
    }

  if (argx != argc)
    {
      revc_usage (errname, revc_lint_loop_usage);
      goto bail;
    }

  if (revc_save_dir (errname, &cwdb))
    goto bail;

  if (revc_chdir (errname, dir))
    goto bail;

  if (revc_tree_setup (errname,
                       &path_to_root, &path_from_root,
                       0, 0,
                       0, 0,
                       0, 0,
                       0, 0,
                       &manifest_list, &manifest_list_size,
                       &ignore_list, &ignore_list_size,
                       0, 0,
                       0, 0))
    goto bail;


  {
    t_revc_lint_callback_fn new_callback = 0;
    t_revc_lint_callback_fn missing_callback = 0;

    if (show_new && show_missing)
      {
        new_callback = (zero_sep ? revc_listing_show_plus_zero_sep : revc_listing_show_plus_nl_sep);
        missing_callback = (zero_sep ? revc_listing_show_minus_zero_sep : revc_listing_show_minus_nl_sep);
      }
    else if (show_new)
      {
        new_callback = (zero_sep ? revc_listing_show_zero_sep : revc_listing_show_nl_sep);
      }
    else if (show_missing)
      {
        missing_callback = (zero_sep ? revc_listing_show_zero_sep : revc_listing_show_nl_sep);
      }

    if (revc_lint (errname,
                   &lint_unclean,
                   path_from_root,
                   manifest_list, manifest_list_size,
                   ignore_list, ignore_list_size,
                   new_callback,
                   missing_callback,
                   0))
      goto bail;
  }

  if (lint_unclean)
    answer = 1;
  else
    answer = 0;

  goto leave;
}




/* arch-tag: Tom Lord Thu Jun 23 17:29:43 2005 (mains/lint-loop-main.c)
 */


