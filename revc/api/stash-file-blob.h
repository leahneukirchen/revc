/* stash-file-blob.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__STASH_FILE_BLOB_H
#define INCLUDE__API__STASH_FILE_BLOB_H


#include "hackerlab/machine/types.h"


/* automatically generated __STDC__ prototypes */
extern int revc_stash_file_blob (const t_uchar * const errname,
                                 size_t * file_size_ret,
                                 size_t * zip_size_ret,
                                 const t_uchar * const dir,
                                 const t_uchar * const blob,
                                 const t_uchar * const path);
#endif  /* INCLUDE__API__STASH_FILE_BLOB_H */


/* arch-tag: Tom Lord Sun Jul  3 09:52:14 2005 (api/stash-file-blob.h)
 */
