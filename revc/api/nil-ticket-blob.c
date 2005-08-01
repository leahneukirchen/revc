/* nil-ticket-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/api/string-blob.h"
#include "revc/api/nil-ticket.h"
#include "revc/api/nil-ticket-blob.h"

int
revc_nil_ticket_blob (const t_uchar * const errname,
                      t_uchar * const blob_ret)
{
  t_uchar * ticket = 0;
  int answer;

  ticket =  revc_nil_ticket (errname);
  if (!ticket)
    return -1;

  answer = revc_string_blob (errname, blob_ret, ticket, str_length (ticket));

  lim_free (0, ticket);

  return answer;
}
                      





/* arch-tag: Tom Lord Mon Jun 27 10:15:18 2005 (nil-ticket-blob.c)
 */
