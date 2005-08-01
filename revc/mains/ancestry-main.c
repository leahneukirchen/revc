/* ancestry-main.c
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
#include "revc/deps/save-dir.h"
#include "revc/deps/restore-dir.h"
#include "revc/deps/chdir.h"
#include "revc/deps/write.h"
#include "revc/api/opts.h"
#include "revc/api/list-skip.h"
#include "revc/api/tree-setup.h"
#include "revc/api/parse-ancestry-line.h"
#include "revc/mains/ancestry-main.h"

const t_uchar * const revc_manifest_usage = "[-0|--null] [dir]";

int
revc_ancestry_main (int argc, char ** argv)
{
  t_revc_cwd_buffer cwdb = REVC_CWD_BUFFER_INIT;
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  int zero_sep = 0;
  t_uchar * dir = 0;
  t_uchar * path_from_root = 0;
  t_uchar * list = 0;
  size_t list_size = 0;
  t_uchar * list_pos = 0;
  size_t list_remain = 0;
  
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
      lim_free (0, dir);
      lim_free (0, path_from_root);
      lim_free (0, list);
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
    dir = str_save (0, ".");

  if (argx != argc)
    {
      revc_usage (errname, revc_manifest_usage);
      goto bail;
    }

  if (revc_save_dir (errname, &cwdb))
    goto bail;

  if (revc_chdir (errname, dir))
    goto bail;

  if (revc_tree_setup (errname,
                       0, &path_from_root,
                       0, 0, 0, 0, 0, 0, 0, 0, 
                       0, 0, 0, 0, &list, &list_size, 0, 0))
    goto bail;
  
  
  list_pos = list;
  list_remain = list_size;

  if (list_remain)
    revc_list_skip (&list_pos, &list_remain);
  
  while (list_remain)
    {
      size_t ancestor_number;
      size_t merge_number;
      t_uchar * revision_name;

      if (revc_parse_ancestry_line (errname, &ancestor_number, &merge_number, &revision_name, list_pos))
        goto bail;

      if (merge_number == 0)
        {
          if (zero_sep)
            {
              if (revc_write (errname, revision_name, str_length (revision_name) + 1))
                goto bail;
            }
          else
            {
              if (revc_write (errname, revision_name, str_length (revision_name)))
                goto bail;
              if (revc_write (errname, "\n", 1))
                goto bail;
            }
        }
      revc_list_skip (&list_pos, &list_remain);
    }
  
  answer = 0;
  
  goto leave;
}  




/* arch-tag: Tom Lord Sun Jun 26 14:54:46 2005 (mains/ancestry-main.c)
 */
