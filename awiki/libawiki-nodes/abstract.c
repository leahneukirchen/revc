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
#include "awiki/libawiki-nodes/abstract.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (abstract, "abstract");
Identifier_safe_decl (styled_lines, "styled-lines");
Identifier_safe_decl (essay, "essay");
Identifier_safe_decl (null, "null");



/* code */

static t_awiki_node_fns abstract_type_fns =
{
  (t_uchar *)"builtin 'abstract' type",
  awiki_parse_abstract
};

static t_awiki_node_type abstract_type = { &abstract_type_fns };



int
awiki_init_node_abstract (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier (abstract), &abstract_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_abstract (t_awiki_node_type * node_type,
                      ssize_t graph,
                      ssize_t node,
                      t_buffer_point * error_pt,
                      t_buffer_point * his_start_pt,
                      t_buffer_point * his_end_pt)
{
  t_buffer_point start_pt;
  t_buffer_point end_pt;
  t_buffer_point pos;
  int has_dash_three;

  init_bufpt (&start_pt);
  init_bufpt (&end_pt);
  init_bufpt (&pos);

  if (   (0 > bufpt_copy_from (&start_pt, his_start_pt))
      || (0 > bufpt_copy_from (&end_pt, his_end_pt))
      || (0 > bufpt_copy_from (&pos, his_start_pt)))
    {
    bail:
      (void)uninit_bufpt (&start_pt);
      (void)uninit_bufpt (&end_pt);
      (void)uninit_bufpt (&pos);
      return -1;
    }

  /* The body of a abstract display consists of a region of 
   * styled text, separated by a line containing only
   * a block of three or more dashes from an essay.
   * 
   * The first region is the abstract title.  The
   * second the abstract body.
   * 
   *   [[abstract
   *		     Abstract
   *		      ----
   *          The earth is flat, it turns out.
   *   ]]
   * 
   * After parsing display headers (above), start_pt and end_pt
   * leave us with just the body lines of the display (between the
   * '[[' and ']]' lines).
   * 
   * If no "---" line is present, then the abstract doesn't have a 
   * title.
   * 
   */

  has_dash_three = awiki_scan_til_dash3_line_or_end (&pos, &end_pt);

  {
    ssize_t title_node;

    title_node = digraph_alloc_node (graph);
    if (0 > title_node)
      goto bail;

    if (0 > digraph_insert_edge (graph, node, 0, title_node))
      goto bail;

    if (!has_dash_three)
      {
        if (0 > awiki_parse_for_type (Identifier (null), graph, title_node, error_pt, &start_pt, &start_pt))
          goto bail;
        if (0 > bufpt_goto (&pos, bufpt_pos (&start_pt)))
          goto bail;
      }
    else
      {
        if (0 > awiki_parse_for_type (Identifier (styled_lines), graph, title_node, error_pt, &start_pt, &pos))
          goto bail;
        if (0 > bufpt_end_of_line (&pos, &end_pt))
          goto bail;
        if (0 > bufpt_next_line (&pos, &end_pt))
          goto bail;
      }
  }

  {
    ssize_t body_node;
      
    body_node = digraph_alloc_node (graph);
    if (body_node < 0)
      goto bail;
        
    if (0 >  digraph_insert_edge (graph, node, digraph_n_subnodes (graph, node), body_node))
      goto bail;

    if (0 > awiki_parse_for_type (Identifier (essay), graph, body_node, error_pt, &pos, &end_pt))
      goto bail;
  }

  if (0 > bufpt_goto (his_start_pt, bufpt_pos (his_end_pt)))
    goto bail;

  if (0 > awiki_set_node_type (graph, node, Identifier (abstract)))
    goto bail;

  if (0 > uninit_bufpt (&pos))
    {
      (void)uninit_bufpt (&start_pt);
      (void)uninit_bufpt (&end_pt);
      return -1;
    }

  if (0 > uninit_bufpt (&start_pt))
    {
      (void)uninit_bufpt (&end_pt);
      return -1;
    }

  if (0 > uninit_bufpt (&end_pt))
    return -1;

  return 0;
}



/* arch-tag: Tom Lord Sat Nov 20 12:56:47 2004 (libawiki-nodes/abstract.c)
 */
