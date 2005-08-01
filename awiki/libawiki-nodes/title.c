/* title.c: 
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
#include "awiki/libawiki-nodes/title.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (title, "title");
Identifier_safe_decl (styled_lines, "styled-lines");


/* code */

static t_awiki_node_fns title_type_fns =
{
  (t_uchar *)"builtin 'title' type",
  awiki_parse_title
};

static t_awiki_node_type title_type = { &title_type_fns };

int
awiki_init_node_title (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier (title), &title_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_title (t_awiki_node_type * node_type,
                   ssize_t graph,
                   ssize_t node,
                   t_buffer_point * error_pt,
                   t_buffer_point * his_start_pt,
                   t_buffer_point * his_end_pt)
{
  t_buffer_point start_pt;
  t_buffer_point end_pt;
  t_buffer_point title_start_pt;

  init_bufpt (&start_pt);
  init_bufpt (&end_pt);
  init_bufpt (&title_start_pt);

  if (   (0 > bufpt_copy_from (&start_pt, his_start_pt))
      || (0 > bufpt_copy_from (&end_pt, his_end_pt)))
    {
    bail:
      uninit_bufpt (&start_pt);
      uninit_bufpt (&end_pt);
      uninit_bufpt (&title_start_pt);
      return -1;
    }


  /* The body of a title display consists of regions of 
   * stylized text, separated by lines containing only
   * a block of three or more dashes.
   * 
   * The first such region is the main title.  Later regions
   * are subtitles:
   * 
   *   [[title
   *		A Pattern Language
   *		      ----
   *            Towns, Buildings, Construction
   *   ]]
   * 
   * After parsing display headers (above), start_pt and end_pt
   * leave us with just the body lines of the display (between the
   * '[[' and ']]' lines).
   * 
   */

  while (1)
    {
      int has_dash_three;
      ssize_t subnode;
        
      if (0 > bufpt_copy_from (&title_start_pt, &start_pt))
        goto bail;

      has_dash_three = awiki_scan_til_dash3_line_or_end (&start_pt, &end_pt);

      subnode = digraph_alloc_node (graph);
      if (subnode < 0)
        goto bail;
        
      if (0 >  digraph_insert_edge (graph, node, digraph_n_subnodes (graph, node), subnode))
        goto bail;

      if (0 > awiki_parse_for_type (Identifier (styled_lines), graph, subnode, error_pt, &title_start_pt, &start_pt))
        goto bail;

      if (!has_dash_three)
        break;
      else
        {
          if (0 > bufpt_next_line (&start_pt, &end_pt))
            goto bail;
        }
    }

  if (0 > awiki_set_node_type (graph, node, Identifier (title)))
    goto bail;

  if (   (0 > uninit_bufpt (&start_pt))
      || (0 > uninit_bufpt (&end_pt))
      || (0 > uninit_bufpt (&title_start_pt)))
    goto bail;


  return 0;
}



/* arch-tag: Tom Lord Sun Oct 31 11:04:08 2004 (title.c)
 */
