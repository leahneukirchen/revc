/* list-find.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBREVC__LIST_FIND_H
#define INCLUDE__LIBREVC__LIST_FIND_H


#include "hackerlab/machine/types.h"


/* automatically generated __STDC__ prototypes */
extern int revc_list_find (const t_uchar * const errname,
                           t_uchar ** const answer,
                           t_uchar * const inv,
                           size_t const len,
                           t_uchar * name,
                           int exact,
                           int (*cmp) (const t_uchar * const a,
                                       const t_uchar * const b));
#endif  /* INCLUDE__LIBREVC__LIST_FIND_H */


/* arch-tag: Tom Lord Sat Jun 18 09:48:58 2005 (librevc/find-in-inventory.h)
 */
