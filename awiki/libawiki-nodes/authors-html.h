/* authors-html.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__AUTHORS_HTML_H
#define INCLUDE__LIBAWIKI_NODES__AUTHORS_HTML_H


#include "awiki/libawiki-nodes/authors.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_authors_html (void);
extern int awiki_authors_to_html (t_awiki_html_state * state,
                                  t_awiki_html_type * type,
                                  ssize_t graph,
                                  ssize_t node,
                                  t_buffer_point * error_pt,
                                  t_buffer_point * output_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES__AUTHORS_HTML_H */


/* arch-tag: Tom Lord Sun Nov 21 15:31:27 2004 (authors-html.h)
 */
