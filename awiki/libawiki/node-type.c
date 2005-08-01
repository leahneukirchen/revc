/* node-type.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/properties/property.h"
#include "hackerlab/graphs/digraph.h"
#include "awiki/libawiki/node-type.h"


/* typedefs */


/* __STDC__ prototypes for static functions */
static ssize_t node_type_prop_id (void);


/* declarations */


/* code */

t_uchar *
awiki_node_type (ssize_t graph,
                 ssize_t node)
{
  t_uchar ** type_box;

  type_box = digraph_property_soft_ref (graph, sizeof (t_uchar *), node, node_type_prop_id ());
  if (!type_box)
    return 0;

  return *type_box;
}


static ssize_t
node_type_prop_id (void)
{
  static ssize_t id = -1;
  if (id < 0)
    id = property_global_id (&node_type_prop_id);
  return id;
}


int
awiki_set_node_type (ssize_t graph,
                     ssize_t node,
                     const t_uchar * type_atom)
{
  const t_uchar ** type_box;

  type_box = digraph_property_ref (graph, sizeof (t_uchar *), node, node_type_prop_id (), 0);
  if (!type_box)
    return -1;

  *type_box = type_atom;

  return 0;
}



/* arch-tag: Tom Lord Sun Nov 21 11:38:27 2004 (node-type.c)
 */
