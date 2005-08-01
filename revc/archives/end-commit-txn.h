/* end-commit-txn.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__ARCHIVES__END_COMMIT_TXN_H
#define INCLUDE__ARCHIVES__END_COMMIT_TXN_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"



/* automatically generated __STDC__ prototypes */
extern int revc_end_commit_txn (const t_uchar * const errname,
                                struct revc_archives * const archs,
                                int const txn_n,
                                const t_uchar * const revision_name,
                                const t_uchar * const ticket_blob,
                                const t_uchar * const commit_ticket_blob);
#endif  /* INCLUDE__ARCHIVES__END_COMMIT_TXN_H */


/* arch-tag: Tom Lord Tue Jun 28 09:05:38 2005 (archives/end-commit-txn.h)
 */
