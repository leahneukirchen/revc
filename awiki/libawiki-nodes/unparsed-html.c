/* unparsed-html.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "awiki/libawiki-nodes/unparsed-html.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_decl(unparsed, "unparsed");

static t_awiki_html_fns unparsed_html_fns =
{
  (t_uchar *)"builtin 'unparsed' HTML type",
  awiki_unparsed_to_html
};

static t_awiki_html_type unparsed_html_type = { &unparsed_html_fns };



/* code */

int
awiki_init_node_unparsed_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier_unparsed (), &unparsed_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}




int
awiki_unparsed_to_html (t_awiki_html_state * state,
                        t_awiki_html_type * type,
                        ssize_t graph,
                        ssize_t node,
                        t_buffer_point * error_pt,
                        t_buffer_point * output_pt)
{
  if (!state)
    return -1;

  AWIKI_HTML_MULTILINE (state, output_pt, "pre");
  {
    t_buffer_point * start;
    t_buffer_point * end;
    t_uchar * str;
    ssize_t len;

    start = awiki_node_start (graph, node);
    end = awiki_node_end (graph, node);
    len = bufpt_pos (end) - bufpt_pos (start);
    if (len)
      {
        str = buf_range (bufpt_session (start),
                         bufpt_buffer (start), 
                         bufpt_pos (start), 
                         len);
        if (!str)
          return -1;
        if (0 > awiki_pt_insert_html_quoting (output_pt, str, len))
          return -1;
      }
  }
  AWIKI_HTML_CLOSE_MULTILINE (state, output_pt, "pre");

  return 0;
}




/* arch-tag: Tom Lord Sun Nov 21 11:29:07 2004 (unparsed-html.c)
 */
