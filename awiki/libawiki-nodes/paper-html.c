/* paper-html.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "awiki/libawiki-nodes/paper-html.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (paper, "paper");

static t_awiki_html_fns paper_html_fns =
{
  (t_uchar *)"builtin 'paper' HTML type",
  awiki_paper_to_html
};

static t_awiki_html_type paper_html_type = { &paper_html_fns };


/* code */

int
awiki_init_node_paper_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier_paper (), &paper_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}


int
awiki_paper_to_html (t_awiki_html_state * state,
                     t_awiki_html_type * type,
                     ssize_t graph,
                     ssize_t node,
                     t_buffer_point * error_pt,
                     t_buffer_point * output_pt)
{
  if (!state)
    return -1;

  AWIKI_HTML_MULTILINE (state, output_pt, "html");
  {
    /* head */
    AWIKI_HTML_MULTILINE (state, output_pt, "head");
    {
    }
    AWIKI_HTML_CLOSE_MULTILINE (state, output_pt, "head");

    /* body */
    AWIKI_HTML_MULTILINE (state, output_pt, "body");
    {
      ssize_t bound;
      ssize_t x;

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
        }
    }
    AWIKI_HTML_CLOSE_MULTILINE (state, output_pt, "body");
  }
  AWIKI_HTML_CLOSE_MULTILINE (state, output_pt, "html");
  
  return 0;
}



/* arch-tag: Tom Lord Sun Nov 21 11:56:50 2004 (paper-html.c)
 */
