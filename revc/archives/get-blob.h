/* get-blob.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__ARCHIVES__GET_BLOB_H
#define INCLUDE__ARCHIVES__GET_BLOB_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_get_blob (const t_uchar * const errname,
                          t_uchar ** const str_ret,
                          size_t * const size_ret,
                          struct revc_archives * const archs,
                          const t_uchar * const revision,
                          t_uchar * const prereq_list,
                          size_t const prereq_list_size,
                          const t_uchar * const blob,
                          int soft_errors);
#endif  /* INCLUDE__ARCHIVES__GET_BLOB_H */


/* arch-tag: Tom Lord Mon Jun 27 12:51:24 2005 (archives/get-blob.h)
 */
