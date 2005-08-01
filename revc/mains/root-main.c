/* root-main.c
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
#include "revc/deps/cwd.h"
#include "revc/deps/write.h"
#include "revc/api/opts.h"
#include "revc/api/find-root.h"
#include "revc/mains/root-main.h"

const t_uchar * const revc_root_usage = "[-0|--null] [-r|--relative] [dir]";

int
revc_root_main (int argc, char ** argv)
{
  t_revc_cwd_buffer cwdb = REVC_CWD_BUFFER_INIT;
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  int relative = 0;
  int zero_sep = 0;
  t_uchar * dir = 0;
  t_uchar * path_to_root = 0;
  
  struct revc_opts opts[] =
    {
      { REVC_FLAG_OPT, "-r", "--relative", &relative },
      { REVC_FLAG_OPT, "-0", "--null", &zero_sep },
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_root_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, errname);
      lim_free (0, dir);
      lim_free (0, path_to_root);
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
      revc_usage (errname, revc_root_usage);
      goto bail;
    }

  if (revc_save_dir (errname, &cwdb))
    goto bail;

  if (revc_chdir (errname, dir))
    goto bail;

  if (revc_find_root (errname, &path_to_root, 0))
    goto bail;
  
  if (relative)
    {
      if (zero_sep)
        {
          if (revc_write (errname, path_to_root, str_length (path_to_root) + 1))
            goto bail;
        }
      else
        {
          if (revc_write (errname, path_to_root, str_length (path_to_root)))
            goto bail;
          if (revc_write (errname, "\n", 1))
            goto bail;
        }
    }
  else
    {
      t_uchar * abs = 0;

      if (revc_chdir (errname, path_to_root))
        goto bail;

      abs = revc_cwd (errname);
      if (zero_sep)
        {
          if (revc_write (errname, abs, str_length (abs) + 1))
            goto bail;
        }
      else
        {
          if (revc_write (errname, abs, str_length (abs)))
            goto bail;
          if (revc_write (errname, "\n", 1))
            goto bail;
        }
      lim_free (0, abs);
    }


  answer = 0;
  goto leave;
}  




/* arch-tag: Tom Lord Sun Jun 26 16:12:39 2005 (mains/root.c)
 */
