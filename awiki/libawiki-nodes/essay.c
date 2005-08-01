/* essay.c: 
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
#include "awiki/libawiki-nodes/essay.h"

/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (essay, "essay");
Identifier_safe_decl (paragraph, "paragraph");
Identifier_safe_decl (unparsed, "unparsed");


/* code */

static t_awiki_node_fns essay_type_fns =
{
  (t_uchar *)"builtin 'essay' type",
  awiki_parse_essay
};

static t_awiki_node_type essay_type = { &essay_type_fns };

int
awiki_init_node_essay_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier (essay), &essay_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_essay (t_awiki_node_type * node_type,
                   ssize_t graph,
                   ssize_t node,
                   t_buffer_point * error_pt,
                   t_buffer_point * start_pt,
                   t_buffer_point * end_pt)
{
  if (0 > awiki_set_node_type (graph, node, Identifier (essay)))
    return -1;

  while (1)
    {
      ssize_t subnode;
      int is_paragraph;

      subnode = digraph_alloc_node (graph);
      if (subnode < 0)
        return -1;

      is_paragraph = awiki_parse_for_type (Identifier (paragraph), graph, subnode, error_pt, start_pt, end_pt);
      if (is_paragraph < 0)
        {
          digraph_free_node (graph, subnode);
          return -1;
        }
      else if (!is_paragraph)
        {
          int is_display;
          
          is_display = awiki_parse_for_display (graph, subnode, error_pt, start_pt, end_pt);
          if (is_display < 0)
            {
              digraph_free_node (graph, subnode);
              return -1;
            }
          else if (!is_display)
            {
              digraph_free_node (graph, subnode);
              break;
            }
        }

      if (0 > digraph_insert_edge (graph, node, digraph_n_subnodes (graph, node), subnode))
        {
          digraph_free_node (graph, subnode);
          return -1;
        }
    }

  if (0 > bufpt_pass_whitespace (start_pt, end_pt))
    return -1;

  if (bufpt_pos (start_pt) < bufpt_pos (end_pt))
    {
      ssize_t subnode;

      subnode = digraph_alloc_node (graph);
      if (subnode < 0)
        return -1;

      if (0 > awiki_parse_for_type (Identifier (unparsed), graph, subnode, error_pt, start_pt, end_pt))
        {
          digraph_free_node (graph, subnode);
          return -1;
        }

      if (0 > digraph_insert_edge (graph, node, digraph_n_subnodes (graph, node), subnode))
        {
          digraph_free_node (graph, subnode);
          return -1;
        }
    }
  
  return 1;
}


/* arch-tag: Tom Lord Sun Oct 31 09:52:52 2004 (essay.c)
 */
