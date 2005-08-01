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
#include "awiki/libawiki-nodes/styled-lines.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (styled_lines, "styled-lines");
Identifier_safe_decl (recursive_styled_text, "recursive-styled-text");


/* code */

static t_awiki_node_fns styled_lines_type_fns =
{
  (t_uchar *)"builtin 'styled-lines' type",
  awiki_parse_styled_lines
};

static t_awiki_node_type styled_lines_type = { &styled_lines_type_fns };



int
awiki_init_node_styled_lines (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier (styled_lines), &styled_lines_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_styled_lines (t_awiki_node_type * node_type,
                          ssize_t graph,
                          ssize_t node,
                          t_buffer_point * error_pt,
                          t_buffer_point * start_pt,
                          t_buffer_point * end_pt)
{
  ssize_t line_no;
  t_buffer_point line_start;

  init_bufpt (&line_start);

  if (0 > awiki_set_node_type (graph, node, Identifier (styled_lines)))
    {
    bail:
      uninit_bufpt (&line_start);
      return -1;
    }

  if (0 > bufpt_pass_whitespace (start_pt, end_pt))
    goto bail;


  if (0 > awiki_set_node_start (graph, node, start_pt))
    goto bail;

  line_no = 0;

  while (!bufpt_is_at_end (start_pt, end_pt))
    {
      if (0 > !bufpt_copy_from (&line_start, start_pt))
        goto bail;

      if (0 > bufpt_end_of_line (start_pt, end_pt))
        goto bail;

      while (bufpt_pos (&line_start) < bufpt_pos (start_pt))
        {
          if (0 > bufpt_backward_char (start_pt, end_pt, 1))
            goto bail;

          if (!char_is_blank (bufpt_char_at (start_pt, end_pt)))
            {
              if (0 > bufpt_forward_char (start_pt, end_pt, 1))
                goto bail;
              break;
            }
        }

      /* next line of styled text is between line_start and start_pt
       */
      {
        ssize_t subnode;

        subnode = digraph_alloc_node (graph);
        if (subnode < 0)
          goto bail;

        if (0 > digraph_insert_edge (graph, node, line_no, subnode))
          goto bail;

        ++line_no;

        if (0 > awiki_parse_for_type (Identifier (recursive_styled_text), graph, subnode, error_pt, &line_start, start_pt))
          goto bail;
      }


      /* Now move to the next line and continue
       */
      if (0 > bufpt_next_line (start_pt, end_pt))
        goto bail;

      if (0 > bufpt_pass_blanks (start_pt, end_pt))
        goto bail;
    }


  if (0 > awiki_set_node_end (graph, node, start_pt))
    return -1;

  if (0 > uninit_bufpt (&line_start))
    goto bail;

  return 0;
}



/* arch-tag: Tom Lord Sat Nov  6 11:26:20 2004 (styled-lines.c)
 */
