/* essay-html.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "awiki/libawiki/awiki.h"
#include "awiki/libawiki-nodes/display.h"
#include "awiki/libawiki-nodes/essay-html.h"

/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (essay, "essay");


/* code */

static t_awiki_html_fns essay_html_fns =
{
  (t_uchar *)"builtin 'essay' HTML type",
  awiki_essay_to_html
};

static t_awiki_html_type essay_html_type = { &essay_html_fns };


int
awiki_init_node_essay (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier (essay), &essay_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_essay_to_html (t_awiki_html_state * state,
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
      q = awiki_to_html (state, graph, subnode, error_pt, output_pt);

      if (0 > q)
        return -1;
    }
  
  return 0;
}




/* arch-tag: Tom Lord Sun Nov 21 16:32:41 2004 (libawiki-nodes/essay-html.c)
 */
