/* commit-zip-size.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__ARCHIVES__COMMIT_ZIP_SIZE_H
#define INCLUDE__ARCHIVES__COMMIT_ZIP_SIZE_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_commit_zip_size (const t_uchar * const errname,
                                 size_t * zsize_ret,
                                 struct revc_archives * archs,
                                 int txn_n);
#endif  /* INCLUDE__ARCHIVES__COMMIT_ZIP_SIZE_H */


/* arch-tag: Tom Lord Sat Jul  9 09:21:59 2005 (commit-zip-size.h)
 */
