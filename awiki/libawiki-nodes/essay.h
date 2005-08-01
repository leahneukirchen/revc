/* essay.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__ESSAY_H
#define INCLUDE__LIBAWIKI_NODES__ESSAY_H


#include "awiki/libawiki/awiki.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_essay_html (void);
extern int awiki_parse_essay (t_awiki_node_type * node_type,
                              ssize_t graph,
                              ssize_t node,
                              t_buffer_point * error_pt,
                              t_buffer_point * start_pt,
                              t_buffer_point * end_pt);
extern int awiki_essay_to_html (t_awiki_html_state * state,
                                t_awiki_html_type * type,
                                ssize_t graph,
                                ssize_t node,
                                t_buffer_point * error_pt,
                                t_buffer_point * output_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES__ESSAY_H */


/* arch-tag: Tom Lord Sun Oct 31 09:52:49 2004 (essay.h)
 */