/* paragraph.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__PARAGRAPH_H
#define INCLUDE__LIBAWIKI_NODES__PARAGRAPH_H


#include "awiki/libawiki/awiki.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_paragraph (void);
extern int awiki_parse_paragraph (t_awiki_node_type * node_type,
                                  ssize_t graph,
                                  ssize_t node,
                                  t_buffer_point * error_pt,
                                  t_buffer_point * start_pt,
                                  t_buffer_point * end_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES__PARAGRAPH_H */


/* arch-tag: Tom Lord Fri Oct 29 11:13:05 2004 (libawiki/node-paragraph.h)
 */
