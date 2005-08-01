/* commit-ticket.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "hackerlab/char/char-class.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/error.h"
#include "revc/deps/file-stats.h"
#include "revc/deps/get-to-string.h"
#include "revc/api/constants.h"
#include "revc/api/zget-to-string.h"
#include "revc/api/nil-fq-revision.h"
#include "revc/api/nil-hq-revision.h"
#include "revc/api/nil-wq-revision.h"
#include "revc/api/nil-ticket-blob.h"
#include "revc/api/nil-blob.h"
#include "revc/api/string-blob.h"
#include "revc/api/initial-blob-hint.h"
#include "revc/api/initial-prereqs.h"
#include "revc/api/initial-prereqs-blob.h"
#include "revc/api/initial-ancestry-blob.h"
#include "revc/api/add-blob-to-hint-set.h"
#include "revc/archives/find-revision.h"
#include "revc/archives/commit-ticket.h"


/* __STDC__ prototypes for static functions */



static int scan_token (const t_uchar * const errname,
                       t_uchar ** pos, size_t * remain,
                       t_uchar * key);
static int scan_nl (const t_uchar * const errname,
                    t_uchar ** pos, size_t * remain);
static int blob_token (const t_uchar * const errname,
                       t_uchar * ret,
                       t_uchar ** pos, size_t * remain);
static int metadata_token (const t_uchar * const errname,
                           t_uchar * ret,
                           t_uchar ** pos, size_t * remain);
static int revname_token (const t_uchar * const errname,
                          t_uchar * ret,
                          t_uchar ** pos, size_t * remain);




