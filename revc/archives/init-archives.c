/* init-archives.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/os/stdlib.h"
#include "hackerlab/fs/file-names.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "revc/deps/error.h"
#include "revc/api/constants.h"
#include "revc/archives/init-archives.h"

int
revc_init_archives (const t_uchar * const errname,
                    struct revc_archives * archs)
{
  int x;
  t_uchar * const env_arch = getenv ("REVC_ARCHIVE");
  t_uchar * const home = getenv ("HOME");

  mem_set0 ((t_uchar *)archs, sizeof (*archs));

  for (x = 0; x < REVC_MAX_SIMULTANEOUS_COMMITS; ++x)
    archs->_commits[x].next = x - 1;

  archs->_first_free_commmit = (REVC_MAX_SIMULTANEOUS_COMMITS - 1);

  for (x = 0; x < REVC_MAX_SIMULTANEOUS_GETS; ++x)
    archs->_gets[x].next = x - 1;

  archs->_first_free_get = (REVC_MAX_SIMULTANEOUS_GETS - 1);

  if (env_arch)
    archs->_archive_dir = str_save (0, env_arch);
  else if (home)
    archs->_archive_dir = file_name_in_vicinity (0, home, ".revc-archive");
  else
    {
      revc_error (errname, "no $REVC_ARCHIVE or $HOME set?\n");
      return -1;
    }

  return 0;
}



/* arch-tag: Tom Lord Sun Jun 26 17:16:45 2005 (init-archives.c)
 */
