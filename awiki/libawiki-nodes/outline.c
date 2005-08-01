/* outline.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "awiki/libawiki/awiki.h"
#include "awiki/libawiki-nodes/outline.h"


/* typedefs */


/* __STDC__ prototypes for static functions */
static int awiki_parse_outline_or_section_parts (t_awiki_node_type * node_type,
                                                 ssize_t graph,
                                                 ssize_t node,
                                                 t_buffer_point * error_pt,
                                                 t_buffer_point * start_pt,
                                                 t_buffer_point * end_pt);


/* declarations */

Identifier_safe_decl (outline, "outline");
Identifier_safe_decl (essay, "essay");
Identifier_safe_decl (recursive_styled_text, "recursive-styled-text");
Identifier_safe_decl (outline_section, "outline-section");


/* code */

static t_awiki_node_fns outline_type_fns =
{
  (t_uchar *)"builtin 'outline' type",
  awiki_parse_outline
};

static t_awiki_node_type outline_type = { &outline_type_fns };

static t_awiki_node_fns outline_section_type_fns =
{
  (t_uchar *)"builtin 'outline-section' type",
  awiki_parse_outline_section
};

static t_awiki_node_type outline_section_type = { &outline_section_type_fns };


int
awiki_init_node_outline (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier (outline), &outline_type))
        return -1;
      if (0 > awiki_register_type (Identifier (outline_section), &outline_section_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_outline (t_awiki_node_type * node_type,
                   ssize_t graph,
                   ssize_t node,
                   t_buffer_point * error_pt,
                   t_buffer_point * start_pt,
                   t_buffer_point * end_pt)
{
  if (0 > awiki_set_node_type (graph, node, Identifier (outline)))
    return -1;

  return awiki_parse_outline_or_section_parts (node_type, graph, node, error_pt, start_pt, end_pt);
}


static int
awiki_parse_outline_or_section_parts (t_awiki_node_type * node_type,
                                      ssize_t graph,
                                      ssize_t node,
                                      t_buffer_point * error_pt,
                                      t_buffer_point * start_pt,
                                      t_buffer_point * end_pt)
{
  t_buffer_point lead_start;
  t_buffer_point lead_end;
  t_buffer_point section_start;
  t_buffer_point section_end;
  int answer = -1;


  init_bufpt (&lead_start);
  init_bufpt (&lead_end);
  init_bufpt (&section_start);
  init_bufpt (&section_end);

  if (   (0 > bufpt_copy_from (&lead_start, start_pt))
      || (0 > bufpt_copy_from (&lead_end, start_pt))
      || (0 > bufpt_copy_from (&section_start, start_pt))
      || (0 > bufpt_copy_from (&section_end, start_pt)))
    {
    bail:
      answer = -1;

    normal_return:
      (void)uninit_bufpt (&lead_start);
      (void)uninit_bufpt (&lead_end);
      (void)uninit_bufpt (&section_start);
      (void)uninit_bufpt (&section_end);
      return answer;
    }


  while (!bufpt_is_at_end (&lead_end, end_pt))
    {
      if ((bufpt_col_tabifying (&lead_end) == 0)
          && bufpt_looking_at_str_n (&lead_end, end_pt, "*", 1))
        break;
      if (0 > bufpt_next_line (&lead_end, end_pt))
        goto bail;
    }

  if (0 > bufpt_copy_from (&section_start, &lead_end))
    goto bail;

  {
    ssize_t lead_node;

    lead_node = digraph_alloc_node (graph);
    if (lead_node < 0)
      goto bail;

    if (0 > digraph_insert_edge (graph, node, digraph_n_subnodes (graph, node), lead_node))
      goto bail;

    if (0 > awiki_parse_for_type (Identifier (essay), graph, lead_node, error_pt, &lead_start, &lead_end))
      goto bail;
  }

  if (bufpt_is_at_end (&section_start, end_pt))
    {
      answer = 1;
      goto normal_return;
    }

  {
    ssize_t outline_depth;
    
    if (0 > bufpt_goto (&section_end, bufpt_pos (&section_start)))
      goto bail;
    
    outline_depth = 0;
    while (   (!bufpt_is_at_end (&section_end, end_pt))
           && ('*' == bufpt_char_at (&section_end, end_pt)))
      {
        if (0 > bufpt_goto (&section_end, 1 + bufpt_pos (&section_end)))
          goto bail;
        ++outline_depth;
      }
    if (0 > bufpt_beginning_of_line (&section_end, end_pt))
      goto bail;

    while (1)
      {
        ssize_t this_depth;

        do
          {
            if (0 > bufpt_end_of_line (&section_end, end_pt))
              goto bail;
            if (0 > bufpt_next_line (&section_end, end_pt))
              goto bail;
          }
        while (   !bufpt_is_at_end (&section_end, end_pt)
               && ('*' != bufpt_char_at (&section_end, end_pt)));


        this_depth = 0;
        while (   (!bufpt_is_at_end (&section_end, end_pt))
               && ('*' == bufpt_char_at (&section_end, end_pt)))
          {
            if (0 > bufpt_goto (&section_end, 1 + bufpt_pos (&section_end)))
              goto bail;
            ++this_depth;
          }

        if (this_depth && (this_depth < outline_depth))
          outline_depth = this_depth;

        if ((this_depth == outline_depth) || bufpt_is_at_end (&section_end, end_pt))
          {
            ssize_t section_node;

            if (this_depth == outline_depth)
              {
                if (0 > bufpt_beginning_of_line (&section_end, end_pt))
                  goto bail;
              }

            section_node = digraph_alloc_node (graph);
            if (section_node < 0)
              goto bail;

            if (0 > digraph_insert_edge (graph, node, digraph_n_subnodes (graph, node), section_node))
              goto bail;

            if (0 > awiki_parse_for_type (Identifier (outline_section), graph, section_node, error_pt, &section_start, &section_end))
              goto bail;

            if (0 > bufpt_goto (&section_start, bufpt_pos (&section_end)))
              goto bail;

            if (bufpt_is_at_end (&section_start, end_pt))
              {
                answer = 1;
                goto normal_return;
              }
          }
      }

    
  }

  return 0;
}


int
awiki_parse_outline_section (t_awiki_node_type * node_type,
                             ssize_t graph,
                             ssize_t node,
                             t_buffer_point * error_pt,
                             t_buffer_point * start_pt,
                             t_buffer_point * end_pt)
{
  t_buffer_point title_start;
  t_buffer_point title_end;
  int answer = -1;

  init_bufpt (&title_start);
  init_bufpt (&title_end);

  if (0 > bufpt_pass_whitespace (start_pt, end_pt))
    return -1;

  if (   (0 > bufpt_copy_from (&title_start, start_pt))
      || (0 > bufpt_copy_from (&title_end, start_pt)))
    {
    bail:
      answer = -1;

    normal_return:
      (void)uninit_bufpt (&title_start);
      (void)uninit_bufpt (&title_end);
      return answer;
    }

  while (   (!bufpt_is_at_end (start_pt, end_pt))
         && ('*' == bufpt_char_at (start_pt, end_pt)))
    {
      if (0 > bufpt_goto (start_pt, 1 + bufpt_pos (start_pt)))
        goto bail;
    }

  if (0 > bufpt_pass_blanks (start_pt, end_pt))
    goto bail;

  if (0 > bufpt_goto (&title_start, bufpt_pos (start_pt)))
    goto bail;

  if (0 > bufpt_next_line (start_pt, end_pt))
    goto bail;

  while (   (!bufpt_is_at_end (start_pt, end_pt))
         && !bufpt_looking_at_blank_line (start_pt, end_pt))
    {
      if (0 > bufpt_next_line (start_pt, end_pt))
        goto bail;
    }

  if (0 > bufpt_goto (&title_end, bufpt_pos (start_pt)))
    goto bail;

  {
    ssize_t section_title_node;

    section_title_node = digraph_alloc_node (graph);
    if (section_title_node < 0)
      goto bail;

    if (0 > digraph_insert_edge (graph, node, 0, section_title_node))
      goto bail;

    if (0 > awiki_parse_for_type (Identifier (recursive_styled_text), graph, section_title_node, error_pt, &title_start, &title_end))
      goto bail;
  }

  answer = awiki_parse_outline_or_section_parts (node_type, graph, node, error_pt, start_pt, end_pt);

  if (0 > awiki_set_node_type (graph, node, Identifier (outline_section)))
    return -1;

  answer = 1;

  goto normal_return;
}



/* arch-tag: Tom Lord Thu Nov 18 19:51:38 2004 (libawiki-nodes/outline.c)
 */
