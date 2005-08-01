/* paragraph.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "awiki/libawiki/awiki.h"
#include "awiki/libawiki-nodes/unparsed.h"
#include "awiki/libawiki-nodes/styled-text.h"
#include "awiki/libawiki-nodes/paragraph-html.h"



Identifier_safe_decl (paragraph, "paragraph");



static t_awiki_html_fns paragraph_html_fns =
{
  (t_uchar *)"builtin 'paragraph' HTML type",
  awiki_paragraph_to_html
};

static t_awiki_html_type paragraph_html_type = { &paragraph_html_fns };


int
awiki_init_node_paragraph_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier_paragraph (), &paragraph_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}





int
awiki_paragraph_to_html (t_awiki_html_state * state,
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

  AWIKI_HTML_MULTILINE (state, output_pt, "p");
  awiki_html_indent (state, output_pt);
  {
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
  }
  AWIKI_HTML_SILENT_CLOSE_MULTILINE (state, output_pt, "p");
  
  return 0;
}




/* arch-tag: Tom Lord Sun Nov 21 17:01:02 2004 (libawiki-nodes/paragraph-html.c)
 */