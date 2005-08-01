/* paper.c: 
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
#include "awiki/libawiki-nodes/paper.h"



Identifier_safe_decl (paper, "paper");
Identifier_safe_decl (title, "title");
Identifier_safe_decl (authors, "authors");
Identifier_safe_decl (abstract, "abstract");
Identifier_safe_decl (outline, "outline");



static t_awiki_node_fns paper_type_fns =
{
  (t_uchar *)"builtin 'paper' type",
  awiki_parse_paper
};

static t_awiki_node_type paper_type = { &paper_type_fns };



int
awiki_init_node_paper (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier_paper (), &paper_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_paper (t_awiki_node_type * node_type,
                   ssize_t graph,
                   ssize_t node,
                   t_buffer_point * error_pt,
                   t_buffer_point * start_pt,
                   t_buffer_point * end_pt)
{
  /* A paper consists of:
   *
   *   headers
   * 
   *   title
   * 
   *   authors
   * 
   *   abstract
   * 
   *   outline
   * 
   *   sections
   *
   * where each section is optional.
   * 
   * Every input text is a valid paper consisting, if nothing 
   * else (and if not empty) of just a "lead" section.
   * An empty (or blank) input text defines an empty paper.
   * 
   * The headers section matches our crazy RFC822++ syntax
   * and thus permits blank lines, header block terminator 
   * lines, headers whose names do not start in column 
   * 0 and headers whose values contain blank lines.
   * 
   * The title, authors, and abstract are displays of kinds
   * 'title', 'author', and 'abstract', respectively.  Thus,
   * they start with something like '[[title' and are 
   * supposed to end with a similarly indented ']]' containing
   * relatively indented lines in between. 
   * 
   * The lead is a list of paragraph (including paragraphs which
   * are displays), uinterrupted by any emacs-outline-style 
   * header lines that start in column 0.  It is parsed as an 
   * 'anonymous-section' node.
   * 
   * The sections part of a paper stretch from outline-style header,
   * starting in column 0, to outline-style header of equal or lesser
   * depth, starting in column 0.  These are are parse as 'section'
   * nodes.  (Each non-empty section contains, recursively, a lead
   * 'anonymous' section followed optional by (nested) outline
   * sections.)
   */

  ssize_t title_node;
  ssize_t authors_node;
  ssize_t abstract_node;
  ssize_t lead_node;
  t_buffer_point title_start;
  t_buffer_point authors_start;
  t_buffer_point abstract_start;

  title_node = digraph_alloc_node (graph);
  authors_node = digraph_alloc_node (graph);
  abstract_node = digraph_alloc_node (graph);
  lead_node = digraph_alloc_node (graph);
  init_bufpt (&title_start);
  init_bufpt (&authors_start);
  init_bufpt (&abstract_start);

  if ((title_node < 0) || (authors_node < 0) || (abstract_node < 0)  || (lead_node < 0))
    {
    error_return:
      (void)uninit_bufpt (&title_start);
      (void)uninit_bufpt (&authors_start);
      (void)uninit_bufpt (&abstract_start);
      return -1;
    }

  if (   (0 > digraph_insert_edge (graph, node, 0, title_node))
      || (0 > digraph_insert_edge (graph, node, 1, authors_node))
      || (0 > digraph_insert_edge (graph, node, 2, abstract_node))
      || (0 > digraph_insert_edge (graph, node, 3, lead_node)))
    goto error_return;

  if (0 > bufpt_pass_whitespace (start_pt, end_pt))
    goto error_return;

  if (0 > bufpt_copy_from (&title_start, start_pt))
    goto error_return;

  {
    int res;

    res = awiki_constrained_display_scan (Identifier(title), start_pt, end_pt);

    if (res < 0)
      goto error_return;
    else if (res)
      {
        if (0 > awiki_parse_for_display (graph, title_node, error_pt, &title_start, start_pt))
          goto error_return;
      }
  }



  if (0 > bufpt_pass_whitespace (start_pt, end_pt))
    goto error_return;

  if (0 > bufpt_copy_from (&authors_start, start_pt))
    goto error_return;

  {
    int res;

    res = awiki_constrained_display_scan (Identifier(authors), start_pt, end_pt);

    if (res < 0)
      goto error_return;
    else if (res)
      {
        if (0 > awiki_parse_for_display (graph, authors_node, error_pt, &authors_start, start_pt))
          goto error_return;
      }
  }


  if (0 > bufpt_pass_whitespace (start_pt, end_pt))
    goto error_return;

  if (0 > bufpt_copy_from (&abstract_start, start_pt))
    goto error_return;

  {
    int res;

    res = awiki_constrained_display_scan (Identifier(abstract), start_pt, end_pt);

    if (res < 0)
      goto error_return;
    else if (res)
      {
        if (0 > awiki_parse_for_display (graph, abstract_node, error_pt, &authors_start, start_pt))
          goto error_return;
      }
  }

  if (0 > awiki_parse_for_type (Identifier (outline), graph, lead_node, error_pt, start_pt, end_pt))
    return -1;

  if (0 > awiki_set_node_type (graph, node, Identifier (paper)))
    return -1;

  if (0 > uninit_bufpt (&title_start))
    goto error_return;
  if (0 > uninit_bufpt (&authors_start))
    goto error_return;
  if (0 > uninit_bufpt (&abstract_start))
    goto error_return;

  return 1;
}



/* arch-tag: Tom Lord Fri Oct 29 09:27:57 2004 (libawiki/node-paper.c)
 */
