/* begin-revision-traversal.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "revc/archives/commit-ticket.h"
#include "revc/archives/get-blob.h"
#include "revc/api/revision-traversal-next.h"
#include "revc/api/revision-traversal-end.h"
#include "revc/api/begin-revision-traversal.h"

int
revc_begin_revision_traversal (const t_uchar * const errname,
                               struct revc_traversal ** trav,
                               struct revc_archives * archs,
                               t_uchar * revision)
{
  struct revc_revision_traversal * answer = 0;
  t_uchar fq_revname[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar ticket_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar revname[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar root_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar root_meta[REVC_MAX_METADATA_SIZE + 1];
  t_uchar prereqs_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar * prereqs = 0;
  size_t prereqs_size = 0;
  
  if (revc_commit_ticket (errname,
                          fq_revname, ticket_blob, revname, root_blob, root_meta, 0, 0, prereqs_blob, 0, 0, 0, 0, 0, 0,
                          archs, revision))
    return -1;

  if (0 >= revc_get_blob (errname, &prereqs, &prereqs_size,
                          archs, revision, 0, 0,
                          prereqs_blob,
                          0))
    return -1;

  answer = (struct revc_revision_traversal *)lim_malloc (0, sizeof (*answer));
  mem_set0 ((t_uchar *)answer, sizeof (*answer));

  answer->trav.node = str_save (0, ".");
  str_cpy (answer->trav.blob, root_blob);
  str_cpy (answer->trav.meta, root_meta);
  answer->trav.next = revc_revision_traversal_next;
  answer->trav.end = revc_revision_traversal_end;

  answer->archs = archs;
  answer->revision = str_save (0, revision);

  answer->prereqs = prereqs;
  prereqs = 0;

  answer->prereqs_size = prereqs_size;

  *trav = &answer->trav;

  return 0;
}



/* arch-tag: Tom Lord Sat Jul  2 18:21:23 2005 (begin-revision-traversal.c)
 */
