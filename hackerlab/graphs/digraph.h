/* digraph.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBGRAPH__DIGRAPH_H
#define INCLUDE__LIBGRAPH__DIGRAPH_H


#include "hackerlab/obtables/obtable.h"
#include "hackerlab/graphs/node-set.h"



typedef struct digraph t_digraph;



struct digraph
{
  alloc_limits _limits;
  t_obnode_set _nodes;
};


/* automatically generated __STDC__ prototypes */
extern ssize_t open_digraph (alloc_limits limits,
                             ssize_t representation_type);
extern int close_digraph (ssize_t n);
extern alloc_limits digraph_limits (ssize_t d);
extern ssize_t digraph_n_nodes (ssize_t d);
extern ssize_t digraph_alloc_node (ssize_t d);
extern int digraph_free_node (ssize_t d, ssize_t n);
extern ssize_t digraph_n_subnodes (ssize_t d, ssize_t n);
extern ssize_t digraph_n_parent_nodes (ssize_t d, ssize_t n);
extern ssize_t digraph_subnode_data_k (ssize_t d, ssize_t n, ssize_t k);
extern ssize_t digraph_parent_data_k (ssize_t * out_slot,
                                      ssize_t d,
                                      ssize_t n,
                                      ssize_t k);
extern int digraph_insert_edge (ssize_t d,
                                ssize_t source,
                                ssize_t where,
                                ssize_t dest);
extern int digraph_delete_edge (ssize_t d,
                                ssize_t source,
                                ssize_t where);
extern void * digraph_property_soft_ref (ssize_t d,
                                         ssize_t elt_size,
                                         ssize_t node,
                                         ssize_t global_property_id);
extern void * digraph_property_ref (ssize_t d,
                                    ssize_t elt_size,
                                    ssize_t node,
                                    ssize_t global_property_id,
                                    t_property_type * type);
#endif  /* INCLUDE__LIBGRAPH__DIGRAPH_H */


/* tag: Tom Lord Thu Oct 28 08:06:19 2004 (graph.h)
 */
