/* tty.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__TTY_H
#define INCLUDE__LIBAWIKI_NODES__TTY_H


#include "awiki/libawiki/awiki.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_tty (void);
extern int awiki_parse_tty (t_awiki_node_type * node_type,
                            ssize_t graph,
                            ssize_t node,
                            t_buffer_point * error_pt,
                            t_buffer_point * start_pt,
                            t_buffer_point * end_pt);
extern int awiki_tty_to_html (t_awiki_html_state * state,
                              t_awiki_html_type * type,
                              ssize_t graph,
                              ssize_t node,
                              t_buffer_point * error_pt,
                              t_buffer_point * output_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES__TTY_H */


/* arch-tag: Tom Lord Thu Nov 18 18:44:30 2004 (libawiki-nodes/tty.h)
 */
