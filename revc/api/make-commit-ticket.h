/* make-commit-ticket.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__MAKE_COMMIT_TICKET_H
#define INCLUDE__API__MAKE_COMMIT_TICKET_H


#include "hackerlab/machine/types.h"


/* automatically generated __STDC__ prototypes */
extern int revc_make_commit_ticket (const t_uchar * const errname,
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
                                    const t_uchar * const blob_contents_blob);
#endif  /* INCLUDE__API__MAKE_COMMIT_TICKET_H */


/* arch-tag: Tom Lord Thu Jul  7 12:54:37 2005 (api/make-commit-ticket.h)
 */
