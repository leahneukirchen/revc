/* saying-html.c: 
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
#include "awiki/libawiki-nodes/saying.h"
#include "awiki/libawiki-nodes/saying-html.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (saying, "saying");


/* code */

static t_awiki_html_fns saying_html_fns =
{
  (t_uchar *)"builtin 'saying' HTML type",
  awiki_saying_to_html
};

static t_awiki_html_type saying_html_type = { &saying_html_fns };


int
awiki_init_node_saying_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier (saying), &saying_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_saying_to_html (t_awiki_html_state * state,
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

  AWIKI_HTML_MULTILINE (state, output_pt, "blockquote class=\"saying\"");
  awiki_html_indent (state, output_pt);
  {
    for (x = 0; x < bound; ++x)
      {
        ssize_t subnode;
        int q;

        subnode = digraph_subnode_data_k (graph, node, x);
        if (subnode < 0)
          return -1;
        if (x)
          {
            awiki_html_directive (state, output_pt, "hr width=\"14%\"");
            awiki_html_newline_indent (state, output_pt);
            awiki_html_directive (state, output_pt, "small");
          }
        q = awiki_to_html (state, graph, subnode, error_pt, output_pt);
        if (x)
          awiki_html_directive (state, output_pt, "/small");

        if (0 > q)
          return -1;
      }
  }
  AWIKI_HTML_CLOSE_MULTILINE (state, output_pt, "blockquote");
  
  return 0;
}



/* arch-tag: Tom Lord Mon Nov 22 10:37:02 2004 (libawiki-nodes/saying-html.c)
 */
