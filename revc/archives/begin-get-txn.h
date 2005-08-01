/* begin-get-txn.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__ARCHIVES__BEGIN_GET_TXN_H
#define INCLUDE__ARCHIVES__BEGIN_GET_TXN_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_begin_get_txn (const t_uchar * const errname,
                               struct revc_archives * const archs,
                               const t_uchar * const revname,
                               const t_uchar * const prereqs,
                               size_t prereqs_size);
#endif  /* INCLUDE__ARCHIVES__BEGIN_GET_TXN_H */


/* arch-tag: Tom Lord Sat Jul  2 17:29:23 2005 (begin-get-txn.h)
 */
