/* tree-setup.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__TREE_SETUP_H
#define INCLUDE__API__TREE_SETUP_H


#include "hackerlab/machine/types.h"



/* automatically generated __STDC__ prototypes */
extern int revc_tree_setup  (const t_uchar * const errname,
                             t_uchar ** const path_to_root_ret,
                             t_uchar ** const path_from_root_ret,
                             t_uchar ** const path_to_manifest_file_ret,
                             t_uchar ** const path_to_manifest_tmp_file_ret,
                             t_uchar ** const path_to_ignore_file_ret,
                             t_uchar ** const path_to_ignore_tmp_file_ret,
                             t_uchar ** const path_to_ancestry_file_ret,
                             t_uchar ** const path_to_ancestry_tmp_file_ret,
                             t_uchar ** const path_to_prereqs_file_ret,
                             t_uchar ** const path_to_prereqs_tmp_file_ret,
                             t_uchar ** const manifest_list_ret,
                             size_t  *  const manifest_list_size_ret,
                             t_uchar ** const ignore_list_ret,
                             size_t  *  const ignore_list_size_ret,
                             t_uchar ** const ancestry_list_ret,
                             size_t  *  const ancestry_list_size_ret,
                             t_uchar ** const prereqs_list_ret,
                             size_t  *  const prereqs_list_size_ret);
#endif  /* INCLUDE__API__TREE_SETUP_H */


/* arch-tag: Tom Lord Mon Jun 27 14:32:45 2005 (api/tree-setup.h)
 */
