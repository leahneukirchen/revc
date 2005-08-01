/* lint-loop-main.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__CMDS__LINT_LOOP_MAIN_H
#define INCLUDE__CMDS__LINT_LOOP_MAIN_H


#include "hackerlab/machine/types.h"



typedef int (*t_revc_lint_callback_fn) (const t_uchar * const errname,
                                        const t_uchar * const path,
                                        size_t const path_size,
                                        const t_uchar * const cwd_path_from_root,
                                        void * rock);
     


/* automatically generated __STDC__ prototypes */
extern int revc_lint (const t_uchar * const errname,
                      int * unclean_ret,
                      const t_uchar * const path_from_root,
                      t_uchar * const manifest, size_t manifest_size,
                      t_uchar * const ignore, size_t ignore_size,
                      t_revc_lint_callback_fn new_callback,
                      t_revc_lint_callback_fn missing_callback,
                      void * rock);
#endif  /* INCLUDE__CMDS__LINT_LOOP_MAIN_H */


/* arch-tag: Tom Lord Fri Jun 24 10:50:47 2005 (api/lint.h)
 */
