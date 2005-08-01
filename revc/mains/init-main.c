/* init-main.c: 
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
#include "revc/deps/restore-dir.h"
#include "revc/deps/save-dir.h"
#include "revc/deps/chdir.h"
#include "revc/api/opts.h"
#include "revc/api/init.h"
#include "revc/mains/init-main.h"

const t_uchar * const revc_init_usage = "[dir]";

int
revc_init_main (int argc, char ** argv)
{
  t_revc_cwd_buffer cwdb = REVC_CWD_BUFFER_INIT;
  t_uchar * errname = 0;
  int argx = 0;
  t_uchar * dir = 0;
  int answer = 2;

  struct revc_opts opts[] =
    {
      { REVC_END_OPTS, }
    };


  if (revc_opts (&errname, &argx, opts, argc, argv, revc_init_usage))
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
      revc_usage (errname, revc_init_usage);
      goto bail;
    }

  if (revc_save_dir (errname, &cwdb))
    goto bail;

  if (revc_chdir (errname, dir))
    goto bail;

  if (revc_init (errname))
    goto bail;

  answer = 0;

  goto leave;

}




/* arch-tag: Tom Lord Thu Jun 23 11:27:53 2005 (init-main.c)
 */
