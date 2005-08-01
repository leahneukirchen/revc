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
#include "awiki/libawiki-nodes/styled-text-html.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


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

static t_awiki_html_fns noteref_text_html_fns =
{
  (t_uchar *)"builtin 'noteref-text' HTML type",
  awiki_noteref_text_to_html
};

static t_awiki_html_type noteref_text_html_type = { &noteref_text_html_fns };


static t_awiki_html_fns notedef_text_html_fns =
{
  (t_uchar *)"builtin 'notedef-text' HTML type",
  awiki_notedef_text_to_html
};

static t_awiki_html_type notedef_text_html_type = { &notedef_text_html_fns };


static t_awiki_html_fns recursive_styled_text_html_fns =
{
  (t_uchar *)"builtin 'recursive_styled-text' HTML type",
  awiki_recursive_styled_text_to_html
};

static t_awiki_html_type recursive_styled_text_html_type = { &recursive_styled_text_html_fns };


static t_awiki_html_fns code_text_html_fns =
{
  (t_uchar *)"builtin 'code-text' HTML type",
  awiki_code_text_to_html
};

static t_awiki_html_type code_text_html_type = { &code_text_html_fns };


static t_awiki_html_fns url_text_html_fns =
{
  (t_uchar *)"builtin 'url-text' HTML type",
  awiki_url_text_to_html
};

static t_awiki_html_type url_text_html_type = { &url_text_html_fns };


static t_awiki_html_fns styled_text_html_fns =
{
  (t_uchar *)"builtin 'styled-text' HTML type",
  awiki_styled_text_to_html
};

static t_awiki_html_type styled_text_html_type = { &styled_text_html_fns };


static t_awiki_html_fns emphasized_text_html_fns =
{
  (t_uchar *)"builtin 'emphasized-text' HTML type",
  awiki_emphasized_text_to_html
};

static t_awiki_html_type emphasized_text_html_type = { &emphasized_text_html_fns };


static t_awiki_html_fns stressed_text_html_fns =
{
  (t_uchar *)"builtin 'stressed-text' HTML type",
  awiki_stressed_text_to_html
};

static t_awiki_html_type stressed_text_html_type = { &stressed_text_html_fns };


