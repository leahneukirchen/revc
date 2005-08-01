/* build-node.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__BUILD_NODE_H
#define INCLUDE__API__BUILD_NODE_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_build_node (const t_uchar * const errname,
                            t_uchar ** const manifest,
                            size_t * manifest_size,
                            struct revc_archives * archs,
                            const t_uchar * const fq_rev,
                            t_uchar * const prereq_list,
                            size_t const prereq_list_size,
                            const t_uchar * const logical_path,
                            const t_uchar * const output_node,
                            const t_uchar * const blob,
                            const t_uchar * const metadata);
#endif  /* INCLUDE__API__BUILD_NODE_H */


/* arch-tag: Tom Lord Mon Jun 27 12:21:28 2005 (build-node.h)
 */
