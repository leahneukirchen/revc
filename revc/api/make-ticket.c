/* make-ticket.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str-many.h"
#include "hackerlab/char/str.h"
#include "revc/deps/error.h"
#include "revc/api/constants.h"
#include "revc/api/string-blob.h"
#include "revc/api/make-ticket.h"

int
revc_make_ticket (const t_uchar * const errname,
                  t_uchar * const wq_rev_ret,
                  t_uchar ** const ticket_ret,
                  const t_uchar * const revname,
                  const t_uchar * const root_blob,
                  const t_uchar * const root_metadata,
                  const t_uchar * const ancestry_blob)
{
  t_uchar * answer = 0;
  size_t revname_len = str_length_n (revname, REVC_MAX_REVNAME_SIZE + 1);
  t_uchar ticket_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];

  answer = str_alloc_cat_many (0,
                               "revision: ", revname, "\n",
                               "root-blob: ", root_blob, "\n",
                               "root-metadata: ", root_metadata, "\n",
                               "ancestry-blob: ", ancestry_blob, "\n",
                               "\n",
                               str_end);

  if (revc_string_blob (errname, ticket_blob, answer, str_length (answer)))
    {
    bail:
      lim_free (0, answer);
      return 0;
    }

  if ((revname_len + str_length (ticket_blob) + sizeof ("/+") - 1) > REVC_MAX_BLOB_ADDR_SIZE)
    {
      revc_error (errname, "revision name too long (%s)\n", revname);
      goto bail;
    }

  if (wq_rev_ret)
    {
      str_cpy (wq_rev_ret, revname);
      str_cat (wq_rev_ret, "/+");
      str_cat (wq_rev_ret, ticket_blob);
    }

  *ticket_ret = answer;
           
  return 0;
}




/* arch-tag: Tom Lord Mon Jul  4 13:05:40 2005 (make-commit-ticket.c)
 */
