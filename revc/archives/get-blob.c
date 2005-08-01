/* get-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/deps/error.h"
#include "revc/api/prereqs-skip.h"
#include "revc/api/nil-blob.h"
#include "revc/api/initial-prereqs-blob.h"
#include "revc/api/initial-ancestry-blob.h"
#include "revc/api/initial-ancestry.h"
#include "revc/api/initial-prereqs.h"
#include "revc/api/has-stashed-blob.h"
#include "revc/api/stashed-blob-to-str.h"
#include "revc/archives/find-revision.h"
#include "revc/archives/get-blob.h"


int
revc_get_blob (const t_uchar * const errname,
               t_uchar ** const str_ret,
               size_t * const size_ret,
               struct revc_archives * const archs,
               const t_uchar * const revision,
               t_uchar * const prereq_list,
               size_t const prereq_list_size,
               const t_uchar * const blob,
               int soft_errors)
{
  /* dangle: archive format */
  const t_uchar * const nil_blob = revc_nil_blob (errname);
  const t_uchar * const prereqs_blob = revc_initial_prereqs_blob (errname);
  const t_uchar * const ancestry_blob = revc_initial_ancestry_blob (errname);
  t_uchar * prereq_pos = 0;
  size_t prereq_remain = 0;
  int s;

  if (!nil_blob || !prereqs_blob)
    return -1;

  prereq_pos = prereq_list;
  prereq_remain = prereq_list_size;

  while (prereq_remain)
    {
      t_uchar * revdir = 0;
      int found;

      found = revc__find_revision (errname, &revdir, archs, prereq_pos);
      if (found < 0)
        return -1;

      /* dangle: should use blob hint optimization! */
      s = revc_has_stashed_blob (errname, revdir, blob);
      if (s < 0)
        {
          lim_free (0, revdir);
          return -1;
        }

      if (s)
        {
          s = revc_stashed_blob_to_str (errname, str_ret, size_ret, revdir, blob);
          if (s < 0)
            {
              lim_free (0, revdir);
              return -1;
            }
          else
            {
              lim_free (0, revdir);
              return 1;
            }
        }

      lim_free (0, revdir);

      if (revc_prereqs_skip (errname, &prereq_pos, &prereq_remain))
        return -1;
    }

  /* should really only be if `nil' is the revision or is in the prereqs list. */
  if (!str_cmp (blob, nil_blob))
    {
      if (str_ret)
        *str_ret = 0;
      if (size_ret)
        *size_ret = 0;
      return 1;
    }
  else if (!str_cmp (blob, prereqs_blob))
    {
      if (revc_initial_prereqs (errname, str_ret, size_ret))
        return -1;
      return 1;
    }
  else if (!str_cmp (blob, ancestry_blob))
    {
      if (revc_initial_ancestry (errname, str_ret, size_ret))
        return -1;
      return 1;
    }
  else if (soft_errors)
    return 0;
  else
    {
      revc_error (errname, "could not find blob\n  rev=%s\n  blob=%s\n", revision, blob);
      return -1;
    }
}





/* arch-tag: Tom Lord Mon Jun 27 12:51:30 2005 (archives/get-blob.c)
 */
