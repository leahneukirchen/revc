/* outline.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__OUTLINE_H
#define INCLUDE__LIBAWIKI_NODES__OUTLINE_H


#include "awiki/libawiki/awiki.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_outline (void);
extern int awiki_parse_outline (t_awiki_node_type * node_type,
                                ssize_t graph,
                                ssize_t node,
                                t_buffer_point * error_pt,
                                t_buffer_point * start_pt,
                                t_buffer_point * end_pt);
extern int awiki_parse_outline_section (t_awiki_node_type * node_type,
                                        ssize_t graph,
                                        ssize_t node,
                                        t_buffer_point * error_pt,
                                        t_buffer_point * start_pt,
                                        t_buffer_point * end_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES__OUTLINE_H */


/* arch-tag: Tom Lord Thu Nov 18 19:52:17 2004 (libawiki-nodes/outline.h)
 */
