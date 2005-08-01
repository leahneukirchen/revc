/* unparsed.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__UNPARSED_H
#define INCLUDE__LIBAWIKI_NODES__UNPARSED_H


#include "awiki/libawiki/awiki.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_unparsed (void);
extern int awiki_parse_unparsed (t_awiki_node_type * node_type,
                                 ssize_t graph,
                                 ssize_t node,
                                 t_buffer_point * error_pt,
                                 t_buffer_point * start_pt,
                                 t_buffer_point * end_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES_UNPARSED_H */


/* arch-tag: Tom Lord Thu Oct 28 16:55:30 2004 (node-unparsed.h)
 */