int
revc_commit_ticket (const t_uchar * const errname,
                    t_uchar * const fq_revision_name_ret,
                    t_uchar * const ticket_blob_ret,
                    t_uchar * const revision_name_ret,
                    t_uchar * const root_blob_ret,
                    t_uchar * const root_metadata_ret,
                    t_uchar * const ancestry_blob_ret,
                    t_uchar * const ignored_blob_ret,
                    t_uchar * const prereqs_blob_ret,
                    t_uchar * const n_blobs_ret,
                    t_uchar * const file_size_ret,
                    t_uchar * const zip_size_ret,
                    t_uchar * const blob_blob_ret,
                    t_uchar ** const prereqs_ret,
                    size_t * const prereqs_size_ret,
                    struct revc_archives * const archs,
                    const t_uchar * const revision)
{
  /* dangle: deal with wq and fq revision names */

  int found;
  t_uchar ctblob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar tblob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar rev[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar fqrev[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar * archive_path = 0;
  t_uchar * commit_ticket_path = 0;
  t_uchar * prereqs_path = 0;
  int answer = -69;
  t_uchar * commit_ticket = 0;
  size_t commit_ticket_size = 0;

  found = revc__find_revision (errname, &archive_path, archs, revision);
  if (found < 0)
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, archive_path);
      lim_free (0, commit_ticket_path);
      lim_free (0, prereqs_path);
      lim_free (0, commit_ticket);
      return answer;
    }

  if (found != 0)
    {
      t_uchar * ct_pos;
      size_t ct_remain;

      commit_ticket_path = file_name_in_vicinity (0, archive_path, "commit-ticket");
      prereqs_path = file_name_in_vicinity (0, archive_path, "prereqs");

      if (revc_get_to_string (errname, &commit_ticket, &commit_ticket_size, commit_ticket_path))
        goto bail;

      if (revc_string_blob (errname, ctblob, commit_ticket, commit_ticket_size))
        goto bail;

      ct_pos = commit_ticket;
      ct_remain = commit_ticket_size;

      if (scan_token (errname, &ct_pos, &ct_remain, "ticket-blob:"))
        goto bail;

      if (blob_token (errname, tblob, &ct_pos, &ct_remain))
        goto bail;

      if (ticket_blob_ret)
        str_cpy (ticket_blob_ret, tblob);

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_token (errname, &ct_pos, &ct_remain, "revision:"))
        goto bail;

      if (revname_token (errname, rev, &ct_pos, &ct_remain))
        goto bail;

      if (revision_name_ret)
        str_cpy (revision_name_ret, rev);

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      /* dangle range check! */
      str_cpy (fqrev, rev);
      str_cat (fqrev, "/");
      str_cat (fqrev, ctblob);
      str_cat (fqrev, "+");
      str_cat (fqrev, tblob);
      
      if (fq_revision_name_ret)
        str_cpy (fq_revision_name_ret, fqrev);
      
      if (scan_token (errname, &ct_pos, &ct_remain, "root-blob:"))
        goto bail;

      if (blob_token (errname, root_blob_ret, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_token (errname, &ct_pos, &ct_remain, "root-metadata:"))
        goto bail;

      if (metadata_token (errname, root_metadata_ret, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_token (errname, &ct_pos, &ct_remain, "ancestry-blob:"))
        goto bail;

      if (blob_token (errname, ancestry_blob_ret, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_token (errname, &ct_pos, &ct_remain, "ignored-blob:"))
        goto bail;

      if (blob_token (errname, ignored_blob_ret, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_token (errname, &ct_pos, &ct_remain, "prereqs-blob:"))
        goto bail;

      if (blob_token (errname, prereqs_blob_ret, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_token (errname, &ct_pos, &ct_remain, "n-blobs:"))
        goto bail;

      if (blob_token (errname, n_blobs_ret, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_token (errname, &ct_pos, &ct_remain, "total-file-size:"))
        goto bail;

      if (blob_token (errname, file_size_ret, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_token (errname, &ct_pos, &ct_remain, "total-zip-size:"))
        goto bail;

      if (blob_token (errname, zip_size_ret, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_token (errname, &ct_pos, &ct_remain, "blob-contents-blob:"))
        goto bail;

      if (blob_token (errname, blob_blob_ret, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (scan_nl (errname, &ct_pos, &ct_remain))
        goto bail;

      if (prereqs_ret || prereqs_size_ret)
        {
          if (revc_zget_to_string (errname, prereqs_ret, prereqs_size_ret, prereqs_path))
            goto bail;
        }

      answer = 0;
      goto leave;
    }
  else if (   !str_casecmp (revision, REVC_NIL_REVNAME)
           || !str_casecmp (revision, revc_nil_fq_revision (errname))
           || !str_casecmp (revision, revc_nil_hq_revision (errname))
           || !str_casecmp (revision, revc_nil_wq_revision (errname)))
    {
      if (fq_revision_name_ret)
        {
          const t_uchar * const fq_nil = revc_nil_fq_revision (errname);

          if (!fq_nil)
            goto bail;

          str_cpy (fq_revision_name_ret,  fq_nil);
        }

      if (ticket_blob_ret)
        {
          if (revc_nil_ticket_blob (errname, ticket_blob_ret))
            goto bail;
        }

      if (revision_name_ret)
        {
          str_cpy (revision_name_ret,  "nil");
        }

      if (root_blob_ret)
        {
          const t_uchar * rb = revc_nil_blob (errname);
          if (!rb)
            goto bail;
          str_cpy (root_blob_ret, rb);
        }

      if (root_metadata_ret)
        {
          str_cpy (root_metadata_ret, REVC_DEFAULT_DIR_METADATA);
        }

      if (ignored_blob_ret)
        {
          const t_uchar * rb = revc_nil_blob (errname);
          if (!rb)
            goto bail;
          str_cpy (ignored_blob_ret, rb);
        }

      if (prereqs_blob_ret)
        {
          const t_uchar * rb = revc_initial_prereqs_blob (errname);
          if (!rb)
            goto bail;
          str_cpy (prereqs_blob_ret, rb);
        }

      if (ancestry_blob_ret)
        {
          const t_uchar * rb = revc_initial_ancestry_blob (errname);
          if (!rb)
            goto bail;
          str_cpy (ancestry_blob_ret, rb);
        }

      /* dangle: fix the following four _ret handlers.
       * (n_blobs, file_size, zip_size, blob_blob)
       */
      if (n_blobs_ret)
        *n_blobs_ret = 2;
      if (file_size_ret)
        {
          *file_size_ret = 0;
        }
      if (zip_size_ret)
        {
          *zip_size_ret = 0;
        }
      if (blob_blob_ret)
        {
        }

      if (prereqs_ret)
        {
          if (revc_initial_prereqs (errname, prereqs_ret, prereqs_size_ret))
            goto bail;
        }

      answer = 0;
      goto leave;
    }
  else
    {
      revc_error (errname, "revision not found (%s)\n", revision);
      goto bail;
    }
}
                    
static int
scan_token (const t_uchar * const errname,
            t_uchar ** pos, size_t * remain,
            t_uchar * key)
{
  while (*remain && *key && (**pos == *key))
    {
      ++key;
      ++*pos;
      --*remain;
    }

  if (!*key)
    return 0;
  else
    {
      revc_error (errname, "syntax error in commit ticket\n");
      return -1;
    }
}

static int
scan_nl (const t_uchar * const errname,
         t_uchar ** pos, size_t * remain)
{
  if (!*remain || (**pos != '\n'))
    {
      revc_error (errname, "syntax error in commit ticket\n");
      return -1;
    }

  ++*pos;
  --*remain;
  return 0;
}

static int
blob_token (const t_uchar * const errname,
            t_uchar * ret,
            t_uchar ** pos, size_t * remain)
{
  /* dangle range check */

  while (*remain && char_is_blank (**pos))
    {
      ++*pos;
      --*remain;
    }

  while (*remain && !char_is_space (**pos))
    {
      if (ret)
        *(ret++) = *((*pos)++);
      else
        ++*pos;
      --*remain;
    }

  if (ret)
    *(ret++) = 0;

  return 0;
}


static int
metadata_token (const t_uchar * const errname,
                t_uchar * ret,
                t_uchar ** pos, size_t * remain)
{
  /* dangle range check */

  while (*remain && char_is_blank (**pos))
    {
      ++*pos;
      --*remain;
    }

  while (*remain && !char_is_space (**pos))
    {
      if (ret)
        *(ret++) = *((*pos)++);
      else
        ++*pos;
      --*remain;
    }

  if (ret)
    *(ret++) = 0;

  return 0;
}


static int
revname_token (const t_uchar * const errname,
               t_uchar * ret,
               t_uchar ** pos, size_t * remain)
{
  /* dangle range check */

  while (*remain && char_is_blank (**pos))
    {
      ++*pos;
      --*remain;
    }

  while (*remain && !char_is_space (**pos))
    {
      if (ret)
        *(ret++) = *((*pos)++);
      else
        ++*pos;
      --*remain;
    }

  if (ret)
    *(ret++) = 0;

  return 0;
}



/* arch-tag: Tom Lord Mon Jun 27 08:52:37 2005 (commit-ticket.c)
 */
