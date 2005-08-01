/* node-pts.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/graphs/digraph.h"
#include "awiki/libawiki/node-pts.h"


/* typedefs */


/* __STDC__ prototypes for static functions */
static ssize_t node_start_prop_id (void);
static ssize_t node_end_prop_id (void);
static int set_node_pt_prop (ssize_t graph,
                             ssize_t node,
                             t_buffer_point * where,
                             ssize_t prop_id,
                             t_property_type * prop_type);
static void node_pt_init_fn (t_property_type * type,
                             alloc_limits limits,
                             ssize_t pos,
                             void * mem);
static void node_pt_uninit_fn (t_property_type * type,
                               alloc_limits limits,
                               ssize_t pos,
                               void * mem);


/* declarations */

static t_property_fns node_pt_fns =
{
  "node_pt",
  sizeof (t_buffer_point),
  node_pt_init_fn,
  node_pt_uninit_fn
};

static t_property_type node_pt_type = { &node_pt_fns };



/* code */

t_buffer_point *
awiki_node_start (ssize_t graph,
                  ssize_t node)
{
  return (t_buffer_point *)digraph_property_soft_ref (graph, sizeof (t_buffer_point), node, node_start_prop_id ());
}


t_buffer_point *
awiki_node_end (ssize_t graph,
                ssize_t node)
{
  return (t_buffer_point *)digraph_property_soft_ref (graph, sizeof (t_buffer_point), node, node_end_prop_id ());
}


int
awiki_set_node_start (ssize_t graph,
                      ssize_t node,
                      t_buffer_point * where)
{
  return set_node_pt_prop (graph, node, where, node_start_prop_id (), &node_pt_type);
}


int
awiki_set_node_end (ssize_t graph,
                    ssize_t node,
                    t_buffer_point * where)
{
  return set_node_pt_prop (graph, node, where, node_end_prop_id (), &node_pt_type);
}




static ssize_t
node_start_prop_id (void)
{
  static ssize_t id = -1;
  if (id < 0)
    id = property_global_id (&node_start_prop_id);
  return id;
}


static ssize_t
node_end_prop_id (void)
{
  static ssize_t id = -1;
  if (id < 0)
    id = property_global_id (&node_end_prop_id);
  return id;
}


static int
set_node_pt_prop (ssize_t graph,
                  ssize_t node,
                  t_buffer_point * where,
                  ssize_t prop_id,
                  t_property_type * prop_type)
{
  t_buffer_point * pt;

  pt = digraph_property_ref (graph, sizeof (t_buffer_point), node, prop_id, prop_type);
  if (!pt)
    return -1;

  if (0 > bufpt_copy_from (pt, where))
    return -1;

  return 0;
}




static void
node_pt_init_fn (t_property_type * type,
                 alloc_limits limits,
                 ssize_t pos,
                 void * mem)
{
  t_buffer_point * pt;

  pt = (t_buffer_point *)mem;
  init_bufpt (pt);
}


static void
node_pt_uninit_fn (t_property_type * type,
                   alloc_limits limits,
                   ssize_t pos,
                   void * mem)
{
  t_buffer_point * pt;

  pt = (t_buffer_point *)mem;
  if (0 > uninit_bufpt (pt))
    {
      panic("unable to maintain error handling invariant");
    }
}



/* arch-tag: Tom Lord Sun Nov 21 11:46:42 2004 (node-pts.c)
 */
