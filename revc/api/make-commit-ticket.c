/* make-commit-ticket.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/fmt/cvt.h"
#include "hackerlab/char/str-many.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/error.h"
#include "revc/api/constants.h"
#include "revc/api/string-blob.h"
#include "revc/api/make-commit-ticket.h"

int
revc_make_commit_ticket (const t_uchar * const errname,
                         t_uchar * const fq_rev_ret,
                         t_uchar ** const commit_ticket_ret,
                         const t_uchar * const revname,
                         const t_uchar * const ticket_blob,
                         const t_uchar * const root_blob,
                         const t_uchar * const root_metadata,
                         const t_uchar * const ancestry_blob,
                         const t_uchar * const ignored_blob,
                         const t_uchar * const prereqs_blob,
                         size_t n_blobs,
                         size_t const file_size,
                         size_t const zip_size,
                         const t_uchar * const blob_contents_blob)
{
  t_uchar * answer = 0;
  size_t revname_len = str_length_n (revname, REVC_MAX_REVNAME_SIZE + 1);
  t_uchar commit_ticket_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar n_blobs_str[4 * sizeof (t_ulong) + 1];
  t_uchar zip_size_str[4 * sizeof (t_ulong) + 1];
  t_uchar file_size_str[4 * sizeof (t_ulong) + 1];

  cvt_ulong_to_decimal (n_blobs_str, (t_ulong)n_blobs);

  cvt_ulong_to_decimal (zip_size_str, (t_ulong)zip_size);

  cvt_ulong_to_decimal (file_size_str, (t_ulong)file_size);

  answer = str_alloc_cat_many (0,
                               "ticket-blob: ", ticket_blob, "\n",
                               "revision: ", revname, "\n",
                               "root-blob: ", root_blob, "\n",
                               "root-metadata: ", root_metadata, "\n",
                               "ancestry-blob: ", ancestry_blob, "\n",
                               "ignored-blob: ", ignored_blob, "\n",
                               "prereqs-blob: ", prereqs_blob, "\n",
                               "n-blobs: ", n_blobs_str, "\n",
                               "total-file-size: ", file_size_str, "\n",
                               "total-zip-size: ", zip_size_str, "\n",
                               "blob-contents-blob: ", blob_contents_blob, "\n",
                               "\n",
                               str_end);

  if (revc_string_blob (errname, commit_ticket_blob, answer, str_length (answer)))
    {
    bail:
      lim_free (0, answer);
      return 0;
    }

  if ((revname_len + str_length (ticket_blob) + str_length (commit_ticket_blob) + sizeof ("/+") - 1) > REVC_MAX_BLOB_ADDR_SIZE)
    {
      revc_error (errname, "revision name too long (%s)\n", revname);
      goto bail;
    }

  if (fq_rev_ret)
    {
      str_cpy (fq_rev_ret, revname);
      str_cat (fq_rev_ret, "/");
      str_cat (fq_rev_ret, commit_ticket_blob);
      str_cat (fq_rev_ret, "+");
      str_cat (fq_rev_ret, ticket_blob);
    }

  *commit_ticket_ret = answer;
           
  return 0;
}




/* arch-tag: Tom Lord Thu Jul  7 12:54:44 2005 (api/make-commit-ticket.c)
 */
