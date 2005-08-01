/* commit.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__COMMIT_H
#define INCLUDE__API__COMMIT_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


struct commit_state
{
  t_uchar ** blob_index_lines;
  size_t n_blobs;
  size_t total_blob_size;
};





/* automatically generated __STDC__ prototypes */
extern int revc_commit (const t_uchar * const errname,
                        struct revc_archives * const archs,
                        const t_uchar * const revname,
                        const t_uchar * const path_to_root,
                        const t_uchar * const path_from_root,
                        t_uchar * const manifest,
                        size_t manifest_size,
                        t_uchar * const ignore,
                        size_t ignore_size,
                        t_uchar * const ancestry,
                        size_t const ancestry_size,
                        t_uchar * const prereqs,
                        size_t const prereqs_size,
                        t_uchar * const selected_subtrees,
                        size_t const selected_subtrees_size);
#endif  /* INCLUDE__API__COMMIT_H */


/* arch-tag: Tom Lord Mon Jun 27 17:52:41 2005 (commit.h)
 */
