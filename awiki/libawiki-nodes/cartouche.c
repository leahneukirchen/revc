/* cartouche.c: 
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
#include "awiki/libawiki-nodes/cartouche.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (cartouche, "cartouche");
Identifier_safe_decl (essay, "essay");


/* code */

static t_awiki_node_fns cartouche_type_fns =
{
  (t_uchar *)"builtin 'cartouche' type",
  awiki_parse_cartouche
};

static t_awiki_node_type cartouche_type = { &cartouche_type_fns };

int
awiki_init_node_cartouche (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier (cartouche), &cartouche_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_cartouche (t_awiki_node_type * node_type,
                   ssize_t graph,
                   ssize_t node,
                   t_buffer_point * error_pt,
                   t_buffer_point * start_pt,
                   t_buffer_point * end_pt)
{
  ssize_t essay_node;

  /* The body of a cartouche display consists of regions of 
   * an essay.
   */

  essay_node = digraph_alloc_node (graph);
  if (essay_node < 0)
    return -1;

  if (0 > digraph_insert_edge (graph, node, 0, essay_node))
    return -1;

  if (0 > awiki_parse_for_type (Identifier (essay), graph, essay_node, error_pt, start_pt, end_pt))
    return -1;

  /* Oh, yeah: the type:
   */
  if (0 > awiki_set_node_type (graph, node, Identifier (cartouche)))
    return -1;

  if (0 > bufpt_goto (start_pt, bufpt_pos (end_pt)))
    return -1;

  return 0;
}



/* arch-tag: Tom Lord Sat Nov 20 11:47:37 2004 (libawiki-nodes/cartouche.c)
 */
