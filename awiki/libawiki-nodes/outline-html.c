/* outline-html.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "awiki/libawiki/awiki.h"
#include "awiki/libawiki-nodes/outline-html.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (outline, "outline");
Identifier_safe_decl (outline_section, "outline-section");


/* code */

static t_awiki_html_fns outline_html_fns =
{
  (t_uchar *)"builtin 'outline' HTML type",
  awiki_outline_to_html
};

static t_awiki_html_type outline_html_type = { &outline_html_fns };


static t_awiki_html_fns outline_section_html_fns =
{
  (t_uchar *)"builtin 'outline-section' HTML type",
  awiki_outline_section_to_html
};

static t_awiki_html_type outline_section_html_type = { &outline_section_html_fns };


int
awiki_init_node_outline_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier (outline), &outline_html_type))
        return -1;
      if (0 > awiki_register_html_type (Identifier (outline_section), &outline_section_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_outline_to_html (t_awiki_html_state * state,
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


int
awiki_outline_section_to_html (t_awiki_html_state * state,
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

  awiki_html_newline_indent (state, output_pt);
  state->indent_level += 2;
  awiki_html_newline_indent (state, output_pt);


  {
    ssize_t title_node;

    title_node = digraph_subnode_data_k (graph, node, 0);
    if (title_node < 0)
      return -1;
    switch (state->section_depth)
      {
      case 0:
        if (   (0 > awiki_html_directive (state, output_pt, "h2"))
            || (0 > awiki_to_html (state, graph, title_node, error_pt, output_pt))
            || (0 > awiki_html_directive (state, output_pt, "/h2")))
          return -1;
        break;
      case 1:
        if (   (0 > awiki_html_directive (state, output_pt, "h3"))
            || (0 > awiki_to_html (state, graph, title_node, error_pt, output_pt))
            || (0 > awiki_html_directive (state, output_pt, "/h3")))
          return -1;
        break;
      case 2:
        if (   (0 > awiki_html_directive (state, output_pt, "h4"))
            || (0 > awiki_to_html (state, graph, title_node, error_pt, output_pt))
            || (0 > awiki_html_directive (state, output_pt, "/h4")))
          return -1;
        break;
      default:
        {
          ssize_t x;

          if (0 > awiki_html_directive (state, output_pt, "h4"))
            return -1;

          for (x = 2; x < state->section_depth; ++x)
            if (0 > bufpt_insert_str_n (output_pt, "*", 1))
              return -1;

          if (0 > bufpt_insert_str_n (output_pt, " ", 1))
            return -1;

          if (0 > awiki_to_html (state, graph, title_node, error_pt, output_pt))
            return -1;

          if (0 > awiki_html_directive (state, output_pt, "/h4"))
            return -1;

          if(0 > awiki_html_newline_indent (state, output_pt))
            return -1;

          break;
        }
      }
  }


  for (x = 1; x < bound; ++x)
    {
      ssize_t subnode;

      subnode = digraph_subnode_data_k (graph, node, x);
      if (subnode < 0)
        return -1;
      ++state->section_depth;
      if (0 > awiki_to_html (state, graph, subnode, error_pt, output_pt))
        return -1;
      --state->section_depth;
    }
  
  
  state->indent_level -= 2;
  awiki_html_newline_indent (state, output_pt);
  awiki_html_newline_indent (state, output_pt);
  
  return 0;
}




/* arch-tag: Tom Lord Sun Nov 21 17:08:57 2004 (libawiki-nodes/outline-html.c)
 */
