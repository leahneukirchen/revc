/* html.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "hackerlab/identifiers/identifier-table.h"
#include "awiki/libawiki/awiki.h"
#include "awiki/libawiki/html.h"


/* __STDC__ prototypes for static functions */



static t_identifier_table * id_table (void);
static t_awiki_html_type * id_type (const t_uchar * name_atom);


/* Declarations */

Regexp_decl(implicit_mailto, "^[^:/]*@[^:/]*$", REG_EXTENDED);




int
awiki_register_html_type (const t_uchar * name_id,
                          t_awiki_html_type * type)
{
  t_awiki_html_type ** type_box;

  if (!name_id)
    return -1;

  if (id_type (name_id))
    return -1;

  type_box = identifier_table_entry_ensure (id_table (), lim_use_malloc, sizeof (t_awiki_html_type *), name_id);
  if (!type_box)
    return -1;

  *type_box = type;
  return 0;
}


t_awiki_html_type *
awiki_find_html_type (t_uchar * name_id)
{
  t_awiki_html_type ** type_box;

  if (!name_id)
    return 0;

  type_box = (t_awiki_html_type **)identifier_table_entry_find (id_table (), lim_use_malloc, sizeof (t_awiki_html_type *), name_id);
  if (!type_box)
    return 0;

  return *type_box;
}


t_awiki_html_type *
awiki_node_html_type (ssize_t graph, ssize_t node)
{
  t_uchar * node_type;

  node_type = awiki_node_type (graph, node);
  if (!node_type)
    return 0;

  return awiki_find_html_type (node_type);
}



int
init_awiki_init_html_state (t_awiki_html_state * state,
                            const t_uchar * root_prefix)
{
  if (!state)
    return -1;

  mem_set0 ((t_uchar *)state, sizeof (*state));
  if (root_prefix)
    {
      state->root_prefix = str_save (lim_use_malloc, root_prefix);
      if (!state->root_prefix)
        return -1;
    }
  return 0;
}


void
uninit_awiki_html_state (t_awiki_html_state * state)
{
  if (state)
    {
      lim_free (lim_use_malloc, state->root_prefix);
      mem_set0 ((t_uchar *)state, sizeof (*state));
    }
}




int
awiki_to_html (t_awiki_html_state * state,
               ssize_t graph,
               ssize_t node,
               t_buffer_point * error_pt,
               t_buffer_point * output_pt)
{
  t_awiki_html_type * type;

  type = awiki_node_html_type (graph, node);
  if (!type || !type->fns || !type->fns->to_html)
    return -1;

  return type->fns->to_html (state, type, graph, node, error_pt, output_pt);
}



int
awiki_html_directive (t_awiki_html_state * state,
                      t_buffer_point * pt,
                      t_uchar * directive)
{
  if (!state || !pt)
    return -1;

  if (0 > bufpt_insert_str_n (pt, "<", 1))
    return -1;
  if (0 > bufpt_insert_str_n (pt, directive, str_length (directive)))
    return -1;
  if (0 > bufpt_insert_str_n (pt, ">", 1))
    return -1;

  return 0;
}


int
awiki_pt_insert_html_quoting (t_buffer_point * output_pt,
                              t_uchar * s,
                              ssize_t len)
{
  while (len)
    {
      ssize_t x;

      for (x = 0;
           (   (x < len)
            && (s[x] != '<')
            && (s[x] != '>')
            && (s[x] != '&')
            && (s[x] != '"'));
           ++x)
        ;

      if (x)
        {
          if (0 > bufpt_insert_str_n (output_pt, s, x))
            return -1;
          s += x;
          len -= x;
        }

      if (len)
        {
          t_uchar * entity;

          entity = ((s[0] == '<')
                    ? "&lt;"
                    : (s[0] == '>')
                    ? "&gt;"
                    : (s[0] == '&')
                    ? "&amp;"
                    : (s[0] == '"')
                    ? "&quot;"
                    : "[[[UNRECOGNIZEDCHARACTER]]]");

          if (0 > bufpt_insert_str_n (output_pt, entity, str_length (entity)))
            return -1;

          s += 1;
          len -= 1;
        }
    }

  return 0;
}




int
awiki_html_newline_indent (t_awiki_html_state * state,
                           t_buffer_point * pt)
{
  ssize_t x;
  ssize_t bound;

  if (!state || !pt)
    return -1;

  if (0 > bufpt_insert_str_n (pt, "\n", 1))
    return -1;

  bound = state->indent_level;
  for (x = 0; x < bound; ++x)
    {
      if (0 > bufpt_insert_str_n (pt, " ", 1))
        return -1;
    }
  return 0;
}


