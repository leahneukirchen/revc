/* unparsed.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "awiki/libawiki-nodes/unparsed.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (unparsed, "unparsed");



static t_awiki_node_fns unparsed_type_fns =
{
  (t_uchar *)"builtin 'unparsed' type",
  awiki_parse_unparsed
};

static t_awiki_node_type unparsed_type = { &unparsed_type_fns };

int
awiki_init_node_unparsed (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier_unparsed (), &unparsed_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}

int
awiki_parse_unparsed (t_awiki_node_type * node_type,
                      ssize_t graph,
                      ssize_t node,
                      t_buffer_point * error_pt,
                      t_buffer_point * start_pt,
                      t_buffer_point * end_pt)
{
  if (0 > awiki_set_node_type (graph, node, Identifier (unparsed)))
    return -1;

  if (0 > awiki_set_node_start (graph, node, start_pt))
    return -1;

  if (0 > awiki_set_node_end (graph, node, end_pt))
    return -1;

  if (0 > bufpt_goto (start_pt, bufpt_pos (end_pt)))
    return -1;

  return 1;
}



/* arch-tag: Tom Lord Thu Oct 28 16:56:11 2004 (node-unparsed.c)
 */
