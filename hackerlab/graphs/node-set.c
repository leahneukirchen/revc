/* node-set.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/handles/handle.h"
#include "hackerlab/graphs/node-set.h"


/* __STDC__ prototypes for static functions */
static t_obnode * obnode__n (t_obnode_set * os,
                             alloc_limits limits,
                             ssize_t n);
static int obnode__insert_edge (t_obnode_set * os,
                                alloc_limits limits,
                                ssize_t source_node,
                                ssize_t out_edge_slot,
                                ssize_t dest_node);
static int obnode__delete_edge (t_obnode_set * os,
                                alloc_limits limits,
                                ssize_t source_node,
                                ssize_t out_edge_slot);
static ssize_t obnode__add_in_edge (t_obnode_set * os,
                                    alloc_limits limits,
                                    ssize_t source_node,
                                    ssize_t out_edge_slot,
                                    ssize_t dest_node);
static int obnode__remove_in_edge (t_obnode_set * os,
                                   alloc_limits limits,
                                   ssize_t source_node,
                                   ssize_t out_edge_slot,
                                   ssize_t dest_node);
static int in_edge_cmp (void * va, void * vb, void * ign);



int
init_obnode_set (t_obnode_set * os,
                 alloc_limits limits,
                 ssize_t representation_type)
{
  if (!os)
    return -1;

  mem_set0 ((t_uchar *)os, sizeof (*os));

  if (0 > init_obtable (&os->_obnodes, limits, sizeof (t_obnode), 0, 0))
    return -1;

  if (0 > init_property_table (&os->_property_table, limits))
    return -1;

  if (representation_type > 0)
    {
      os->_representation_type = representation_type;
    }
  else
    {
      os->_representation_type = property_representation_id ((void *)init_obnode_set);
      if (os->_representation_type <= 0)
        return -1;
    }

  return 0;
}



int
uninit_obnode_set (t_obnode_set * os,
                   alloc_limits limits)
{
  ssize_t bound;
  ssize_t x;

  if (!os)
    return 0;

  bound = obtable_room (&os->_obnodes, limits, sizeof (t_obnode), 0, 0);
  for (x = 0; x < bound; ++x)
    {
      t_obnode * node;

      node = (t_obnode *)obtable_ref (&os->_obnodes, limits, sizeof (t_obnode), 0, 0, x);
      if (   node
          && (0 <= node->_handle_set)
          && (0 > free_handle_set (node->_handle_set)))
        return -1;
      node->_handle_set = -1;

      uninit_oblist (&node->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0);
      uninit_oblist (&node->_in_edges, limits, sizeof (t_obnode_in_edge), 0, 0);
    }
  uninit_obtable (&os->_obnodes, limits, sizeof (t_obnode), 0, 0);
  if (0 > uninit_property_table (&os->_property_table, limits))
    return -1;
  mem_set0 ((t_uchar *)os, sizeof (*os));
  return 0;
}



ssize_t
obnode_set_n_nodes (t_obnode_set * os,
                    alloc_limits limits)
{
  if (!os)
    return 0;

  return obtable_n_elts (&os->_obnodes, limits, sizeof (t_obnode), 0, 0);
}


ssize_t
obnode_set_alloc_node (t_obnode_set * os,
                       alloc_limits limits)
{
  ssize_t answer;

  if (!os)
    return 0;

  answer = obtable_alloc (&os->_obnodes, limits, sizeof (t_obnode), 0, 0);
  if (answer >= 0)
    {
      t_obnode * node;

      node = (t_obnode *)obtable_ref (&os->_obnodes, limits, sizeof (t_obnode), 0, 0, answer);
      if (!node)
        return -1;

      if (0 > obnode_set__init_node (node, os, limits, answer))
        {
          obtable_free (&os->_obnodes, limits, sizeof (t_obnode), 0, 0, answer);
          return -1;
        }
    }

  return answer;
}

int
obnode_set_free_node (t_obnode_set * os,
                      alloc_limits limits,
                      ssize_t id)
{
  t_obnode * node;

  node = (t_obnode *)obtable_ref (&os->_obnodes, limits, sizeof (t_obnode), 0, 0, id);

  if (!node)
    return -1;

  if (   (node->_handle_set >= 0)
      && (0 > free_handle_set (node->_handle_set)))
      return -1;

  node->_handle_set = -1;

  while (0 < obnode_n_subnodes (os, limits, id))
    {
      if (0 > obnode_delete_edge  (os, limits, id, 0))
        return -1;
    }

  while (0 < obnode_n_parent_nodes (os, limits, id))
    {
      ssize_t source_id;
      ssize_t edge_slot;

      source_id = obnode_parent_data_k (&edge_slot, os, limits, id, 0);
      if (source_id < 0)
        return -1;

      if (0 > obnode_delete_edge (os, limits, source_id, edge_slot))
        return -1;
    }

  obtable_free (&os->_obnodes, limits, sizeof (t_obnode), 0, 0, id);
  return 0;
}



