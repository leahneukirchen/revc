/* z-main.c
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
#include "revc/api/zip-string.h"
#include "revc/mains/z-main.h"

const t_uchar * const revc_z_usage = "[file]";

int
revc_z_main (int argc, char ** argv)
{
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  t_uchar * file = 0;
  t_uchar * str = 0;
  size_t size = 0;
  t_uchar * z_str = 0;
  size_t z_size = 0;

  struct revc_opts opts[] =
    {
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_z_usage))
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
      revc_usage (errname, revc_z_usage);
      goto bail;
    }

  if (revc_get_to_string (errname, &str, &size, file))
    goto bail;

  if (revc_zip_string (errname, &z_str, &z_size, str, size))
    goto bail;

  if (revc_write (errname, z_str, z_size))
    goto bail;

  answer = 0;
  goto leave;
}



/* arch-tag: Tom Lord Thu Jul  7 16:24:12 2005 (mains/z-main.c)
 */
