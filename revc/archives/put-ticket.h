/* put-ticket.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__ARCHIVES__PUT_TICKET_H
#define INCLUDE__ARCHIVES__PUT_TICKET_H


#include "hackerlab/machine/types.h"
#include "revc/archives/archives.h"


/* automatically generated __STDC__ prototypes */
extern int revc_put_ticket (const t_uchar * const errname,
                            struct revc_archives * archs,
                            int txn_n,
                            const t_uchar * const ticket);
#endif  /* INCLUDE__ARCHIVES__PUT_TICKET_H */


/* arch-tag: Tom Lord Mon Jul  4 13:24:15 2005 (put-commit-ticket.h)
 */