int
obnode_set__init_node (t_obnode * node,
                       t_obnode_set * os,
                       alloc_limits limits,
                       ssize_t id)
{
  if (!node)
    return -1;

  node->_handle_set = -1;
  if (0 < init_oblist (&node->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0))
    return -1;
  if (0 < init_oblist (&node->_in_edges, limits, sizeof (t_obnode_in_edge), 0, 0))
    return -1;

  return 0;
}



static t_obnode *
obnode__n (t_obnode_set * os,
           alloc_limits limits,
           ssize_t n)
{
  if (!os)
    return 0;
  else
    return (t_obnode *)obtable_ref (&os->_obnodes, limits, sizeof (t_obnode), 0, 0, n);
}




ssize_t
obnode_n_subnodes (t_obnode_set * os,
                   alloc_limits limits,
                   ssize_t n)
{
  t_obnode * node;

  node = obnode__n (os, limits, n);

  if (!node)
    return -1;
  else
    return oblist_size (&node->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0);
}


t_obnode_out_edge *
obnode__subnode_k (t_obnode_set * os,
                    alloc_limits limits,
                    ssize_t n,
                    ssize_t k)
{
  t_obnode * node;

  node = obnode__n (os, limits, n);

  if (!node)
    return 0;
  else
    return (t_obnode_out_edge *)oblist_burst (0, &node->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0, k, 1);
}


ssize_t
obnode_subnode_data_k (t_obnode_set * os,
                       alloc_limits limits,
                       ssize_t n,
                       ssize_t k)
{
  t_obnode * node;

  node = obnode__n (os, limits, n);

  if (!node)
    return -1;
  else
    {
      t_obnode_out_edge * edge;

      edge = (t_obnode_out_edge *)oblist_burst (0, &node->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0, k, 1);

      if (!edge)
        return -1;
      else
        return edge->_dest_node;
    }
}



ssize_t
obnode_n_parent_nodes (t_obnode_set * os,
                   alloc_limits limits,
                   ssize_t n)
{
  t_obnode * node;

  node = obnode__n (os, limits, n);

  if (!node)
    return -1;
  else
    return oblist_size (&node->_in_edges, limits, sizeof (t_obnode_in_edge), 0, 0);
}


t_obnode_in_edge *
obnode__parent_k (t_obnode_set * os,
                  alloc_limits limits,
                  ssize_t n,
                  ssize_t k)
{
  t_obnode * node;

  node = obnode__n (os, limits, n);

  if (!node)
    return 0;
  else
    return (t_obnode_in_edge *)oblist_burst (0, &node->_in_edges, limits, sizeof (t_obnode_in_edge), 0, 0, k, 1);
}


ssize_t
obnode_parent_data_k (ssize_t * out_edge_slot,
                      t_obnode_set * os,
                      alloc_limits limits,
                      ssize_t n,
                      ssize_t k)
{
  t_obnode * node;

  node = obnode__n (os, limits, n);

  if (!node)
    {
      if (out_edge_slot)
        *out_edge_slot = -1;
      return -1;
    }
  else
    {
      t_obnode_in_edge * edge;

      edge = (t_obnode_in_edge *)oblist_burst (0, &node->_in_edges, limits, sizeof (t_obnode_in_edge), 0, 0, k, 1);

      if (!edge)
        {
          if (out_edge_slot)
            *out_edge_slot = -1;
          return -1;
        }
      else
        {
          if (out_edge_slot)
            *out_edge_slot = edge->_out_edge_slot;
          return edge->_source_node;
        }
    }
}



static int
obnode__insert_edge (t_obnode_set * os,
                     alloc_limits limits,
                     ssize_t source_node,
                     ssize_t out_edge_slot,
                     ssize_t dest_node)
{
  t_obnode * snode;
  ssize_t n_out_edges;
  ssize_t x;
  t_obnode_out_edge * e;

  snode = obnode__n (os, limits, source_node);
  if (!snode)
    return -1;

  n_out_edges = oblist_size (&snode->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0);

  for (x = out_edge_slot; x < n_out_edges; ++x)
    {
      ssize_t subnode;

      subnode = obnode_subnode_data_k (os, limits, source_node, x);
      if (0 > obnode__remove_in_edge (os, limits, source_node, x, subnode))
        return -1;
      if (0 > obnode__add_in_edge (os, limits, source_node, x + 1, subnode))
        return -1;
    }

  if (0 > oblist_insert_n (&snode->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0, out_edge_slot, 1, 0))
    return -1;

  e = (t_obnode_out_edge *)oblist_burst (0, &snode->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0, out_edge_slot, 1);
  if (!e)
    return -1;

  e->_dest_node = dest_node;

  if (0 > obnode__add_in_edge (os, limits, source_node, out_edge_slot, dest_node))
    return -1;

  return 0;
}


