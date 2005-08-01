/* styled-text.c: 
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
#include "awiki/libawiki-nodes/styled-text.h"


/* typedefs */


/* __STDC__ prototypes for static functions */



static int parse_asymmetric_repeatable_delimited_text_frob (const t_uchar * type,
                                                            ssize_t graph,
                                                            ssize_t node,
                                                            t_buffer_point * error_pt,
                                                            t_buffer_point * start_pt,
                                                            t_buffer_point * end_pt,
                                                            t_uchar end_frob_c);
static int parse_symmetric_repeatable_delimited_text_frob (const t_uchar * type,
                                                           ssize_t graph,
                                                           ssize_t node,
                                                           t_buffer_point * error_pt,
                                                           t_buffer_point * start_pt,
                                                           t_buffer_point * end_pt);


/* declarations */

Identifier_safe_decl (code_text, "code-text");
Identifier_safe_decl (styled_text, "styled-text");
Identifier_safe_decl (recursive_styled_text, "recursive-styled-text");
Identifier_safe_decl (emphasized_text, "emphasized-text");
Identifier_safe_decl (stressed_text, "stressed-text");
Identifier_safe_decl (url_text, "url-text");
Identifier_safe_decl (noteref_text, "noteref-text");
Identifier_safe_decl (notedef_text, "notedef-text");


/* code */

static t_awiki_node_fns noteref_text_type_fns =
{
  (t_uchar *)"builtin 'noteref-text' type",
  awiki_parse_noteref_text,
};

static t_awiki_node_type noteref_text_type = { &noteref_text_type_fns };


static t_awiki_node_fns notedef_text_type_fns =
{
  (t_uchar *)"builtin 'notedef-text' type",
  awiki_parse_noteref_text,     /* yes, noteref */
};

static t_awiki_node_type notedef_text_type = { &notedef_text_type_fns };


static t_awiki_node_fns recursive_styled_text_type_fns =
{
  (t_uchar *)"builtin 'recursive_styled-text' type",
  awiki_parse_recursive_styled_text
};

static t_awiki_node_type recursive_styled_text_type = { &recursive_styled_text_type_fns };


static t_awiki_node_fns code_text_type_fns =
{
  (t_uchar *)"builtin 'code-text' type",
  awiki_parse_code_text
};

static t_awiki_node_type code_text_type = { &code_text_type_fns };


static t_awiki_node_fns url_text_type_fns =
{
  (t_uchar *)"builtin 'url-text' type",
  awiki_parse_url_text
};

static t_awiki_node_type url_text_type = { &url_text_type_fns };


static t_awiki_node_fns styled_text_type_fns =
{
  (t_uchar *)"builtin 'styled-text' type",
  awiki_parse_styled_text
};

static t_awiki_node_type styled_text_type = { &styled_text_type_fns };


static t_awiki_node_fns emphasized_text_type_fns =
{
  (t_uchar *)"builtin 'emphasized-text' type",
  awiki_parse_emphasized_text
};

static t_awiki_node_type emphasized_text_type = { &emphasized_text_type_fns };


static t_awiki_node_fns stressed_text_type_fns =
{
  (t_uchar *)"builtin 'stressed-text' type",
  awiki_parse_stressed_text
};

static t_awiki_node_type stressed_text_type = { &stressed_text_type_fns };



