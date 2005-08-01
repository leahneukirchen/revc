/* save-dir.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__DEPS__SAVE_DIR_H
#define INCLUDE__DEPS__SAVE_DIR_H


#include "hackerlab/machine/types.h"

typedef int t_revc_cwd_buffer;

#define REVC_CWD_BUFFER_INIT -1


/* automatically generated __STDC__ prototypes */
extern int revc_save_dir (const t_uchar * const errname,
                          t_revc_cwd_buffer * const buffer);
#endif  /* INCLUDE__DEPS__SAVE_DIR_H */


/* arch-tag: Tom Lord Thu Jun 23 12:20:12 2005 (deps/save-dir.h)
 */
