/* commit-needs-str-blob.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__ARCHIVES__COMMIT_NEEDS_STR_BLOB_H
#define INCLUDE__ARCHIVES__COMMIT_NEEDS_STR_BLOB_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_commit_needs_str_blob (const t_uchar * const errname,
                                       struct revc_archives * archs,
                                       int txn_n,
                                       const t_uchar * const blob,
                                       const t_uchar * const data,
                                       size_t const size);
#endif  /* INCLUDE__ARCHIVES__COMMIT_NEEDS_STR__BLOB_H */


/* arch-tag: Tom Lord Sun Jul  3 09:36:50 2005 (commit-needs-blob.h)
 */
