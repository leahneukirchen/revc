/* html.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__AWIKI_HTML_H
#define INCLUDE__LIBAWIKI__AWIKI_HTML_H


#include "hackerlab/buffers/buffers.h"
#include "hackerlab/graphs/digraph.h"
#include "awiki/libawiki/parse.h"



typedef struct awiki_html_state t_awiki_html_state;
typedef struct awiki_html_type t_awiki_html_type;
typedef struct awiki_html_fns t_awiki_html_fns;


typedef int (*t_awiki_to_html_fn) (t_awiki_html_state * state,
                                   t_awiki_html_type * type,
                                   ssize_t graph,
                                   ssize_t node,
                                   t_buffer_point * error_pt,
                                   t_buffer_point * output_pt);



struct awiki_html_state
{
  t_uchar * root_prefix;
  ssize_t indent_level;
  ssize_t emphasis_depth;
  ssize_t stress_depth;
  ssize_t section_depth;
};


struct awiki_html_type
{
  t_awiki_html_fns * fns;
};


struct awiki_html_fns
{
  t_uchar * debugger_name;
  t_awiki_to_html_fn to_html;
};



#define AWIKI_HTML_MULTILINE(STATE,PT,STRING) if (0 > awiki_html_multiline ((STATE), (PT), (STRING))) return -1
#define AWIKI_HTML_MULTILINE_INLINE(STATE,PT,STRING) if (0 > awiki_html_multiline_inline ((STATE), (PT), (STRING))) return -1
#define AWIKI_HTML_CLOSE_MULTILINE(STATE,PT,STRING) if (0 > awiki_html_close_multiline ((STATE), (PT), (STRING))) return -1
#define AWIKI_HTML_SILENT_CLOSE_MULTILINE(STATE,PT,STRING) if (0 > awiki_html_silent_close_multiline ((STATE), (PT), (STRING))) return -1




/* automatically generated __STDC__ prototypes */
extern int awiki_register_html_type (const t_uchar * name_id,
                                     t_awiki_html_type * type);
extern t_awiki_html_type * awiki_find_html_type (t_uchar * name_id);
extern t_awiki_html_type * awiki_node_html_type (ssize_t graph, ssize_t node);
extern int init_awiki_init_html_state (t_awiki_html_state * state,
                                       const t_uchar * root_prefix);
extern void uninit_awiki_html_state (t_awiki_html_state * state);
extern int awiki_to_html (t_awiki_html_state * state,
                          ssize_t graph,
                          ssize_t node,
                          t_buffer_point * error_pt,
                          t_buffer_point * output_pt);
extern int awiki_html_directive (t_awiki_html_state * state,
                                 t_buffer_point * pt,
                                 t_uchar * directive);
extern int awiki_pt_insert_html_quoting (t_buffer_point * output_pt,
                                         t_uchar * s,
                                         ssize_t len);
extern int awiki_html_newline_indent (t_awiki_html_state * state,
                                      t_buffer_point * pt);
extern int awiki_html_newline (t_awiki_html_state * state,
                               t_buffer_point * pt);
extern int awiki_html_indent (t_awiki_html_state * state,
                              t_buffer_point * pt);
extern int awiki_html_multiline (t_awiki_html_state * state,
                                 t_buffer_point * pt,
                                 t_uchar * str);
extern int awiki_html_close_multiline (t_awiki_html_state * state,
                                       t_buffer_point * pt,
                                       t_uchar * str);
extern int awiki_html_silent_close_multiline (t_awiki_html_state * state,
                                              t_buffer_point * pt,
                                              t_uchar * str);
extern int awiki_html_link_str_n (t_awiki_html_state * state,
                                  t_buffer_point * pt,
                                  t_uchar * str,
                                  ssize_t len);
extern int awiki_html_anchor_link_str_n (t_awiki_html_state * state,
                                         t_buffer_point * pt,
                                         t_uchar * str,
                                         ssize_t len);
extern int awiki_html_anchor_name_str_n (t_awiki_html_state * state,
                                         t_buffer_point * pt,
                                         t_uchar * str,
                                         ssize_t len);
#endif  /* INCLUDE__LIBAWIKI__AWIKI_HTML_H */


/* arch-tag: Tom Lord Thu Oct 28 14:47:09 2004 (awiki-html.h)
 */
