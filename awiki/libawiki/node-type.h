/* node-type.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__NODE_TYPE_H
#define INCLUDE__LIBAWIKI__NODE_TYPE_H


#include "hackerlab/machine/types.h"



/* automatically generated __STDC__ prototypes */
extern t_uchar * awiki_node_type (ssize_t graph,
                                  ssize_t node);
extern int awiki_set_node_type (ssize_t graph,
                                ssize_t node,
                                const t_uchar * type_atom);
#endif  /* INCLUDE__LIBAWIKI__NODE_TYPE_H */


/* arch-tag: Tom Lord Sun Nov 21 11:38:50 2004 (node-type.h)
 */
