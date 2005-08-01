/* file-blob-commiting.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__FILE_BLOB_COMMITING_H
#define INCLUDE__API__FILE_BLOB_COMMITING_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_file_blob_committing (const t_uchar * const errname,
                                      t_uchar * const blob_ret,
                                      struct revc_archives * const archs,
                                      int const txn_n,
                                      const t_uchar * const path,
                                      size_t size);
#endif  /* INCLUDE__API__FILE_BLOB_COMMITING_H */


/* arch-tag: Tom Lord Sat Jul  2 10:10:45 2005 (file-blob-commiting.h)
 */
