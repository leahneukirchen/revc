/* find-revision.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/arrays/ar.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/file-is-dir.h"
#include "revc/deps/list.h"
#include "revc/deps/free-dir-list.h"
#include "revc/archives/revision-path.h"
#include "revc/archives/is-better-revdir-match.h"
#include "revc/archives/find-revision.h"

int
revc__find_revision (const t_uchar * const errname,
                     t_uchar ** const ret,
                     struct revc_archives * const archs,
                     const t_uchar * const revision)
{
  int answer = -69;
  t_uchar * revision_dir = 0;
  t_uchar * best = 0;
  t_uchar ** dir_list = 0;
  int stat;
  size_t x;

  revision_dir = revc__revision_path (errname, archs->_archive_dir, revision);

  if (!revision_dir)
    {
    bail:
      answer = -1;
    leave:
      if ((answer >= 0) && ret)
        *ret = file_name_in_vicinity (0, revision_dir, best);
      else
        lim_free (0, best);
      
      revc_free_dir_list (&dir_list);

      return answer;
    }


  stat = revc_file_is_dir (errname, revision_dir);
  if (stat < 0)
    goto bail;

  if (!stat)
    {
      answer = 0;
      goto leave;
    }

  if (0 > revc_list (errname, &dir_list, revision_dir))
    goto bail;

  for (x = 0; x < ar_size ((void *)dir_list, 0, sizeof (t_uchar *)); ++x)
    {
      if (revc__is_better_revdir_match (revision, dir_list[x], best))
        {
          lim_free (0, best);
          best = str_save (0, dir_list[x]);
        }
    }

  answer = (best ? 1 : 0);
  goto leave;
}



/* arch-tag: Tom Lord Thu Jul 14 11:41:38 2005 (find-revision.c)
 */
