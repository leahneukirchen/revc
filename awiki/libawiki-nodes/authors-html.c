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



/* code */

static t_awiki_html_fns authors_html_fns =
{
  (t_uchar *)"builtin 'authors' HTML type",
  awiki_authors_to_html
};

static t_awiki_html_type authors_html_type = { &authors_html_fns };


int
awiki_init_node_authors_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier (authors), &authors_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_authors_to_html (t_awiki_html_state * state,
                     t_awiki_html_type * type,
                     ssize_t graph,
                     ssize_t node,
                     t_buffer_point * error_pt,
                     t_buffer_point * output_pt)
{
  ssize_t lines_node;

  if (!state)
    return -1;

  lines_node = digraph_subnode_data_k (graph, node, 0);
  if (0 > lines_node)
    return -1;

  awiki_html_indent (state, output_pt);
  awiki_html_directive (state, output_pt, "p align=center");
  awiki_html_directive (state, output_pt, "b");
        
  if (0 > awiki_to_html (state, graph, lines_node, error_pt, output_pt))
    return -1;
        
  awiki_html_directive (state, output_pt, "/b");

  if (0 > awiki_html_newline_indent (state, output_pt))
    return -1;

  awiki_html_directive (state, output_pt, "hr width=\"25%\" align=center");

  if (0 > awiki_html_newline_indent (state, output_pt))
    return -1;

  return 0;
}



/* arch-tag: Tom Lord Sun Nov 21 15:30:36 2004 (libawiki-nodes/authors-html.c)
 */
