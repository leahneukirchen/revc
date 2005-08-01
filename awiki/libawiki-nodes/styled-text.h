/* styled-text.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__STYLED_TEXT_H
#define INCLUDE__LIBAWIKI_NODES__STYLED_TEXT_H


#include "awiki/libawiki/awiki.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_styled_text (void);
extern int awiki_treeparse_styled_text (ssize_t graph,
                                        ssize_t node,
                                        t_buffer_point * error_pt,
                                        t_buffer_point * start_pt,
                                        t_buffer_point * end_pt);
extern int awiki_parse_styled_text (t_awiki_node_type * node_type,
                                    ssize_t graph,
                                    ssize_t node,
                                    t_buffer_point * error_pt,
                                    t_buffer_point * start_pt,
                                    t_buffer_point * end_pt);
extern int awiki_parse_emphasized_text (t_awiki_node_type * node_type,
                                        ssize_t graph,
                                        ssize_t node,
                                        t_buffer_point * error_pt,
                                        t_buffer_point * start_pt,
                                        t_buffer_point * end_pt);
extern int awiki_parse_code_text (t_awiki_node_type * node_type,
                                  ssize_t graph,
                                  ssize_t node,
                                  t_buffer_point * error_pt,
                                  t_buffer_point * start_pt,
                                  t_buffer_point * end_pt);
extern int awiki_parse_url_text (t_awiki_node_type * node_type,
                                 ssize_t graph,
                                 ssize_t node,
                                 t_buffer_point * error_pt,
                                 t_buffer_point * start_pt,
                                 t_buffer_point * end_pt);
extern int awiki_parse_noteref_text (t_awiki_node_type * node_type,
                                     ssize_t graph,
                                     ssize_t node,
                                     t_buffer_point * error_pt,
                                     t_buffer_point * start_pt,
                                     t_buffer_point * end_pt);
extern int awiki_parse_stressed_text (t_awiki_node_type * node_type,
                                      ssize_t graph,
                                      ssize_t node,
                                      t_buffer_point * error_pt,
                                      t_buffer_point * start_pt,
                                      t_buffer_point * end_pt);
extern int awiki_parse_recursive_styled_text (t_awiki_node_type * node_type,
                                              ssize_t graph,
                                              ssize_t node,
                                              t_buffer_point * error_pt,
                                              t_buffer_point * start_pt,
                                              t_buffer_point * end_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES__STYLED_TEXT_H */


/* arch-tag: Tom Lord Wed Nov 17 16:59:28 2004 (libawiki-nodes/styled-text.h)
 */
