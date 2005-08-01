/* display.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__DISPLAY_H
#define INCLUDE__LIBAWIKI__DISPLAY_H


#include "awiki/libawiki/awiki.h"


/* automatically generated __STDC__ prototypes */
extern int awiki_constrained_display_scan (const t_uchar * type_name,
                                           t_buffer_point * start_pt,
                                           t_buffer_point * end_pt);
extern int awiki_display_scan (t_buffer_point * start_pt,
                               t_buffer_point * end_pt);
extern int awiki_parse_for_display (ssize_t graph,
                                    ssize_t node,
                                    t_buffer_point * error_pt,
                                    t_buffer_point * start_pt,
                                    t_buffer_point * end_pt);
extern int awiki_display_parse_headers (const t_uchar ** type_id,
                                        ssize_t * ref_col,
                                        ssize_t graph,
                                        ssize_t node,
                                        t_buffer_point * error_pt,
                                        t_buffer_point * start_pt,
                                        t_buffer_point * end_pt);
#endif  /* INCLUDE__LIBAWIKI__DISPLAY_H */


/* arch-tag: Tom Lord Sat Oct 30 09:39:16 2004 (display.h)
 */
