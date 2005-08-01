/* prereqs-main.c
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
#include "revc/deps/fmt.h"
#include "revc/deps/save-dir.h"
#include "revc/deps/restore-dir.h"
#include "revc/deps/chdir.h"
#include "revc/deps/write.h"
#include "revc/api/constants.h"
#include "revc/api/opts.h"
#include "revc/api/prereqs-skip.h"
#include "revc/api/tree-setup.h"
#include "revc/api/parse-ancestry-line.h"
#include "revc/mains/prereqs-main.h"

const t_uchar * const revc_prereqs_usage = "[-0|--null] [dir]";

int
revc_prereqs_main (int argc, char ** argv)
{
  t_revc_cwd_buffer cwdb = REVC_CWD_BUFFER_INIT;
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  int zero_sep = 0;
  t_uchar * dir = 0;
  t_uchar * prereqs = 0;
  size_t prereqs_size = 0;
  t_uchar * prereqs_pos = 0;
  size_t prereqs_remain = 0;
  
  struct revc_opts opts[] =
    {
      { REVC_FLAG_OPT, "-0", "--null", &zero_sep },
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_prereqs_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, errname);
      lim_free (0, dir);
      lim_free (0, prereqs);
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
      revc_usage (errname, revc_prereqs_usage);
      goto bail;
    }

  if (revc_save_dir (errname, &cwdb))
    goto bail;

  if (revc_chdir (errname, dir))
    goto bail;

  if (revc_tree_setup (errname,
                       0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, &prereqs, &prereqs_size))
    goto bail;
  
  
  prereqs_pos = prereqs;
  prereqs_remain = prereqs_size;
  
  while (prereqs_remain)
    {
      revc_fmt (errname, "%s\n", prereqs_pos);
      if (revc_prereqs_skip (errname, &prereqs_pos, &prereqs_remain))
        goto bail;
    }

  answer = 0;
  
  goto leave;
}  




/* arch-tag: Tom Lord Sun Jul  3 07:55:04 2005 (mains/prereqs-main.c)
 */
