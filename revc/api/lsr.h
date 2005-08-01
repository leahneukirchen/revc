/* lsr.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBREVC__LSR_H
#define INCLUDE__LIBREVC__LSR_H


#include "hackerlab/machine/types.h"

typedef int (*t_revc_lsr_fn) (const t_uchar * const errname,
                              const t_uchar * const path_from_root,
                              size_t const path_size,
                              const t_uchar * const cwd_path_from_root,
                              void * const rock);


/* automatically generated __STDC__ prototypes */
extern int revc_lsr (const t_uchar * const errname,
                     t_revc_lsr_fn const callback,
                     void * const rock);
#endif  /* INCLUDE__LIBREVC__LSR_H */


/* arch-tag: Tom Lord Thu Jun 23 09:57:18 2005 (lsr.h)
 */
