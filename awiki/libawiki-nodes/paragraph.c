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
#include "awiki/libawiki-nodes/paragraph.h"



Identifier_safe_decl (paragraph, "paragraph");

Regexp_decl (blank_line, "^[[:blank:]]*\n", REG_EXTENDED);
Regexp_decl (continued_line, "^[^\n]*\\[\\.\\.\\]", REG_EXTENDED);





static t_awiki_node_fns paragraph_type_fns =
{
  (t_uchar *)"builtin 'paragraph' type",
  awiki_parse_paragraph
};

static t_awiki_node_type paragraph_type = { &paragraph_type_fns };


int
awiki_init_node_paragraph (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier_paragraph (), &paragraph_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_paragraph (t_awiki_node_type * node_type,
                      ssize_t graph,
                      ssize_t node,
                      t_buffer_point * error_pt,
                      t_buffer_point * start_pt,
                      t_buffer_point * end_pt)
{
  t_buffer_point pos;
  t_buffer_point my_start;
  t_buffer_point my_end;
  int answer;

  init_bufpt (&pos);
  init_bufpt (&my_start);
  init_bufpt (&my_end);

  if (0 > bufpt_copy_from (&pos, start_pt))
    {
    bail:
      uninit_bufpt (&pos);
      uninit_bufpt (&my_start);
      uninit_bufpt (&my_end);
      return -1;
    }

  answer = 0;

  if (0 > bufpt_pass_whitespace (&pos, end_pt))
    goto bail;

  if (bufpt_pos (&pos) >= bufpt_pos (end_pt))
    goto return_normally;

  if (bufpt_looking_at_str_n (&pos, end_pt, "[[", 2))
    goto return_normally;

  if ((0 == bufpt_col_tabifying (&pos)) && (bufpt_looking_at_str_n (&pos, end_pt, "*", 1)))
    goto return_normally;

  if (0 > awiki_set_node_start (graph, node, &pos))
    goto bail;


  {
    while (bufpt_pos (&pos) < bufpt_pos (end_pt))
      {
        if (bufpt_looking_at_re (&pos, end_pt, Regexp (continued_line), 0))
          {
            if (0 > bufpt_next_line (&pos, end_pt))
              goto bail;

            while (   (bufpt_pos (&pos) < bufpt_pos (end_pt))
                   && (bufpt_looking_at_re (&pos, end_pt, Regexp (blank_line), 0)))
              {
                if (0 > bufpt_next_line (&pos, end_pt))
                  goto bail;
              }
          }
        else
          {
            if (0 > bufpt_next_line (&pos, end_pt))
              goto bail;

            if (   bufpt_looking_at_re (&pos, end_pt, Regexp (blank_line), 0)
                || bufpt_looking_at_str_n (&pos, end_pt, "[[", 2)
                || ((0 == bufpt_col_tabifying (&pos)) && (bufpt_looking_at_str_n (&pos, end_pt, "*", 1))))
              break;
          }
      }

    if (bufpt_pos (&pos) > bufpt_pos (end_pt))
      {
        if (0 > bufpt_goto (&pos, bufpt_pos (end_pt)))
          goto bail;
      }
  }

  if (0 > bufpt_pass_blank_lines (&pos, end_pt))
    goto bail;

  if (0 > awiki_set_node_end (graph, node, &pos))
    goto bail;

  if (0 > bufpt_goto (start_pt, bufpt_pos (&pos)))
    goto bail;

  if (0 > bufpt_copy_from (&my_start, awiki_node_start (graph, node)))
    goto bail;

  if (0 > bufpt_copy_from (&my_end, awiki_node_end (graph, node)))
    goto bail;

  if (0 > awiki_treeparse_styled_text (graph, node, error_pt, &my_start, &my_end))
    goto bail;

  if (0 > awiki_set_node_type (graph, node, Identifier (paragraph)))
    goto bail;

  answer = 1;

 return_normally:

  if (0 > uninit_bufpt (&my_start))
    goto bail;
  if (0 > uninit_bufpt (&my_end))
    goto bail;
  if (0 > uninit_bufpt (&pos))
    goto bail;


  return answer;
}




/* arch-tag: Tom Lord Fri Oct 29 11:13:21 2004 (libawiki/node-paragraph.c)
 */
