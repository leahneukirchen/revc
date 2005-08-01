/* styled-lines.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/char-class.h"
#include "hackerlab/buffers/buffers.h"
#include "hackerlab/identifiers/identifier.h"
#include "awiki/libawiki-nodes/scanners.h"
#include "awiki/libawiki-nodes/styled-lines-html.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (styled_lines, "styled-lines");


/* code */

static t_awiki_html_fns styled_lines_html_fns =
{
  (t_uchar *)"builtin 'styled-lines' HTML type",
  awiki_styled_lines_to_html
};

static t_awiki_html_type styled_lines_html_type = { &styled_lines_html_fns };


int
awiki_init_node_styled_lines_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier (styled_lines), &styled_lines_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_styled_lines_to_html (t_awiki_html_state * state,
                            t_awiki_html_type * type,
                            ssize_t graph,
                            ssize_t node,
                            t_buffer_point * error_pt,
                            t_buffer_point * output_pt)
{
  ssize_t bound;
  ssize_t x;

  if (!state)
    return -1;

  bound = digraph_n_subnodes (graph, node);
      
  for (x = 0; x < bound; ++x)
    {
      ssize_t subnode;
      int q;

      subnode = digraph_subnode_data_k (graph, node, x);
      if (subnode < 0)
        return -1;
      if (!awiki_node_type (graph, subnode))
        continue;

      q = awiki_to_html (state, graph, subnode, error_pt, output_pt);

      if (0 > q)
        return -1;

      if (0 > awiki_html_directive (state, output_pt, "br"))
        return -1;

      if (0 > awiki_html_newline_indent (state, output_pt))
        return -1;
    }

  return 0;
}





/* arch-tag: Tom Lord Sun Nov 21 16:24:21 2004 (libawiki-nodes/styled-lines-html.c)
 */
