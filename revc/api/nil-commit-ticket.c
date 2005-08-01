/* nil-commit-ticket.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/fmt/cvt.h"
#include "revc/api/constants.h"
#include "revc/api/string-blob.h"
#include "revc/api/nil-blob-serialization.h"
#include "revc/api/nil-ticket.h"
#include "revc/api/nil-commit-ticket.h"

t_uchar *
revc_nil_commit_ticket (const t_uchar * const errname)
{
  t_uchar * ticket = 0;
  t_uchar * answer = 0;
  t_uchar ticket_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar * nil_blob_serialization = 0;
  size_t nil_blob_serialization_size = 0;
  t_uchar serialization_size[sizeof (t_ulong) * 4 + 1];
  t_uchar serialization_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];

  ticket = revc_nil_ticket (errname);
  if (!ticket)
    {
    bail:
      answer = 0;
    leave:
      lim_free (0, ticket);
      lim_free (0, nil_blob_serialization);
      return answer;
    }

  if (revc_string_blob (errname, ticket_blob, ticket, str_length (ticket)))
    goto bail;

  ticket[str_length(ticket) - 1] = 0; /* kill the final newline */

  if (revc_nil_blob_serialization (errname, &nil_blob_serialization, &nil_blob_serialization_size))
    goto bail;

  cvt_ulong_to_decimal (serialization_size, (t_ulong)nil_blob_serialization_size);

  if (revc_string_blob (errname, serialization_blob, nil_blob_serialization, nil_blob_serialization_size))
    goto bail;

  answer = str_alloc_cat_many (0,
                               "ticket-blob: ", ticket_blob, "\n",
                               ticket,
                               "n-blobs: 1\n",
                               "serialization-size: ", serialization_size, "\n",
                               "serialization-blob: ", serialization_blob, "\n",
                               "\n",
                               str_end);

  goto leave;
}

                      



/* arch-tag: Tom Lord Mon Jun 27 09:38:50 2005 (api/nil-commit-ticket.c)
 */
