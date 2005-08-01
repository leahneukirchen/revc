/* obnode-set.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__OBNODE_SET_H
#define INCLUDE__LIBAWIKI__OBNODE_SET_H


#include "hackerlab/obtables/obtable.h"
#include "hackerlab/oblists/oblist.h"
#include "hackerlab/properties/property.h"



typedef struct obnode_set t_obnode_set;
typedef struct obnode t_obnode;
typedef struct obnode_out_edge t_obnode_out_edge;
typedef struct obnode_in_edge t_obnode_in_edge;



struct obnode_set
{
  t_obtable _obnodes;
  ssize_t _representation_type;
  t_property_table _property_table;
};

struct obnode
{
  struct obtable_elt _obtable_elt;
  t_oblist _out_edges;
  t_oblist _in_edges;
  ssize_t _handle_set;
};

struct obnode_out_edge
{
  ssize_t _dest_node;
};

struct obnode_in_edge
{
  ssize_t _source_node;
  ssize_t _out_edge_slot;
};



/* automatically generated __STDC__ prototypes */
extern int init_obnode_set (t_obnode_set * os,
                            alloc_limits limits,
                            ssize_t representation_type);
extern int uninit_obnode_set (t_obnode_set * os,
                              alloc_limits limits);
extern ssize_t obnode_set_n_nodes (t_obnode_set * os,
                                   alloc_limits limits);
extern ssize_t obnode_set_alloc_node (t_obnode_set * os,
                                      alloc_limits limits);
extern int obnode_set_free_node (t_obnode_set * os,
                                 alloc_limits limits,
                                 ssize_t id);
extern int obnode_set__init_node (t_obnode * node,
                                  t_obnode_set * os,
                                  alloc_limits limits,
                                  ssize_t id);
extern ssize_t obnode_n_subnodes (t_obnode_set * os,
                                  alloc_limits limits,
                                  ssize_t n);
extern t_obnode_out_edge * obnode__subnode_k (t_obnode_set * os,
                                              alloc_limits limits,
                                              ssize_t n,
                                              ssize_t k);
extern ssize_t obnode_subnode_data_k (t_obnode_set * os,
                                      alloc_limits limits,
                                      ssize_t n,
                                      ssize_t k);
extern ssize_t obnode_n_parent_nodes (t_obnode_set * os,
                                      alloc_limits limits,
                                      ssize_t n);
extern t_obnode_in_edge * obnode__parent_k (t_obnode_set * os,
                                            alloc_limits limits,
                                            ssize_t n,
                                            ssize_t k);
extern ssize_t obnode_parent_data_k (ssize_t * out_edge_slot,
                                     t_obnode_set * os,
                                     alloc_limits limits,
                                     ssize_t n,
                                     ssize_t k);
extern int obnode_insert_edge (t_obnode_set * os,
                               alloc_limits limits,
                               ssize_t source,
                               ssize_t insert_order,
                               ssize_t dest);
extern int obnode_delete_edge (t_obnode_set * os,
                               alloc_limits limits,
                               ssize_t source,
                               ssize_t insert_order);
extern void * obnode_property_soft_ref (t_obnode_set * os,
                                        alloc_limits limits,
                                        ssize_t elt_size,
                                        ssize_t node,
                                        ssize_t global_property_id);
extern void * obnode_property_ref (t_obnode_set * os,
                                   alloc_limits limits,
                                   ssize_t elt_size,
                                   ssize_t node,
                                   ssize_t global_property_id,
                                   t_property_type * type);
extern ssize_t obnode_handle_set (t_obnode_set * os,
                                  alloc_limits limits,
                                  ssize_t elt_size,
                                  ssize_t n);
#endif  /* INCLUDE__LIBAWIKI__OBNODE_SET_H */


/* tag: Tom Lord Tue Oct 26 16:22:59 2004 (obnode-set.h)
 */
