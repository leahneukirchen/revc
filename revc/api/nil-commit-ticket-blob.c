/* nil-commit-ticket-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "revc/api/string-blob.h"
#include "revc/api/nil-commit-ticket.h"
#include "revc/api/nil-commit-ticket-blob.h"

int
revc_nil_commit_ticket_blob (const t_uchar * const errname,
                             t_uchar * const blob_ret)
{
  t_uchar * ticket = 0;
  int answer;

  ticket = revc_nil_commit_ticket (errname);
  if (!ticket)
    return -1;

  answer = revc_string_blob (errname, blob_ret, ticket, str_length (ticket));

  lim_free (0, ticket);

  return answer;
}






/* arch-tag: Tom Lord Mon Jun 27 10:26:49 2005 (api/nil-commit-ticket-blob.c)
 */
