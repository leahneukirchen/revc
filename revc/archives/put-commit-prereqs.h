/* put-commit-prereqs.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__ARCHIVES__PUT_COMMIT_PREREQS_H
#define INCLUDE__ARCHIVES__PUT_COMMIT_PREREQS_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_put_commit_prereqs (const t_uchar * const errname,
                                    struct revc_archives * archs,
                                    int txn_n,
                                    const t_uchar * const blob,
                                    const t_uchar * const prereqs,
                                    size_t const prereqs_size);
#endif  /* INCLUDE__ARCHIVES__PUT_COMMIT_PREREQS_H */


/* arch-tag: Tom Lord Thu Jul  7 11:04:09 2005 (archives/put-commit-prereqs.h)
 */
