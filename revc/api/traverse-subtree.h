/* traverse-subtree.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBREVC__TRAVERSE_SUBTREE_H
#define INCLUDE__LIBREVC__TRAVERSE_SUBTREE_H


#include "hackerlab/machine/types.h"



typedef int (*t_revc_traverse_fn) (const t_uchar * const errname,
                                   void * closure,
                                   const t_uchar * const parent_path_from_root,
                                   const t_uchar * const path_from_root,
                                   const t_uchar * const fname,
                                   t_uchar ** const mfst, size_t * const mlen,
                                   t_uchar ** const ign, size_t * const ilen);




/* automatically generated __STDC__ prototypes */
extern int revc_traverse_subtree (const t_uchar * const errname,
                                  const t_uchar * const path_from_root,
                                  t_revc_traverse_fn fn,
                                  void * closure,
                                  t_uchar ** const mfst, size_t * mpos,
                                  t_uchar ** const ign, size_t * ipos);
#endif  /* INCLUDE__LIBREVC__TRAVERSE_SUBTREE_H */


/* arch-tag: Tom Lord Thu Jun 16 19:17:18 2005 (traverse-subtree.h)
 */
