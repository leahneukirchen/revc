/* blockquote.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI_NODES__BLOCKQUOTE_HTML_H
#define INCLUDE__LIBAWIKI_NODES__BLOCKQUOTE_HTML_H


#include "awiki/libawiki-nodes/blockquote.h"



/* automatically generated __STDC__ prototypes */
extern int awiki_init_node_blockquote_html (void);
extern int awiki_blockquote_to_html (t_awiki_html_state * state,
                                    t_awiki_html_type * type,
                                    ssize_t graph,
                                    ssize_t node,
                                    t_buffer_point * error_pt,
                                    t_buffer_point * output_pt);
#endif  /* INCLUDE__LIBAWIKI_NODES__BLOCKQUOTE_HTML_H */


/* arch-tag: Tom Lord Wed Nov 24 09:48:40 2004 (libawiki-nodes/blockquote-html.h)
 */
