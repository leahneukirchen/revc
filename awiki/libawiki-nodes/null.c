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


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Identifier_safe_decl (null, "null");



/* code */

static t_awiki_node_fns null_type_fns =
{
  (t_uchar *)"builtin 'null' type",
  awiki_parse_null
};

static t_awiki_node_type null_type = { &null_type_fns };


int
awiki_init_node_null (void)
{
  static int initialized = -1;

  if (initialized < 0)
    {
      if (0 > awiki_register_type (Identifier (null), &null_type))
        return -1;
      initialized = 0;
    }

  return initialized;
}



int
awiki_parse_null (t_awiki_node_type * node_type,
                   ssize_t graph,
                   ssize_t node,
                   t_buffer_point * error_pt,
                   t_buffer_point * his_start_pt,
                   t_buffer_point * his_end_pt)
{
  if (0 > awiki_set_node_type (graph, node, Identifier (null)))
    return -1;

  if (0 > bufpt_goto (his_start_pt, bufpt_pos (his_end_pt)))
    return -1;

  return 0;
}



/* arch-tag: Tom Lord Sat Nov 20 13:18:05 2004 (libawiki-nodes/null.c)
 */
