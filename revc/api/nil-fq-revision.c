/* nil-fq-revision.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/api/constants.h"
#include "revc/api/nil-commit-ticket-blob.h"
#include "revc/api/nil-ticket-blob.h"
#include "revc/api/nil-fq-revision.h"

const t_uchar *
revc_nil_fq_revision (const t_uchar * const errname)
{
  static int initialized = 0;
  static t_uchar answer[REVC_MAX_REVNAME_SIZE + 1];

  if (!initialized)
    {
      str_cpy (answer, "nil/");
      if (revc_nil_commit_ticket_blob (errname, answer + 4))
        return 0;
      str_cat (answer, "+");
      if (revc_nil_ticket_blob (errname, answer + str_length (answer)))
        return 0;
    }

  return answer;
}



/* arch-tag: Tom Lord Mon Jun 27 11:10:46 2005 (nil-fq-revision.c)
 */
