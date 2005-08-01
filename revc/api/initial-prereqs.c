/* initial-prereqs.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "revc/api/constants.h"
#include "revc/api/nil-wq-revision.h"
#include "revc/api/initial-blob-hint.h"
#include "revc/api/initial-prereqs.h"

int
revc_initial_prereqs (const t_uchar * const errname,
                      t_uchar ** const prereqs_ret,
                      size_t * const size_ret)
{
  const t_uchar * const nilrev = revc_nil_wq_revision (errname);
  size_t const nilrev_len = str_length (nilrev);
  size_t answer_size =  nilrev_len + 1 + REVC_BLOB_HINT_SET_SIZE;
  t_uchar * answer = 0;

  if (!nilrev)
    return -1;

  answer = lim_malloc (0, answer_size);
  mem_move (answer, nilrev, nilrev_len + 1);
  mem_set0 (answer + nilrev_len + 1, REVC_BLOB_HINT_SET_SIZE);

  if (revc_initial_blob_hint (errname, answer + nilrev_len + 1))
    {
      lim_free (0, answer);
      return -1;
    }

  *prereqs_ret = answer;
  *size_ret = answer_size;

  return 0;
}




/* arch-tag: Tom Lord Mon Jun 27 09:33:35 2005 (initial-prereqs.c)
 */
