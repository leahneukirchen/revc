/* make-ticket.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__MAKE_TICKET_H
#define INCLUDE__API__MAKE_TICKET_H


#include "hackerlab/machine/types.h"


/* automatically generated __STDC__ prototypes */
extern int revc_make_ticket (const t_uchar * const errname,
                             t_uchar * const wq_rev_ret,
                             t_uchar ** const ticket_ret,
                             const t_uchar * const revname,
                             const t_uchar * const root_blob,
                             const t_uchar * const root_metadata,
                             const t_uchar * const ancestry_blob);
#endif  /* INCLUDE__API__MAKE_TICKET_H */


/* arch-tag: Tom Lord Mon Jul  4 13:08:28 2005 (make-commit-ticket.h)
 */
