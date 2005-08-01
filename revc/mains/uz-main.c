/* uz-main.c
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "revc/deps/usage.h"
#include "revc/deps/get-to-string.h"
#include "revc/deps/write.h"
#include "revc/api/opts.h"
#include "revc/api/unzip-string.h"
#include "revc/mains/uz-main.h"

const t_uchar * const revc_uz_usage = "[file]";

int
revc_uz_main (int argc, char ** argv)
{
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  t_uchar * file = 0;
  t_uchar * str = 0;
  size_t size = 0;
  t_uchar * uz_str = 0;
  size_t uz_size = 0;

  struct revc_opts opts[] =
    {
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_uz_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, file);
      lim_free (0, str);
      return answer;
    }

  if (argx < argc)
    {
      if (str_cmp (argv[argx], "-"))
        file = str_save (0, argv[argx]);
      ++argx;
    }

  if (argx != argc)
    {
      revc_usage (errname, revc_uz_usage);
      goto bail;
    }

  if (revc_get_to_string (errname, &str, &size, file))
    goto bail;

  if (revc_unzip_string (errname, &uz_str, &uz_size, str, size))
    goto bail;

  if (revc_write (errname, uz_str, uz_size))
    goto bail;

  answer = 0;
  goto leave;
}



/* arch-tag: Tom Lord Thu Jul  7 16:34:38 2005 (mains/uz-main.c)
 */
