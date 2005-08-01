/* null.c: 
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
#include "awiki/libawiki-nodes/null.h"
#include "awiki/libawiki-nodes/null-html.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (null, "null");



/* code */

static t_awiki_html_fns null_html_fns =
{
  (t_uchar *)"builtin 'null' HTML type",
  awiki_null_to_html
};

static t_awiki_html_type null_html_type = { &null_html_fns };


int
awiki_init_node_null_html (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_html_type (Identifier (null), &null_html_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_null_to_html (t_awiki_html_state * state,
                     t_awiki_html_type * type,
                     ssize_t graph,
                     ssize_t node,
                     t_buffer_point * error_pt,
                     t_buffer_point * output_pt)
{
  return 0;
}



/* arch-tag: Tom Lord Sun Nov 21 17:23:34 2004 (libawiki-nodes/null-html.c)
 */
