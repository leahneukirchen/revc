/* saying.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__SAYING_H
#define INCLUDE__LIBAWIKI_NODES__SAYING_H


#include "awiki/libawiki/awiki.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_saying (void);
extern int awiki_parse_saying (t_awiki_node_type * node_type,
                               ssize_t graph,
                               ssize_t node,
                               t_buffer_point * error_pt,
                               t_buffer_point * his_start_pt,
                               t_buffer_point * his_end_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES__SAYING_H */


/* arch-tag: Tom Lord Mon Nov 22 12:29:49 2004 (libawiki-nodes/saying.h)
 */