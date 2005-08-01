/* revision-path.c: 
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
#include "revc/archives/revision-path.h"


t_uchar *
revc__revision_path (const t_uchar * const errname,
                     const t_uchar * const archive_root,
                     const t_uchar * const rev)
{
  const t_uchar * rev_slash = str_chr_index (rev, '/');
  const t_uchar * uqrev_end = (rev_slash ? rev_slash : (rev + str_length (rev)));
  size_t uqrev_size = (uqrev_end - rev);
  const t_uchar * revcat_end = 0;
  size_t revcat_size;
  t_uchar * cat = 0;
  t_uchar * uqrev = 0;
  t_uchar * cat_dir = 0;
  t_uchar * rev_dir = 0;

  {
    const t_uchar * best = 0;
    const t_uchar * pos = 0;

    for (pos = rev; pos < uqrev_end; ++pos)
      {
        if (!best && (*pos == '-'))
          best = pos;

        if ((*pos == '.')
            || ((*pos == '-') && (pos[1] == '-')))
          {
            best = pos;
            break;
          }
      }

    revcat_end = pos;
    revcat_size = (revcat_end - rev);
  }

  if (!revcat_end || (revcat_end == rev))
    cat = str_save (0, "MISC");
  else
    cat = str_save_n (0, rev, revcat_size);

  uqrev = str_save_n (0, rev, uqrev_size);

  cat_dir = file_name_in_vicinity (0, archive_root, cat);

  rev_dir = file_name_in_vicinity (0, cat_dir, uqrev);

  lim_free (0, cat);
  lim_free (0, uqrev);
  lim_free (0, cat_dir);

  return rev_dir;
}





/* arch-tag: Tom Lord Fri Jul 15 08:48:04 2005 (revision-path.c)
 */
