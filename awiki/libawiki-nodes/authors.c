/* authors.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "awiki/libawiki-nodes/display.h"
#include "awiki/libawiki-nodes/scanners.h"
#include "awiki/libawiki-nodes/authors.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (authors, "authors");
Identifier_safe_decl (styled_lines, "styled-lines");



/* code */

static t_awiki_node_fns authors_type_fns =
{
  (t_uchar *)"builtin 'authors' type",
  awiki_parse_authors
};

static t_awiki_node_type authors_type = { &authors_type_fns };

int
awiki_init_node_authors (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier (authors), &authors_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_authors (t_awiki_node_type * node_type,
                     ssize_t graph,
                     ssize_t node,
                     t_buffer_point * error_pt,
                     t_buffer_point * start_pt,
                     t_buffer_point * end_pt)
{
  ssize_t lines_node;

  lines_node = digraph_alloc_node (graph);
  if (0 > lines_node)
    return -1;

  if (0 > digraph_insert_edge (graph, node, 0, lines_node))
    return -1;

  if (0 > awiki_parse_for_type (Identifier (styled_lines), graph, lines_node, error_pt, start_pt, end_pt))
    return -1;

  if (0 > awiki_set_node_type (graph, node, Identifier (authors)))
    return -1;

  return 1;
}




/* arch-tag: Tom Lord Sat Nov 20 14:04:36 2004 (libawiki-nodes/authors.c)
 */