int
awiki_init_node_styled_text_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier (noteref_text), &noteref_text_html_type))
        return -1;
      if (0 > awiki_register_html_type (Identifier (notedef_text), &notedef_text_html_type))
        return -1;
      if (0 > awiki_register_html_type (Identifier (url_text), &url_text_html_type))
        return -1;
      if (0 > awiki_register_html_type (Identifier (code_text), &code_text_html_type))
        return -1;
      if (0 > awiki_register_html_type (Identifier (recursive_styled_text), &recursive_styled_text_html_type))
        return -1;
      if (0 > awiki_register_html_type (Identifier (styled_text), &styled_text_html_type))
        return -1;
      if (0 > awiki_register_html_type (Identifier (emphasized_text), &emphasized_text_html_type))
        return -1;
      if (0 > awiki_register_html_type (Identifier (stressed_text), &stressed_text_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_styled_text_to_html (t_awiki_html_state * state,
                           t_awiki_html_type * type,
                           ssize_t graph,
                           ssize_t node,
                           t_buffer_point * error_pt,
                           t_buffer_point * output_pt)
{
  if (!state)
    return -1;

  {
    t_buffer_point * start;
    t_buffer_point * end;
    t_uchar * str;
    ssize_t len;

    start = awiki_node_start (graph, node);
    end = awiki_node_end (graph, node);
    len = bufpt_pos (end) - bufpt_pos (start);
    str = buf_range (bufpt_session (start),
                     bufpt_buffer (start), 
                     bufpt_pos (start), 
                     len);
    if ((len < 0) || (len && !str))
      return -1;

    if (0 > awiki_pt_insert_html_quoting (output_pt, str, len))
      return -1;
  }

  return 0;

}


int
awiki_code_text_to_html (t_awiki_html_state * state,
                         t_awiki_html_type * type,
                         ssize_t graph,
                         ssize_t node,
                         t_buffer_point * error_pt,
                         t_buffer_point * output_pt)
{
  if (!state)
    return -1;

  if (0 > awiki_html_directive (state, output_pt, "code"))
    return -1;

  {
    t_buffer_point * start;
    t_buffer_point * end;
    t_uchar * str;
    ssize_t len;

    start = awiki_node_start (graph, node);
    end = awiki_node_end (graph, node);
    len = bufpt_pos (end) - bufpt_pos (start);
    str = buf_range (bufpt_session (start),
                     bufpt_buffer (start), 
                     bufpt_pos (start), 
                     len);
    if ((len < 0) || (len && !str))
      return -1;

    if (0 > awiki_pt_insert_html_quoting (output_pt, str, len))
      return -1;
  }  

  if (0 > awiki_html_directive (state, output_pt, "/code"))
    return -1;
  return 0;
}


int
awiki_url_text_to_html (t_awiki_html_state * state,
                        t_awiki_html_type * type,
                        ssize_t graph,
                        ssize_t node,
                        t_buffer_point * error_pt,
                        t_buffer_point * output_pt)
{
  if (!state)
    return -1;

  {
    t_buffer_point * start;
    t_buffer_point * end;
    t_uchar * str;
    ssize_t len;

    start = awiki_node_start (graph, node);
    end = awiki_node_end (graph, node);
    len = bufpt_pos (end) - bufpt_pos (start);
    str = buf_range (bufpt_session (start),
                     bufpt_buffer (start), 
                     bufpt_pos (start), 
                     len);
    if ((len < 0) || (len && !str))
      return -1;

    if (0 > awiki_html_link_str_n (state, output_pt, str, len))
      return -1;
    
    if (!digraph_n_subnodes (graph, node))
      {
        if (0 > awiki_pt_insert_html_quoting (output_pt, str, len))
          return -1;
      }
    else
      {
        ssize_t bound;
        ssize_t x;

        bound = digraph_n_subnodes (graph, node);
      
        for (x = 0; x < bound; ++x)
          {
            ssize_t subnode;
            int q;

            subnode = digraph_subnode_data_k (graph, node, x);
            if (subnode < 0)
              return -1;
            if (!awiki_node_type (graph, subnode))
              continue;
            q = awiki_to_html (state, graph, subnode, error_pt, output_pt);

            if (0 > q)
              return -1;
          }
      }

    if (0 > awiki_html_directive (state, output_pt, "/a"))
      return -1;

  }  
  return 0;
}


int
awiki_noteref_text_to_html (t_awiki_html_state * state,
                            t_awiki_html_type * type,
                            ssize_t graph,
                            ssize_t node,
                            t_buffer_point * error_pt,
                            t_buffer_point * output_pt)
{
  if (!state)
    return -1;

  {
    t_buffer_point * start;
    t_buffer_point * end;
    t_uchar * str;
    ssize_t len;

    start = awiki_node_start (graph, node);
    end = awiki_node_end (graph, node);
    len = bufpt_pos (end) - bufpt_pos (start);
    str = buf_range (bufpt_session (start),
                     bufpt_buffer (start), 
                     bufpt_pos (start), 
                     len);
    if ((len < 0) || (len && !str))
      return -1;

    if (0 > awiki_html_anchor_link_str_n (state, output_pt, str, len))
      return -1;
    
    if (!digraph_n_subnodes (graph, node))
      {
        if (0 > awiki_pt_insert_html_quoting (output_pt, str, len))
          return -1;
      }
    else
      {
        ssize_t bound;
        ssize_t x;

        bound = digraph_n_subnodes (graph, node);
      
        for (x = 0; x < bound; ++x)
          {
            ssize_t subnode;
            int q;

            subnode = digraph_subnode_data_k (graph, node, x);
            if (subnode < 0)
              return -1;
            if (!awiki_node_type (graph, subnode))
              continue;
            q = awiki_to_html (state, graph, subnode, error_pt, output_pt);

            if (0 > q)
              return -1;
          }
      }

    if (0 > awiki_html_directive (state, output_pt, "/a"))
      return -1;

  }  
  return 0;
}


int
awiki_notedef_text_to_html (t_awiki_html_state * state,
                            t_awiki_html_type * type,
                            ssize_t graph,
                            ssize_t node,
                            t_buffer_point * error_pt,
                            t_buffer_point * output_pt)
{
  if (!state)
    return -1;

  {
    t_buffer_point * start;
    t_buffer_point * end;
    t_uchar * str;
    ssize_t len;

    start = awiki_node_start (graph, node);
    end = awiki_node_end (graph, node);
    len = bufpt_pos (end) - bufpt_pos (start);
    str = buf_range (bufpt_session (start),
                     bufpt_buffer (start), 
                     bufpt_pos (start), 
                     len);
    if ((len < 0) || (len && !str))
      return -1;

    if (0 > awiki_html_anchor_name_str_n (state, output_pt, str, len))
      return -1;
    
    if (!digraph_n_subnodes (graph, node))
      {
        if (0 > bufpt_insert_str_n (output_pt, "<b><code>{", 10))
          return -1;
        if (0 > awiki_pt_insert_html_quoting (output_pt, str, len))
          return -1;
        if (0 > bufpt_insert_str_n (output_pt, "}</code></b>", 12))
          return -1;
      }
    else
      {
        ssize_t bound;
        ssize_t x;

        bound = digraph_n_subnodes (graph, node);
      
        for (x = 0; x < bound; ++x)
          {
            ssize_t subnode;
            int q;

            subnode = digraph_subnode_data_k (graph, node, x);
            if (subnode < 0)
              return -1;
            if (!awiki_node_type (graph, subnode))
              continue;
            q = awiki_to_html (state, graph, subnode, error_pt, output_pt);

            if (0 > q)
              return -1;
          }
      }

    if (0 > awiki_html_directive (state, output_pt, "/a"))
      return -1;

  }  
  return 0;
}


int
awiki_recursive_styled_text_to_html (t_awiki_html_state * state,
                                     t_awiki_html_type * type,
                                     ssize_t graph,
                                     ssize_t node,
                                     t_buffer_point * error_pt,
                                     t_buffer_point * output_pt)
{
  if (!state)
    return -1;
  {
    ssize_t bound;
    ssize_t x;

    bound = digraph_n_subnodes (graph, node);
      
    for (x = 0; x < bound; ++x)
      {
        ssize_t subnode;
        int q;

        subnode = digraph_subnode_data_k (graph, node, x);
        if (subnode < 0)
          return -1;
        if (!awiki_node_type (graph, subnode))
          continue;
        q = awiki_to_html (state, graph, subnode, error_pt, output_pt);

        if (0 > q)
          return -1;
      }
  }  
  return 0;
}



int
awiki_emphasized_text_to_html (t_awiki_html_state * state,
                               t_awiki_html_type * type,
                               ssize_t graph,
                               ssize_t node,
                               t_buffer_point * error_pt,
                               t_buffer_point * output_pt)
{
  if (!state)
    return -1;

  if (0 > awiki_html_directive (state, output_pt, (state->emphasis_depth ? "big" : "b")))
    return -1;
  ++state->emphasis_depth;

  {
    ssize_t bound;
    ssize_t x;

    bound = digraph_n_subnodes (graph, node);
      
    for (x = 0; x < bound; ++x)
      {
        ssize_t subnode;
        int q;

        subnode = digraph_subnode_data_k (graph, node, x);
        if (subnode < 0)
          return -1;
        if (!awiki_node_type (graph, subnode))
          continue;
        q = awiki_to_html (state, graph, subnode, error_pt, output_pt);

        if (0 > q)
          return -1;
      }
  }  

  --state->emphasis_depth;

  if (0 > awiki_html_directive (state, output_pt, (state->emphasis_depth ? "/big" : "/b")))
    return -1;
  return 0;
}


int
awiki_stressed_text_to_html (t_awiki_html_state * state,
                             t_awiki_html_type * type,
                             ssize_t graph,
                             ssize_t node,
                             t_buffer_point * error_pt,
                             t_buffer_point * output_pt)
{
  t_uchar * open;
  t_uchar * close;

  if (!state)
    return -1;

  switch (state->stress_depth)
    {
    case 0:
      open = "i";
      close = "/i";
      break;
    case 1:
      open = "u";
      close = "/u";
      break;
    default:
      open = "big";
      close = "/big";
      break;
    }

  if (0 > awiki_html_directive (state, output_pt, open))
    return -1;
  ++state->stress_depth;

  {
    ssize_t bound;
    ssize_t x;

    bound = digraph_n_subnodes (graph, node);
      
    for (x = 0; x < bound; ++x)
      {
        ssize_t subnode;
        int q;

        subnode = digraph_subnode_data_k (graph, node, x);
        if (subnode < 0)
          return -1;
        if (!awiki_node_type (graph, subnode))
          continue;
        q = awiki_to_html (state, graph, subnode, error_pt, output_pt);

        if (0 > q)
          return -1;
      }
  }  

  --state->stress_depth;

  if (0 > awiki_html_directive (state, output_pt, close))
    return -1;
  return 0;
}





/* arch-tag: Tom Lord Sun Nov 21 16:24:45 2004 (libawiki-nodes/styled-text-html.c)
 */
