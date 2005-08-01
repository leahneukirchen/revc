/* abstract.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__ABSTRACT_H
#define INCLUDE__LIBAWIKI_NODES__ABSTRACT_H


#include "awiki/libawiki/awiki.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_abstract (void);
extern int awiki_parse_abstract (t_awiki_node_type * node_type,
                                 ssize_t graph,
                                 ssize_t node,
                                 t_buffer_point * error_pt,
                                 t_buffer_point * his_start_pt,
                                 t_buffer_point * his_end_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES__ABSTRACT_H */


/* arch-tag: Tom Lord Sat Nov 20 12:56:36 2004 (libawiki-nodes/abstract.h)
 */
