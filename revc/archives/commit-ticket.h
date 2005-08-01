/* commit-ticket.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__ARCHIVES__COMMIT_TICKET_H
#define INCLUDE__ARCHIVES__COMMIT_TICKET_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_commit_ticket (const t_uchar * const errname,
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
                               const t_uchar * const revision);
#endif  /* INCLUDE__ARCHIVES__COMMIT_TICKET_H */


/* arch-tag: Tom Lord Mon Jun 27 08:52:40 2005 (commit-ticket.h)
 */