static int
obnode__delete_edge (t_obnode_set * os,
                     alloc_limits limits,
                     ssize_t source_node,
                     ssize_t out_edge_slot)
{
  ssize_t dest_node;
  t_obnode * snode;
  ssize_t n_out_edges;
  ssize_t x;

  dest_node = obnode_subnode_data_k (os, limits, source_node, out_edge_slot);
  if (dest_node < 0)
    return -1;

  if (0 > obnode__remove_in_edge (os, limits, source_node, out_edge_slot, dest_node))
    return -1;

  snode = obnode__n (os, limits, source_node);
  if (!snode)
    return -1;

  n_out_edges = oblist_size (&snode->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0);

  for (x = out_edge_slot + 1; x < n_out_edges; ++x)
    {
      ssize_t subnode;

      subnode = obnode_subnode_data_k (os, limits, source_node, x);
      if (0 > obnode__remove_in_edge (os, limits, source_node, x, subnode))
        return -1;
      if (0 > obnode__add_in_edge (os, limits, source_node, x - 1, subnode))
        return -1;
    }

  if (0 > oblist_delete_n (&snode->_out_edges, limits, sizeof (t_obnode_out_edge), 0, 0, out_edge_slot, 1))
    return -1;

  return 0;
}


static ssize_t
obnode__add_in_edge (t_obnode_set * os,
                     alloc_limits limits,
                     ssize_t source_node,
                     ssize_t out_edge_slot,
                     ssize_t dest_node)
{
  t_obnode * dnode;
  t_obnode_in_edge in_key;

  dnode = obnode__n (os, limits, dest_node);

  if (!dnode)
    return -1;

  in_key._source_node = source_node;
  in_key._out_edge_slot = out_edge_slot;

  return oblist_sorted_insert (&dnode->_in_edges, limits, sizeof (t_obnode_in_edge), 0, 0, (void *)&in_key, in_edge_cmp, 0, 1, 0);
}


static int
obnode__remove_in_edge (t_obnode_set * os,
                        alloc_limits limits,
                        ssize_t source_node,
                        ssize_t out_edge_slot,
                        ssize_t dest_node)
{
  t_obnode * dnode;
  t_obnode_in_edge in_key;
  ssize_t in_edge;

  dnode = obnode__n (os, limits, dest_node);

  if (!dnode)
    return -1;

  in_key._source_node = source_node;
  in_key._out_edge_slot = out_edge_slot;

  in_edge = oblist_find (0, &dnode->_in_edges, limits, sizeof (t_obnode_in_edge), 0, 0, (void *)&in_key, in_edge_cmp, 0);

  if (!in_edge)
    return -1;

  if (0 > oblist_delete_n (&dnode->_in_edges, limits, sizeof (t_obnode_in_edge), 0, 0, in_edge, 1))
    return -1;

  return 0;
}



static int
in_edge_cmp (void * va, void * vb, void * ign)
{
  t_obnode_in_edge * a;
  t_obnode_in_edge * b;

  a = (t_obnode_in_edge *)va;
  b = (t_obnode_in_edge *)vb;

  if (a == b)
    return 0;

  if (!a)
    return -1;
  if (!b)
    return 1;

  if (a->_source_node < b->_source_node)
    return -1;
  if (a->_source_node > b->_source_node)
    return 1;
  if (a->_out_edge_slot < b->_out_edge_slot)
    return -1;
  if (a->_out_edge_slot > b->_out_edge_slot)
    return 1;

  return 0;
}





int
obnode_insert_edge (t_obnode_set * os,
                    alloc_limits limits,
                    ssize_t source,
                    ssize_t insert_order,
                    ssize_t dest)
{
  return obnode__insert_edge (os, limits, source, insert_order, dest);
}


int
obnode_delete_edge (t_obnode_set * os,
                    alloc_limits limits,
                    ssize_t source,
                    ssize_t insert_order)
{
  return obnode__delete_edge (os, limits, source, insert_order);
}



void *
obnode_property_soft_ref (t_obnode_set * os,
                          alloc_limits limits,
                          ssize_t elt_size,
                          ssize_t node,
                          ssize_t global_property_id)
{
  return property_soft_ref (&os->_property_table, limits, elt_size, os->_representation_type, global_property_id, node);
}


void *
obnode_property_ref (t_obnode_set * os,
                     alloc_limits limits,
                     ssize_t elt_size,
                     ssize_t node,
                     ssize_t global_property_id,
                     t_property_type * type)
{
  return property_ref (&os->_property_table, limits, elt_size, os->_representation_type, global_property_id, node, type);
}





ssize_t
obnode_handle_set (t_obnode_set * os,
                   alloc_limits limits,
                   ssize_t elt_size,
                   ssize_t n)
{
  t_obnode * node;

  node = obnode__n (os, limits, n);
  if (!node)
    return -1;

  if (node->_handle_set < 0)
    {
      node->_handle_set = alloc_handle ();
    }

  return node->_handle_set;
}



/* tag: Tom Lord Tue Oct 26 16:34:50 2004 (node-set.c)
 */
