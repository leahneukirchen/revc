/* is-better-revdir-match.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/archives/is-better-revdir-match.h"

int
revc__is_better_revdir_match (const t_uchar * const rev,
                              const t_uchar * const filename,
                              const t_uchar * const best_so_far)
{
  const t_uchar * const rev_slash = str_chr_index (rev, '/');
  const t_uchar * const rev_plus = (!rev_slash ? 0 : str_chr_index (rev_slash, '+'));
  const t_uchar * const ct_blob_start = ((!rev_slash || ((rev_slash + 1) == rev_plus)) ? 0 : rev_slash + 1);
  const t_uchar * const ct_blob_end = (!rev_slash ? 0 : (rev_plus ? rev_plus : rev + str_length (rev)));
  const t_uchar * const t_blob_start = (!rev_plus ? 0 : rev_plus + 1);
  const t_uchar * const t_blob_end = (!rev_plus ? 0 : (rev + str_length (rev)));

  const t_uchar * const file_plus = str_chr_index (filename, '+');
  const t_uchar * const file_ct_blob_start = filename + 1;
  const t_uchar * const file_ct_blob_end = file_plus;
  const t_uchar * const file_t_blob_start = file_plus + 1;
  const t_uchar * const file_t_blob_end = (filename + str_length (filename));

  if (!file_plus || (filename[0] != '.'))
    return 0;

  if (t_blob_start
      && str_cmp_n (file_t_blob_start, (file_t_blob_end - file_t_blob_start),
                    t_blob_start, (t_blob_end - t_blob_start)))
    return 0;

  if (ct_blob_start
      && str_cmp_n (file_ct_blob_start, (file_ct_blob_end - file_ct_blob_start),
                    ct_blob_start, (ct_blob_end - ct_blob_start)))
    return 0;

  if (ct_blob_start || !best_so_far)
    return 1;

  return 0;
}




/* arch-tag: Tom Lord Fri Jul 15 08:34:47 2005 (is-better-revdir-match.c)
 */