int
awiki_init_node_styled_text (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier (noteref_text), &noteref_text_type))
        return -1;
      if (0 > awiki_register_type (Identifier (notedef_text), &notedef_text_type))
        return -1;
      if (0 > awiki_register_type (Identifier (url_text), &url_text_type))
        return -1;
      if (0 > awiki_register_type (Identifier (code_text), &code_text_type))
        return -1;
      if (0 > awiki_register_type (Identifier (recursive_styled_text), &recursive_styled_text_type))
        return -1;
      if (0 > awiki_register_type (Identifier (styled_text), &styled_text_type))
        return -1;
      if (0 > awiki_register_type (Identifier (emphasized_text), &emphasized_text_type))
        return -1;
      if (0 > awiki_register_type (Identifier (stressed_text), &stressed_text_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_treeparse_styled_text (ssize_t graph,
                             ssize_t node,
                             t_buffer_point * error_pt,
                             t_buffer_point * start_pt,
                             t_buffer_point * end_pt)
{
  ssize_t subnode;

  if (0 > bufpt_pass_whitespace (start_pt, end_pt))
    return -1;

 tail_recurse:

  if (0 < bufpt_is_at_end (start_pt, end_pt))
    return 0;

  subnode = digraph_alloc_node (graph);
  if (subnode < 0)
    return -1;

  if (0 > digraph_insert_edge (graph, node, digraph_n_subnodes (graph, node), subnode))
    {
      (void)digraph_free_node (graph, subnode);
      return -1;
    }

  if (bufpt_char_at (start_pt, end_pt) == '/')
    {
      /* An emphasized burst of text.
       */
      if (0 > parse_symmetric_repeatable_delimited_text_frob (Identifier (emphasized_text), graph, subnode, error_pt, start_pt, end_pt))
        return -1;
    }
  else if (bufpt_char_at (start_pt, end_pt) == '\\')
    {
      /* An emphasized burst of text.
       */
      if (0 > parse_symmetric_repeatable_delimited_text_frob (Identifier (emphasized_text), graph, subnode, error_pt, start_pt, end_pt))
        return -1;
    }
  else if (bufpt_char_at (start_pt, end_pt) == '^')
    {
      /* An stressed burst of text.
       */
      if (0 > parse_symmetric_repeatable_delimited_text_frob (Identifier (stressed_text), graph, subnode, error_pt, start_pt, end_pt))
        return -1;
    }
  else if (bufpt_char_at (start_pt, end_pt) == '*')
    {
      /* An stressed burst of text.
       */
      if (0 > parse_symmetric_repeatable_delimited_text_frob (Identifier (stressed_text), graph, subnode, error_pt, start_pt, end_pt))
        return -1;
    }
  else if (bufpt_char_at (start_pt, end_pt) == '~')
    {
      /* An code burst of text.
       */
      if (0 > parse_symmetric_repeatable_delimited_text_frob (Identifier (code_text), graph, subnode, error_pt, start_pt, end_pt))
        return -1;
    }
  else if (bufpt_char_at (start_pt, end_pt) == '`')
    {
      /* An code burst of text.
       */
      if (0 > parse_asymmetric_repeatable_delimited_text_frob (Identifier (code_text), graph, subnode, error_pt, start_pt, end_pt, '\''))
        return -1;
    }
  else if (bufpt_char_at (start_pt, end_pt) == '<')
    {
      /* A url burst of text.
       */
      if (0 > parse_asymmetric_repeatable_delimited_text_frob (Identifier (url_text), graph, subnode, error_pt, start_pt, end_pt, '>'))
        return -1;
    }
  else if (bufpt_char_at (start_pt, end_pt) == '{')
    {
      /* A noteref burst of text.
       */
      if (0 > parse_asymmetric_repeatable_delimited_text_frob (Identifier (noteref_text), graph, subnode, error_pt, start_pt, end_pt, '}'))
        return -1;
    }
  else
    {
      if (0 > awiki_parse_for_type (Identifier (styled_text), graph, subnode, error_pt, start_pt, end_pt))
        return -1;
    }

  goto tail_recurse;
}


static int
parse_asymmetric_repeatable_delimited_text_frob (const t_uchar * type,
                                                 ssize_t graph,
                                                 ssize_t node,
                                                 t_buffer_point * error_pt,
                                                 t_buffer_point * start_pt,
                                                 t_buffer_point * end_pt,
                                                 t_uchar end_frob_c)
{
  int frob_c;
  ssize_t frob_width;
  t_buffer_point burst_body_start;
  t_buffer_point burst_body_end;

  frob_c = bufpt_char_at (start_pt, end_pt);
  if (frob_c < 0)
    return -1;

  frob_width = 0;
  while (   !bufpt_is_at_end (start_pt, end_pt)
         && (frob_c == bufpt_char_at (start_pt, end_pt)))
    {
      if (0 > bufpt_forward_char (start_pt, end_pt, 1))
        return -1;
      ++frob_width;
    }

  if (!frob_width)
    return -1;

  init_bufpt (&burst_body_start);
  init_bufpt (&burst_body_end);

  if (0 > bufpt_copy_from (&burst_body_start, start_pt))
    {
    bail:
      (void)uninit_bufpt (&burst_body_start);
      (void)uninit_bufpt (&burst_body_end);
      return -1;
    }

  if (0 > bufpt_copy_from (&burst_body_end, start_pt))
    goto bail;

  while (1)
    {
      ssize_t maybe_closing_frob_width;

      while (   !bufpt_is_at_end (start_pt, end_pt)
             && (end_frob_c != bufpt_char_at (start_pt, end_pt)))
        {
          if (0 > bufpt_forward_char (start_pt, end_pt, 1))
            goto bail;
        }

      maybe_closing_frob_width = 0;
      while (   (maybe_closing_frob_width < frob_width)
             && !bufpt_is_at_end (start_pt, end_pt)
             && (end_frob_c == bufpt_char_at (start_pt, end_pt)))
        {
          if (0 > bufpt_goto (start_pt, 1 + bufpt_pos (start_pt)))
            goto bail;
          ++maybe_closing_frob_width;
        }

      if (   (maybe_closing_frob_width == frob_width)
          && (   (0 < bufpt_is_at_end (start_pt, end_pt))
              || !char_is_alnum (bufpt_char_at (start_pt, end_pt))))
        {
          if (0 > bufpt_goto (&burst_body_end, -frob_width + bufpt_pos (start_pt)))
            goto bail;

        do_the_subparse:
          if (0 > awiki_parse_for_type (type, graph, node, error_pt, &burst_body_start, &burst_body_end))
            goto bail;

          if (0 > uninit_bufpt (&burst_body_start))
            goto bail;

          if (0 > uninit_bufpt (&burst_body_end))
            goto bail;

          return 0;
        }

      if (bufpt_is_at_end (start_pt, end_pt))
        {
          if (0 > bufpt_goto (&burst_body_end, bufpt_pos (start_pt)))
            goto bail;
          goto do_the_subparse;
        }
    }
}


static int
parse_symmetric_repeatable_delimited_text_frob (const t_uchar * type,
                                                ssize_t graph,
                                                ssize_t node,
                                                t_buffer_point * error_pt,
                                                t_buffer_point * start_pt,
                                                t_buffer_point * end_pt)
{
  int frob_c;

  frob_c = bufpt_char_at (start_pt, end_pt);
  if (frob_c < 0)
    return -1;

  return parse_asymmetric_repeatable_delimited_text_frob (type, graph, node, error_pt, start_pt, end_pt, frob_c);
}


int
awiki_parse_styled_text (t_awiki_node_type * node_type,
                         ssize_t graph,
                         ssize_t node,
                         t_buffer_point * error_pt,
                         t_buffer_point * start_pt,
                         t_buffer_point * end_pt)
{
  if (0 > awiki_set_node_type (graph, node, Identifier (styled_text)))
    return -1;

  if (0 > awiki_set_node_start (graph, node, start_pt))
    return -1;

  while (1)
    {
      int saw_alnum;

      saw_alnum = 0;
      while (   !bufpt_is_at_end (start_pt, end_pt)
             && char_is_alnum (bufpt_char_at (start_pt, end_pt)))
        {
          if (0 > bufpt_goto (start_pt, 1 + bufpt_pos (start_pt)))
            return -1;
          saw_alnum = 1;
        }

      if (bufpt_is_at_end (start_pt, end_pt))
        break;

      if (char_is_space (bufpt_char_at (start_pt, end_pt)))
        {
          if (0 > bufpt_pass_whitespace (start_pt, end_pt))
            return -1;
          continue;
        }

      if (saw_alnum)
        {
          if (0 > bufpt_goto (start_pt, 1 + bufpt_pos (start_pt)))
            return -1;
          continue;
        }

      {
        int done;
        done = 0;
        switch (bufpt_char_at (start_pt, end_pt))
          {
          case '/':
          case '\\':
          case '*':
          case '^':
          case '`':
          case '<':
          case '{':
            done = 1;
            break;
          }
        if (done)
          break;
        else
          {
            if (0 > bufpt_goto (start_pt, 1 + bufpt_pos (start_pt)))
              return -1;
            continue;
          }
      }
    }

  if (0 > awiki_set_node_end (graph, node, start_pt))
    return -1;

  return 0;
}


int
awiki_parse_emphasized_text (t_awiki_node_type * node_type,
                             ssize_t graph,
                             ssize_t node,
                             t_buffer_point * error_pt,
                             t_buffer_point * start_pt,
                             t_buffer_point * end_pt)
{
  if (0 > awiki_set_node_type (graph, node, Identifier (emphasized_text)))
    return -1;

  if (0 > awiki_set_node_start (graph, node, start_pt))
    return -1;

  if (0 > awiki_set_node_end (graph, node, end_pt))
    return -1;

  if (0 > awiki_treeparse_styled_text (graph, node, error_pt, start_pt, end_pt))
    return -1;

  return 0;
}


int
awiki_parse_code_text (t_awiki_node_type * node_type,
                       ssize_t graph,
                       ssize_t node,
                       t_buffer_point * error_pt,
                       t_buffer_point * start_pt,
                       t_buffer_point * end_pt)
{
  if (0 > awiki_set_node_type (graph, node, Identifier (code_text)))
    return -1;

  if (0 > awiki_set_node_start (graph, node, start_pt))
    return -1;

  if (0 > awiki_set_node_end (graph, node, end_pt))
    return -1;

  return 0;
}


int
awiki_parse_url_text (t_awiki_node_type * node_type,
                      ssize_t graph,
                      ssize_t node,
                      t_buffer_point * error_pt,
                      t_buffer_point * start_pt,
                      t_buffer_point * end_pt)
{
  if ('"' == bufpt_char_at (start_pt, end_pt))
    {
      ssize_t subnode;

      subnode = digraph_alloc_node (graph);
      if (subnode < 0)
        return -1;

      if (0 > digraph_insert_edge (graph, node, 0, subnode))
        return -1;

      if (0 > parse_symmetric_repeatable_delimited_text_frob (Identifier (recursive_styled_text), graph, subnode, error_pt, start_pt, end_pt))
        return -1;

      if (0 > bufpt_pass_whitespace (start_pt, end_pt))
        return -1;

      if (bufpt_looking_at_str_n (start_pt, end_pt, "--", 2))
        {
          if (0 > bufpt_goto (start_pt, 2 + bufpt_pos (start_pt)))
            return -1;
          if (0 > bufpt_pass_whitespace (start_pt, end_pt))
            return -1;
        }
    }

  if (0 > awiki_set_node_type (graph, node, Identifier (url_text)))
    return -1;

  if (0 > awiki_set_node_start (graph, node, start_pt))
    return -1;

  if (0 > awiki_set_node_end (graph, node, end_pt))
    return -1;


  if (0 > bufpt_goto (start_pt, bufpt_pos (end_pt)))
    return -1;

  return 0;
}


int
awiki_parse_noteref_text (t_awiki_node_type * node_type,
                          ssize_t graph,
                          ssize_t node,
                          t_buffer_point * error_pt,
                          t_buffer_point * start_pt,
                          t_buffer_point * end_pt)
{
  int is_defn;

  is_defn = ('*' == bufpt_char_at (start_pt, end_pt));
  if (is_defn)
    {
      if (0 > bufpt_goto (start_pt, 1 + bufpt_pos (start_pt)))
        return -1;
    }

  if ('"' == bufpt_char_at (start_pt, end_pt))
    {
      ssize_t subnode;

      subnode = digraph_alloc_node (graph);
      if (subnode < 0)
        return -1;

      if (0 > digraph_insert_edge (graph, node, 0, subnode))
        return -1;

      if (0 > parse_symmetric_repeatable_delimited_text_frob (Identifier (recursive_styled_text), graph, subnode, error_pt, start_pt, end_pt))
        return -1;

      if (0 > bufpt_pass_whitespace (start_pt, end_pt))
        return -1;

      if (bufpt_looking_at_str_n (start_pt, end_pt, "--", 2))
        {
          if (0 > bufpt_goto (start_pt, 2 + bufpt_pos (start_pt)))
            return -1;
          if (0 > bufpt_pass_whitespace (start_pt, end_pt))
            return -1;
        }
    }

  if (0 > awiki_set_node_type (graph, node, (is_defn? Identifier (notedef_text) : Identifier (noteref_text))))
    return -1;

  if (0 > awiki_set_node_start (graph, node, start_pt))
    return -1;

  if (0 > awiki_set_node_end (graph, node, end_pt))
    return -1;


  if (0 > bufpt_goto (start_pt, bufpt_pos (end_pt)))
    return -1;

  return 0;
}


int
awiki_parse_stressed_text (t_awiki_node_type * node_type,
                           ssize_t graph,
                           ssize_t node,
                           t_buffer_point * error_pt,
                           t_buffer_point * start_pt,
                           t_buffer_point * end_pt)
{
  if (0 > awiki_set_node_type (graph, node, Identifier (stressed_text)))
    return -1;

  if (0 > awiki_set_node_start (graph, node, start_pt))
    return -1;

  if (0 > awiki_set_node_end (graph, node, end_pt))
    return -1;

  if (0 > awiki_treeparse_styled_text (graph, node, error_pt, start_pt, end_pt))
    return -1;

  return 0;
}


int
awiki_parse_recursive_styled_text (t_awiki_node_type * node_type,
                                   ssize_t graph,
                                   ssize_t node,
                                   t_buffer_point * error_pt,
                                   t_buffer_point * start_pt,
                                   t_buffer_point * end_pt)
{
  if (0 > awiki_set_node_type (graph, node, Identifier (recursive_styled_text)))
    return -1;

  if (0 > awiki_set_node_start (graph, node, start_pt))
    return -1;

  if (0 > awiki_set_node_end (graph, node, end_pt))
    return -1;

  if (0 > awiki_treeparse_styled_text (graph, node, error_pt, start_pt, end_pt))
    return -1;

  return 0;
}



/* arch-tag: Tom Lord Wed Nov 17 16:59:56 2004 (libawiki-nodes/styled-text.c)
 */
