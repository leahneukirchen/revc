/* file-stats.h: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/os/sys/stat.h"
#include "hackerlab/os/errno.h"
#include "hackerlab/os/errno-to-string.h"
#include "hackerlab/fmt/cvt.h"
#include "hackerlab/vu/vu.h"
#include "revc/deps/error.h"
#include "revc/deps/file-stats.h"

int
revc_file_stats (const t_uchar * const errname,
                 t_uchar * const type_ret,
                 size_t * const size_ret,
                 t_uchar * metadata_ret,
                 const t_uchar * const path)
{
  int errn;
  struct stat statb;

  if (vu_stat (&errn, path, &statb))
    {
      if (errn == ENOENT)
        {
          if (size_ret)
            *size_ret = 0;
          if (type_ret)
            *type_ret = 0;
          return 0;
        }
      else
        {
          revc_error (errname, "i/o error getting file stats (%s)\n  file: %s\n", errno_to_string (errn), path);
          return -1;
        }
    }

  if (size_ret)
    *size_ret = statb.st_size;

  if (type_ret)
    {
      if (S_ISDIR (statb.st_mode))
        *type_ret = 'd';
      else if (S_ISREG (statb.st_mode))
        *type_ret = 'f';
      else
        {
          revc_error (errname, "unhandled file type (%s)\n", path);
          return -1;
        }
    }

  if (metadata_ret)
    {
      size_t amt;

      if (S_ISDIR (statb.st_mode))
        *metadata_ret = 'd';
      else if (S_ISREG (statb.st_mode))
        *metadata_ret = 'f';

      metadata_ret[1] = ((statb.st_mode & S_ISUID) ? 'u' : '-');
      metadata_ret[2] = ((statb.st_mode & S_ISGID) ? 'g' : '-');
#ifdef S_ISVTX
      metadata_ret[3] = ((statb.st_mode & S_ISVTX) ? 'g' : '-');
#else
      metadata_ret[3] = '-';
#endif

      metadata_ret[4] = ((statb.st_mode & S_IRUSR) ? 'r' : '-');
      metadata_ret[5] = ((statb.st_mode & S_IWUSR) ? 'w' : '-');
      metadata_ret[6] = ((statb.st_mode & S_IXUSR) ? 'x' : '-');

      metadata_ret[7] = ((statb.st_mode & S_IRGRP) ? 'r' : '-');
      metadata_ret[8] = ((statb.st_mode & S_IWGRP) ? 'w' : '-');
      metadata_ret[9] = ((statb.st_mode & S_IXGRP) ? 'x' : '-');

      metadata_ret[10] = ((statb.st_mode & S_IROTH) ? 'r' : '-');
      metadata_ret[11] = ((statb.st_mode & S_IWOTH) ? 'w' : '-');
      metadata_ret[12] = ((statb.st_mode & S_IXOTH) ? 'x' : '-');

      metadata_ret[13] = '.';

      amt = cvt_ulong_to_decimal (metadata_ret + 14, (t_ulong)statb.st_uid);
      metadata_ret[14 + amt] = '.';

      (void)cvt_ulong_to_decimal (metadata_ret + 15 + amt, (t_ulong)statb.st_gid);
    }

  return 0;
}



/* arch-tag: Tom Lord Thu Jun 30 15:48:31 2005 (file-stats.h)
 */
