/* lsr-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/deps/restore-dir.h"
#include "revc/deps/save-dir.h"
#include "revc/deps/chdir.h"
#include "revc/deps/usage.h"
#include "revc/api/opts.h"
#include "revc/api/listing-show-zero-sep.h"
#include "revc/api/listing-show-nl-sep.h"
#include "revc/api/lsr.h"
#include "revc/mains/lsr-main.h"

const t_uchar * const revc_lsr_usage = "[-0|--null] [dir]";

int
revc_lsr_main (int argc, char ** argv)
{
  t_revc_cwd_buffer cwdb = REVC_CWD_BUFFER_INIT;
  t_uchar * errname = 0;
  int zero_sep = 0;
  int argx = 0;
  t_uchar * dir = 0;
  int answer = 2;

  struct revc_opts opts[] =
    {
      { REVC_FLAG_OPT, "-0", "--null", &zero_sep },
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_lsr_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, errname);
      lim_free (0, dir);
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
      revc_usage (errname, revc_lsr_usage);
      goto bail;
    }

  if (revc_save_dir (errname, &cwdb))
    goto bail;

  if (revc_chdir (errname, dir))
    goto bail;

  if (revc_lsr (errname, (zero_sep ? revc_listing_show_zero_sep : revc_listing_show_nl_sep), 0))
    goto bail;

  answer = 0;

  goto leave;
}




/* arch-tag: Tom Lord Thu Jun 16 18:31:33 2005 (cmds/revc-lsr.c)
 */


