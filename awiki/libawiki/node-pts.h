/* node-pts.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__NODE_PTS_H
#define INCLUDE__LIBAWIKI__NODE_PTS_H


#include "hackerlab/buffers/buffers.h"


/* automatically generated __STDC__ prototypes */
extern t_buffer_point * awiki_node_start (ssize_t graph,
                                          ssize_t node);
extern t_buffer_point * awiki_node_end (ssize_t graph,
                                        ssize_t node);
extern int awiki_set_node_start (ssize_t graph,
                                 ssize_t node,
                                 t_buffer_point * where);
extern int awiki_set_node_end (ssize_t graph,
                               ssize_t node,
                               t_buffer_point * where);
#endif  /* INCLUDE__LIBAWIKI__NODE_PTS_H */


/* arch-tag: Tom Lord Sun Nov 21 11:53:28 2004 (node-pts.h)
 */
