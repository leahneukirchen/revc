/* get-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/usage.h"
#include "revc/deps/rmrf.h"
#include "revc/deps/rename.h"
#include "revc/api/opts.h"
#include "revc/api/is-valid-changeset-name.h"
#include "revc/api/get.h"
#include "revc/archives/init-archives.h"
#include "revc/archives/close-archives.h"
#include "revc/mains/get-main.h"

const t_uchar * const revc_get_usage = "[-o output-dir] revision";

int
revc_get_main (int argc, char ** argv)
{
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  t_uchar * output_dir = 0;
  t_uchar * changeset = 0;
  t_uchar * output_dir_dir = 0;
  t_uchar * output_dir_tail = 0;
  t_uchar * output_tmp_dir_tail = 0;
  t_uchar * output_tmp_dir = 0;
  struct revc_archives archives = REVC_ARCHIVES_NIL_INIT;

  struct revc_opts opts[] =
    {
      { REVC_ARG_OPT, "-o", "--output", 0, &output_dir },
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_get_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, errname);
      lim_free (0, changeset);
      if (revc_close_archives (errname, &archives))
        answer = 2;
      return answer;
    }

  if (argx == argc)
    {
      revc_usage (errname, revc_get_usage);
      goto bail;
    }

  changeset = str_save (0, argv[argx]);
  ++argx;

  if (argx != argc)
    {
      revc_usage (errname, revc_get_usage);
      goto bail;
    }

  if (!revc_is_valid_changeset_name (errname, changeset, 1))
    goto bail;

  if (!output_dir)
    {
      output_dir = str_save (0, changeset);
    }

  if (revc_init_archives (errname, &archives))
    goto bail;

  output_dir_dir = file_name_directory_file (0, output_dir);
  if (!output_dir_dir)
    output_dir_dir = str_save (0, ".");

  output_dir_tail = file_name_tail (0, output_dir);

  output_tmp_dir_tail = str_alloc_cat (0, ",,", output_dir_tail);

  output_tmp_dir = file_name_in_vicinity (0, output_dir_dir, output_tmp_dir_tail);

  revc_rmrf (errname, output_tmp_dir);

  if (revc_get (errname, &archives, changeset, output_tmp_dir))
    goto bail;

  if (revc_rename (errname, output_tmp_dir, output_dir))
    goto bail;

  answer = 0;

  goto leave;
}




/* arch-tag: Tom Lord Sun Jun 26 13:13:59 2005 (mains/get-main.c)
 */