int
awiki_html_newline (t_awiki_html_state * state,
                    t_buffer_point * pt)
{
  if (!state || !pt)
    return -1;

  if (0 > bufpt_insert_str_n (pt, "\n", 1))
    return -1;

  return 0;
}


int
awiki_html_indent (t_awiki_html_state * state,
                   t_buffer_point * pt)
{
  ssize_t x;
  ssize_t bound;

  if (!state || !pt)
    return -1;

  bound = state->indent_level;
  for (x = 0; x < bound; ++x)
    {
      if (0 > bufpt_insert_str_n (pt, " ", 1))
        return -1;
    }
  return 0;
}



int
awiki_html_multiline (t_awiki_html_state * state,
                      t_buffer_point * pt,
                      t_uchar * str)
{
  if (!state || !pt || !str)
    return -1;

  if (0 > awiki_html_newline_indent (state, pt))
    return -1;
  if (0 > bufpt_insert_str_n (pt, "<", 1))
    return -1;
  if (0 > bufpt_insert_str_n (pt, str, str_length (str)))
    return -1;
  if (0 > bufpt_insert_str_n (pt, ">\n", 2))
    return -1;

  state->indent_level += 2;
  return 0;
}


int
awiki_html_close_multiline (t_awiki_html_state * state,
                            t_buffer_point * pt,
                            t_uchar * str)
{
  if (!state || !pt || !str)
    return -1;

  state->indent_level -= 2;

  if (0 > awiki_html_newline_indent (state, pt))
    return -1;
  if (0 > bufpt_insert_str_n (pt, "</", 2))
    return -1;
  if (0 > bufpt_insert_str_n (pt, str, str_length (str)))
    return -1;
  if (0 > bufpt_insert_str_n (pt, ">\n", 2))
    return -1;

  return 0;
}


int
awiki_html_silent_close_multiline (t_awiki_html_state * state,
                                   t_buffer_point * pt,
                                   t_uchar * str)
{
  if (!state || !pt || !str)
    return -1;

  state->indent_level -= 2;

  if (0 > awiki_html_newline (state, pt))
    return -1;
  if (0 > awiki_html_newline (state, pt))
    return -1;

  return 0;
}



int
awiki_html_link_str_n (t_awiki_html_state * state,
                       t_buffer_point * pt,
                       t_uchar * str,
                       ssize_t len)
{
  if (!state || !pt || !str)
    return -1;

  if (0 > bufpt_insert_str_n (pt, "<a href=\"", 9))
    return -1;
  if (!regnexec (Regexp(implicit_mailto), str, len, 0, 0, 0))
    {
      if (0 > bufpt_insert_str_n (pt, "mailto:", 7))
        return -1;
    }
  else if (len && (str[0] == '$'))
    {
      if (0 > bufpt_insert_str_n (pt, state->root_prefix, str_length (state->root_prefix)))
        return -1;
      str += 1;
      len -= 1;
    }
  if (0 > bufpt_insert_str_n (pt, str, len))
    return -1;
  if (0 > bufpt_insert_str_n (pt, "\">", 2))
    return -1;

  return 0;
}


int
awiki_html_anchor_link_str_n (t_awiki_html_state * state,
                              t_buffer_point * pt,
                              t_uchar * str,
                              ssize_t len)
{
  if (!state || !pt || !str)
    return -1;

  if (0 > bufpt_insert_str_n (pt, "<a href=\"", 9))
    return -1;
  if (0 > bufpt_insert_str_n (pt, "#", 1))
    return -1;
  if (0 > bufpt_insert_str_n (pt, str, len))
    return -1;
  if (0 > bufpt_insert_str_n (pt, "\">", 2))
    return -1;

  return 0;
}


int
awiki_html_anchor_name_str_n (t_awiki_html_state * state,
                              t_buffer_point * pt,
                              t_uchar * str,
                              ssize_t len)
{
  if (!state || !pt || !str)
    return -1;

  if (0 > bufpt_insert_str_n (pt, "<a name=\"", 9))
    return -1;
  if (0 > bufpt_insert_str_n (pt, str, len))
    return -1;
  if (0 > bufpt_insert_str_n (pt, "\">", 2))
    return -1;

  return 0;
}




static t_identifier_table *
id_table (void)
{
  static int initialized = 0;
  static t_identifier_table table;

  if (!initialized)
    {
      if (0 > init_identifier_table (&table, lim_use_malloc, sizeof (t_awiki_html_type *)))
        return 0;
      initialized = 1;
    }

  return &table;
}


static t_awiki_html_type *
id_type (const t_uchar * name_atom)
{
  return (t_awiki_html_type *)identifier_table_entry_find (id_table (), lim_use_malloc, sizeof (t_awiki_html_type *), name_atom);
}



/* arch-tag: Tom Lord Thu Oct 28 14:47:13 2004 (awiki-html.c)
 */
