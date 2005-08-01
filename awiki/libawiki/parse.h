/* parse.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__AWIKI_PARSE_H
#define INCLUDE__LIBAWIKI__AWIKI_PARSE_H


#include "hackerlab/buffers/buffers.h"
#include "hackerlab/graphs/digraph.h"



typedef struct awiki_node_type t_awiki_node_type;
typedef struct awiki_node_fns t_awiki_node_fns;



typedef int (*t_awiki_parse_fn) (t_awiki_node_type * node_type,
                                 ssize_t graph,
                                 ssize_t node,
                                 t_buffer_point * error_pt,
                                 t_buffer_point * source_pt,
                                 t_buffer_point * end_pt);



struct awiki_node_fns
{
  t_uchar * debugger_name;
  t_awiki_parse_fn parser;
};


struct awiki_node_type
{
  t_awiki_node_fns * fns;
};




/* automatically generated __STDC__ prototypes */
extern int awiki_register_type (const t_uchar * name_id,
                                t_awiki_node_type * type);
extern t_awiki_node_type * awiki_find_type (const t_uchar * name_id);
extern int awiki_parse_for_type (const t_uchar * name_atom,
                                 ssize_t graph,
                                 ssize_t node,
                                 t_buffer_point * error_pt,
                                 t_buffer_point * source_pt,
                                 t_buffer_point * end_pt);
#endif  /* INCLUDE__LIBAWIKI__AWIKI_PARSE_H */


/* arch-tag: Tom Lord Thu Oct 28 11:29:03 2004 (awiki-parse.h)
 */
