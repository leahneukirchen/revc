/* digraph.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/graphs/node-set.h"
#include "hackerlab/graphs/digraph.h"


/* typedefs */

OBTABLE_STATIC_TABLE(digraph, t_digraph);


/* __STDC__ prototypes for static functions */



ssize_t
open_digraph (alloc_limits limits,
              ssize_t representation_type)
{
  ssize_t n;
  t_digraph * d;

  n = digraph_alloc_id ();
  d = digraph_ref_id (n);
  if (!d)
    return -1;

  d->_limits = limits;
  if (0 > init_obnode_set (&d->_nodes, limits, representation_type))
    {
      digraph_free_id (n);
      return -1;
    }

  return n;
}

int
close_digraph (ssize_t n)
{
  t_digraph * d;

  d = digraph_ref_id (n);
  if (!d)
    return -1;

  if (0 > uninit_obnode_set (&d->_nodes, d->_limits))
    return -1;

  return 0;
}



alloc_limits
digraph_limits (ssize_t d)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return 0;

  return dg->_limits;
}


ssize_t
digraph_n_nodes (ssize_t d)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return 0;

  return obnode_set_n_nodes (&dg->_nodes, dg->_limits);
}


ssize_t
digraph_alloc_node (ssize_t d)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return -1;

  return obnode_set_alloc_node (&dg->_nodes, dg->_limits);
}


int
digraph_free_node (ssize_t d, ssize_t n)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return -1;

  return obnode_set_free_node (&dg->_nodes, dg->_limits, n);
}



ssize_t
digraph_n_subnodes (ssize_t d, ssize_t n)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return -1;

  return obnode_n_subnodes (&dg->_nodes, dg->_limits, n);
}


ssize_t
digraph_n_parent_nodes (ssize_t d, ssize_t n)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return -1;

  return obnode_n_parent_nodes (&dg->_nodes, dg->_limits, n);
}


ssize_t
digraph_subnode_data_k (ssize_t d, ssize_t n, ssize_t k)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return -1;

  return obnode_subnode_data_k (&dg->_nodes, dg->_limits, n, k);
}


ssize_t
digraph_parent_data_k (ssize_t * out_slot,
                       ssize_t d,
                       ssize_t n,
                       ssize_t k)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return -1;

  return obnode_parent_data_k (out_slot, &dg->_nodes, dg->_limits, n, k);
}

int
digraph_insert_edge (ssize_t d,
                     ssize_t source,
                     ssize_t where,
                     ssize_t dest)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return -1;

  return obnode_insert_edge (&dg->_nodes, dg->_limits, source, where, dest);
}


int
digraph_delete_edge (ssize_t d,
                     ssize_t source,
                     ssize_t where)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return -1;

  return obnode_delete_edge (&dg->_nodes, dg->_limits, source, where);
}


void *
digraph_property_soft_ref (ssize_t d,
                           ssize_t elt_size,
                           ssize_t node,
                           ssize_t global_property_id)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return 0;

  return obnode_property_soft_ref (&dg->_nodes, dg->_limits, elt_size, node, global_property_id);
}


void *
digraph_property_ref (ssize_t d,
                      ssize_t elt_size,
                      ssize_t node,
                      ssize_t global_property_id,
                      t_property_type * type)
{
  t_digraph * dg;

  dg = digraph_ref_id (d);
  if (!dg)
    return 0;

  return obnode_property_ref (&dg->_nodes, dg->_limits, elt_size, node, global_property_id, type);
}



/* tag: Tom Lord Thu Oct 28 08:11:08 2004 (graph.c)
 */
