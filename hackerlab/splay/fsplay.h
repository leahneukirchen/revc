/* fsplay.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__EDIT__FSPLAY_H
#define INCLUDE__EDIT__FSPLAY_H


#include "hackerlab/mem/alloc-limits.h"


/************************************************************************
 *(h2 "Functional Splay Tree Types")
 * 
 * 
 * 
 */


/*(c t_fsplay_tree :category type)
 * typedef struct fsplay_tree
 * {
 *   int refs;
 *   struct fsplay_tree * left;
 *   struct fsplay_tree * right;
 *   void * data;
 * } * t_fsplay_tree;
 * 
 * The type of a node in a functional splay tree.
 * Programs must not modify any but the `data' field.
 * 
 * Program may modify the `data' field but only by replacing
 * it with an equivalent value.
 * 
 * \Fields:/
 * 
 * `refs': the reference count of this node.
 * 
 * `left' and `right': The left and right subtrees (possibly 0).
 * 
 * `data': User data attached to this tree node.  Note that 
 * more than one node may share the same data, consequently, 
 * provisions are made to reference count the data separately from
 * the node (see below).
 */
struct fsplay_tree
{
  int refs;
  struct fsplay_tree * left;
  struct fsplay_tree * right;
  void * data;
};
typedef struct fsplay_tree * t_fsplay_tree;


/*(c t_fsplay_tree_type :category type)
 * typedef <unspecified> t_fsplay_tree_type;
 * 
 * `t_fsplay_tree_type' are ``type objects'' encapsulating a 
 * `struct fsplay_vtable *' and describing the type of a class
 * of `t_fsplay_tree' values.
 * 
 * The function `fsplay_make_type' is used to allocate new
 * `t_fsplay_tree_type' values.
 */
struct fsplay_tree_type;
typedef struct fsplay_tree_type * t_fsplay_tree_type;


/*(c #s"struct fsplay_vtable" :category type)
 * struct fsplay_vtable
 * {
 *   size_t full_tree_node_size;
 *   void (*ref_data) (t_fsplay_tree_type type, t_fsplay_tree tree);
 *   void (*unref_data) (t_fsplay_tree_type type, t_fsplay_tree tree);
 * };
 * 
 * A `struct fsplay_vtable' specifies parameters which distinguish 
 * a particular ``type'' of functional splay tree.    
 * 
 * In order to use functional splay trees, programs must provide an instance
 * of the vtable structure.
 * 
 * \Fields:/
 * 
 * `full_tree_node_size': The number of bytes to allocate for each node
 * in the data structure.   I.e., the number of bytes to allocate when 
 * creating a new `struct fsplay_tree'.   Note that this value _must_ be 
 * greater than `sizeof (struct fsplay_tree)'.   The purpose of this field
 * is to allow programs to embed additional information in tree nodes by
 * adding storage for additional fields beyond those of `struct fsplay_tree'.
 * 
 * `ref_data' and `unref_data': These are called when a new node has been
 * constructed.   The new node with all fields initialized is passed to 
 * these routines.  The `data' field of the new node has either been copied
 * from an older node or was passed to the function `fsplay_make'.  If `data' 
 * requires memory management, `ref' should either increment its reference 
 * count (_not_ the `refs' field of the node) or replace it with a freshly allocated
 * copy of the same data;  `unref' should either decrement the reference count of
 * `data' or free it if it was freshly allocated by `ref'.
 */
struct fsplay_vtable
{
  size_t full_tree_node_size;
  void (*ref_data) (t_fsplay_tree_type type, t_fsplay_tree tree);
  void (*unref_data) (t_fsplay_tree_type type, t_fsplay_tree tree);
};




/*(c t_fsplay_cmp_position_fn :category type)
 * typedef int (*t_fsplay_cmp_position_fn) (t_fsplay_tree_type type,
 *                                          void * position,
 *                                          t_fsplay_tree tree)
 * 
 * The function type of functions used to compare tree nodes to compute
 * their ordering relationship to an arbitrary datum.
 * 
 * The `data' field of `tree' should be compared to the datum indicated 
 * by `position'.  (`position' is ultimately supplied by the same caller who
 * provides the `t_fsplay_cmp_position_fn').  
 * 
 * If `position' is _less than_ `tree', a value less than 0 should be returned.
 * 
 * If `position' is _greater than_ `tree', a value greater than 0 should 
 * be returned.
 * 
 * If `position' is _equal to_ `tree', 0 should be returned.
 */
typedef int (*t_fsplay_cmp_position_fn) (t_fsplay_tree_type type,
                                         void * position,
                                         t_fsplay_tree tree);



/* automatically generated __STDC__ prototypes */
extern t_fsplay_tree_type fsplay_make_type (alloc_limits limits,
                                            struct fsplay_vtable * vtable);
extern t_fsplay_tree fsplay_make (t_fsplay_tree_type type,
                                  void * data);
extern void fsplay_ref (t_fsplay_tree_type type,
                        t_fsplay_tree tree);
extern void fsplay_unref (t_fsplay_tree_type type,
                          t_fsplay_tree tree);
extern t_fsplay_tree fsplay_find (t_fsplay_tree_type type,
                                  t_fsplay_cmp_position_fn cmp,
                                  void * position,
                                  t_fsplay_tree tree);
extern t_fsplay_tree fsplay_find_min (t_fsplay_tree_type type,
                                      t_fsplay_tree tree);
extern t_fsplay_tree fsplay_find_max (t_fsplay_tree_type type,
                                      t_fsplay_tree tree);
extern t_fsplay_tree fsplay_raise (t_fsplay_tree_type type,
                                   t_fsplay_cmp_position_fn cmp,
                                   void * position,
                                   t_fsplay_tree tree);
extern t_fsplay_tree fsplay_raise_min (t_fsplay_tree_type type,
                                       t_fsplay_tree tree);
extern t_fsplay_tree fsplay_raise_max (t_fsplay_tree_type type,
                                       t_fsplay_tree tree);
extern t_fsplay_tree fsplay_insert_after (t_fsplay_tree_type type,
                                          t_fsplay_tree tree,
                                          t_fsplay_tree new_tree);
extern t_fsplay_tree fsplay_insert_before (t_fsplay_tree_type type,
                                           t_fsplay_tree tree,
                                           t_fsplay_tree new_tree);
extern t_fsplay_tree fsplay_delete (t_fsplay_tree_type type,
                                    t_fsplay_tree tree);
#endif  /* INCLUDE__EDIT__FSPLAY_H */


/* tag: Tom Lord Fri Jan  2 09:31:55 2004 (fsplay.h)
 */
