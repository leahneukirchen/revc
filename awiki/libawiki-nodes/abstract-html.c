/* abstract.c: 
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
#include "awiki/libawiki-nodes/abstract-html.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (abstract, "abstract");
Identifier_safe_decl (null, "null");



/* code */

static t_awiki_html_fns abstract_html_fns =
{
  (t_uchar *)"builtin 'abstract' HTML type",
  awiki_abstract_to_html
};

static t_awiki_html_type abstract_html_type = { &abstract_html_fns };


int
awiki_init_node_abstract_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier (abstract), &abstract_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_abstract_to_html (t_awiki_html_state * state,
                        t_awiki_html_type * type,
                        ssize_t graph,
                        ssize_t node,
                        t_buffer_point * error_pt,
                        t_buffer_point * output_pt)
{
  ssize_t x;
  ssize_t title_node;
  ssize_t bound;
  
  if (!state)
    return -1;
  
  title_node = digraph_subnode_data_k (graph, node, 0);
  if (0 > title_node)
    return -1;
  if (Identifier (null) != awiki_node_type (graph, title_node))
    {
      awiki_html_indent (state, output_pt);
      awiki_html_directive (state, output_pt, "p class=\"abstract-title\"");
      if (0 > awiki_to_html (state, graph, title_node, error_pt, output_pt))
        return -1;
      if (0 > awiki_html_newline_indent (state, output_pt))
        return -1;
    }

  AWIKI_HTML_MULTILINE (state, output_pt, "div class=\"abstract-body\"");
  {
    bound = digraph_n_subnodes (graph, node);
    for (x = 1; x < bound; ++x)
      {
        ssize_t subnode;

        subnode = digraph_subnode_data_k (graph, node, x);
        if (subnode < 0)
          return -1;
        if (0 > awiki_to_html (state, graph, subnode, error_pt, output_pt))
          return -1;
      }
  }
  AWIKI_HTML_CLOSE_MULTILINE (state, output_pt, "div");
  
  return 0;
}



/* arch-tag: Tom Lord Sun Nov 21 16:28:26 2004 (libawiki-nodes/abstract-html.c)
 */
