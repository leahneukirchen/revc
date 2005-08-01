/* pathlist-from-argv.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__PATHLIST_FROM_ARGV_H
#define INCLUDE__API__PATHLIST_FROM_ARGV_H


#include "hackerlab/machine/types.h"


/* automatically generated __STDC__ prototypes */
extern int revc_pathlist_from_argv (const t_uchar * const errname,
                                    t_uchar ** const list_ret,
                                    size_t * const list_size_ret,
                                    const t_uchar * const path_from_root,
                                    int argc, char ** argv);
#endif  /* INCLUDE__API__PATHLIST_FROM_ARGV_H */


/* arch-tag: Tom Lord Thu Jun 23 14:10:42 2005 (pathlist-from-argv.h)
 */
