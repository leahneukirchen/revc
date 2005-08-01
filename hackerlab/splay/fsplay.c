/* fsplay.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/splay/fsplay.h"




struct fsplay_tree_type
{
  alloc_limits limits;
  struct fsplay_vtable * vtable;

  struct fsplay_tree * free_list;
  t_uchar * free_pool;
  int n_free_pool;
};

static int const pool_chunk_size = (1 << 10);


/* __STDC__ prototypes for static functions */
static t_fsplay_tree fsplay_alloc_node (t_fsplay_tree_type type);
static t_fsplay_tree fsplay_make_splayed_node (t_fsplay_tree_type type,
                                               t_fsplay_tree being_rotated,
                                               t_fsplay_tree new_left,
                                               t_fsplay_tree new_right);
static int cmp_always_minus_1 (t_fsplay_tree_type type, void * position, t_fsplay_tree tree);
static int cmp_always_plus_1 (t_fsplay_tree_type type, void * position, t_fsplay_tree tree);


/************************************************************************
 *(h1 "t_fpslay_tree -- Functional Splay Trees"
 *     :include "hackerlab/splay/fsplay.h")
 * 
 * Functional splay trees are reference counted binary trees 
 * which _are_never_modified_.
 * 
 * Instead, operations that would normally modify a tree (such as splaying
 * or the insertion of a new element) work by building a new.  The new
 * tree will share as much information as possible with the old tree.
 * 
 * For example, let's suppose that we have a tree of the form:
 * 
 * 
 *        T
 *      /   \ 
 *    Tl     Tr
 * 
 * and want to insert a new element `N' just after `T'.
 * We'll build a new tree:
 * 
 * 
 *           N
 *         /   \
 *        T'    Tr
 *      /
 *    Tl
 * 
 * In which the subtrees `Tl' and `Tr' are shared with the old tree,
 * but elements {T'} and `N' are newly allocated.
 */
/*(menu)
 */

/*(include-documentation "fsplay.h")
 */



/*(c fsplay_make_type)
 * t_fsplay_tree_type fsplay_make_type (alloc_limits limits,
 *                                      struct fsplay_vtable * vtable);
 * 
 * Allocate a new type object describing a class of fsplay trees.
 * (See xref:"struct fsplay_vtable".)
 * 
 * The allocation pool `limits' will be usef for all future allocations
 * related to this tree type.
 */
t_fsplay_tree_type
fsplay_make_type (alloc_limits limits,
                  struct fsplay_vtable * vtable)
{
  t_fsplay_tree_type answer = 0;

  invariant (vtable->full_tree_node_size >= sizeof (struct fsplay_tree));

  answer = lim_malloc (limits, sizeof (*answer));
  if (!answer)
    return 0;

  mem_set0 ((t_uchar *)answer, sizeof (*answer));
  answer->limits = limits;
  answer->vtable = vtable;
  return answer;
}


/************************************************************************
 *(h1 "Memory Management for fsplay Trees")
 * 
 * 
 * 
 */


static t_fsplay_tree
fsplay_alloc_node (t_fsplay_tree_type type)
{
  t_fsplay_tree answer = 0;

 retry:

  if (type->free_list)
    {
      answer = type->free_list;
      type->free_list = answer->left;
    }
  else if (type->n_free_pool)
    {
      --type->n_free_pool;
      answer = (t_fsplay_tree)(type->free_pool + type->n_free_pool * type->vtable->full_tree_node_size);
    }
  else
    {
      type->free_pool = (t_uchar *)lim_malloc (type->limits, pool_chunk_size * type->vtable->full_tree_node_size);
      if (type->free_pool)
        {
          type->n_free_pool = pool_chunk_size;
          goto retry;
        }
    }

  if (answer)
    {
      mem_set0 ((t_uchar *)answer, type->vtable->full_tree_node_size);
      answer->refs = 1;
    }

  return answer;
}


/*(c fsplay_make)
 * t_fsplay_tree fsplay_make (t_fsplay_tree_type type,
 *                            void * data);
 * 
 * Create a new (single-node) splay tree from the indicated `data'.
 * Return 0 upon allocation failure.
 * 
 * Note that as a side effect, except in the case of allocation failure,
 * this function will call the `ref_data' method of the vtable associated
 * with `type'.
 * 
 * The newly allocated tree has a reference count of 1.
 */
t_fsplay_tree
fsplay_make (t_fsplay_tree_type type,
             void * data)
{
  t_fsplay_tree answer = 0;

  answer = fsplay_alloc_node (type);

  if (answer)
    {
      answer->data = data;
      type->vtable->ref_data (type, answer);
    }

  return answer;
}



/*(c fsplay_ref)
 * void fsplay_ref (t_fsplay_tree_type type,
 *                  t_fsplay_tree tree);
 * 
 * Increment the reference count of `tree'.
 */
void
fsplay_ref (t_fsplay_tree_type type,
            t_fsplay_tree tree)
{
  if (tree)
    ++tree->refs;
}


/*(c fsplay_unref)
 * void fsplay_unref (t_fsplay_tree_type type,
 *                    t_fsplay_tree tree);
 * 
 * Decrement the reference count of `tree'.  If it falls to 0, 
 * the tree is deallocated.
 */
void
fsplay_unref (t_fsplay_tree_type type,
              t_fsplay_tree tree)
{
 tail_call:
  if (tree)
    {
      --tree->refs;

      if (!tree->refs)
        {
          t_fsplay_tree left = tree->left;
          t_fsplay_tree right = tree->right;

          tree->left = type->free_list;
          type->free_list = tree;

          if (type->vtable->unref_data)
            {
              type->vtable->unref_data (type, tree);
            }

          if (!right && left)
            {
              tree = left;
              goto tail_call;
            }
          else if (!left && right)
            {
              tree = right;
              goto tail_call;
            }
          else if (left)    /* we know && right */
            {
              fsplay_unref (type, left);
              tree = right;
              goto tail_call;
            }
        }
    }
}


static t_fsplay_tree
fsplay_make_splayed_node (t_fsplay_tree_type type,
                          t_fsplay_tree being_rotated,
                          t_fsplay_tree new_left,
                          t_fsplay_tree new_right)
{
  t_fsplay_tree answer = 0;

  answer = fsplay_alloc_node (type);

  if (answer)
    {
      /* assume that data has a reference count for this new node.
       */
      answer->data = being_rotated->data;
      if (new_left)
        {
          fsplay_ref (type, new_left);
          answer->left = new_left;
        }
      if (new_right)
        {
          fsplay_ref (type, new_right);
          answer->right = new_right;
        }

      type->vtable->ref_data (type, answer);
    }

  return answer;
}


/************************************************************************
 *(h1 "Searching in Functional Splay Trees")
 * 
 * 
 */


/*(c fsplay_find)
 * t_fsplay_tree fsplay_find (t_fsplay_tree_type type,
 *                            t_fsplay_cmp_position_fn cmp,
 *                            t_fsplay_tree tree,
 *                            void * position);
 * 
 * Using the ordering function `cmp', search for data matching
 * `position' in `tree'.
 * 
 * If found, the matching subtree is returned and its reference count
 * is incremented. (Callers should plan to use `fsplay_unref'.)
 * 
 * If not found, 0 is returned.
 */
t_fsplay_tree
fsplay_find (t_fsplay_tree_type type,
             t_fsplay_cmp_position_fn cmp,
             void * position,
             t_fsplay_tree tree)
{
 tail_call:

  if (!tree)
    {
      return 0;
    }
  else
    {
      int cmp_order;

      cmp_order = cmp (type, position, tree);

      if (!cmp_order)
        {
          fsplay_ref (type, tree);
          return tree;
        }
      else if (cmp_order < 0)
        {
          tree = tree->left;
          goto tail_call;
        }
      else
        {
          tree = tree->right;
          goto tail_call;
        }
    }
}


/*(c fsplay_find_min)
 * t_fsplay_tree fsplay_find_min (t_fsplay_tree_type type,
 *                                t_fsplay_tree tree);
 * 
 * Return the leftmost leaf node of `tree', incrementing its 
 * reference count. (Callers should plan to use `fsplay_unref'.)
 */
t_fsplay_tree
fsplay_find_min (t_fsplay_tree_type type,
                 t_fsplay_tree tree)
{
  if (!tree)
    return 0;

  while (tree->left)
    tree = tree->left;

  fsplay_ref (type, tree);
  return tree;
}


/*(c fsplay_find_max)
 * t_fsplay_tree fsplay_find_max (t_fsplay_tree_type type,
 *                                t_fsplay_tree tree);
 * 
 * Return the rightmost leaf node of `tree', incrementing its 
 * reference count. (Callers should plan to use `fsplay_unref'.)
 */
t_fsplay_tree
fsplay_find_max (t_fsplay_tree_type type,
                 t_fsplay_tree tree)
{
  if (!tree)
    return 0;

  while (tree->right)
    tree = tree->right;

  fsplay_ref (type, tree);
  return tree;
}
    

/************************************************************************
 *(h1 "Top-down Splaying for Functional Splay Trees")
 * 
 * "Splaying" is a process by which a binary tree is converted into 
 * an equivalently ordered binary tree, but with a different root node.
 * 
 * On average (for a random sequence of splaying operations --
 * randomly choosing which node to make the new root) splaying is an
 * `O(log n)' operation where `n' is the number of nodes in the tree.
 * 
 * For access patterns exhibiting locality within the ordered set of
 * nodes, splaying tends to move frequently accessed nodes near to the
 * root of the tree (where they can be found very quickly).
 * 
 * In classical splay trees, splaying is a mutating operation: the tree
 * being splayed is modified in place.
 * 
 * With functional splay trees, splaying creates a _new_tree_.  The new
 * tree shares as much state as possible with the old tree, but the root
 * node and up to (on average) `O(log n)' additional nodes may be newly 
 * allocated.
 * 
 * Why use functional splay trees?  They are surely not for every
 * circumstance but they do have an attractive property: after
 * splaying a _functional_ tree, both the original tree and the new
 * tree still exist, relatively efficiently.
 * 
 * The splay operation is the basis of fundamental ordered set
 * operations such as insertion or deletion
 * 
 * Consider, therefore, using functional splay trees to implement an
 * editable string such as for the ``buffer'' data structure of a 
 * text editor:
 * 
 * Such a buffer is, in effect, an ordered set of ``buffer fragments'', 
 * each representing a portion of the buffer.
 * 
 * Basic editting operations insert and delete buffer fragments.
 * 
 * By using any kind of splay tree:
 * 
 * ~ random string insertions and deletions average `O(log n)' of the 
 *   size of the buffer rather than `O(n)' (as with gap buffers).
 * 
 * ~ localized string insertions and deletions average `O(1)' (as
 *   with gap buffers).
 * 
 * By using _functional_ splay trees:
 * 
 * ~ undo and redo are very simple to implement:  just keep the 
 *   pre- or post-edit trees around.
 * 
 * 
 * One difficulty of this approach is the need to avoid
 * fragmentation.   It is unacceptable, for example, that every
 * insertion of a single character adds a new tree node.  In combination
 * with editting, a buffer data structure must also do some form
 * of ``gathering'' -- combining adjacent buffer fragments into 
 * single, contiguous-in-memory buffer fragments.   But splay trees
 * again make this fairly trivial:  gathering the top few nodes in
 * a splay tree is an `O(1)' operation -- a buffer data structure can
 * do this in the background, incrementally, and opportunistically.
 * 
 * Another difficulty of this approach is that buffer text is not 
 * contiguous in memory.   For example, a regexp match must either
 * first gather the buffer into a single node (an `O(n)' operation)
 * or must be able to operate across the fragments of a scattered string.
 * Now you know why `libhackerlab''s Rx pattern matcher has support
 * for scattered strings.
 * 
 * Another difficulty of this approach is I/O, especially writing the
 * contents of a buffer.   No you know why I'm a big fan of scatter/gather
 * I/O in some versions of unix-like operating systems.
 */


/*(c fsplay_raise)
 * t_fsplay_tree fsplay_raise (t_fsplay_tree_type type,
 *                             t_fsplay_cmp_position_fn cmp,
 *                             void * position,
 *                             t_fsplay_tree tree);
 * 
 * Return a new tree which is a splay of `tree' that raises
 * the node of `tree' closest to `position' (according to `cmp')
 * to the root.
 * 
 * ``Closest'' means: an exactly matching node if one exists, 
 * otherwise the node to the immediate left if that exits,
 * otherwise the node to the immediate right.
 */
t_fsplay_tree
fsplay_raise (t_fsplay_tree_type type,
              t_fsplay_cmp_position_fn cmp,
              void * position,
              t_fsplay_tree tree)
{
  int cmp_order;

  if (!tree)
    return 0;

  /* The algorithm below always hold a reference count on 
   * the current root.
   */
  fsplay_ref (type, tree);


 iterate:

  cmp_order = cmp (type, position, tree);

  if (!cmp_order)
    {
      /* Give the root reference we hold to the caller.
       */
      return tree;
    }
  else if (cmp_order < 0)
    {
      /* The desired root node is in the left subtree, if the left subtree exists.
       */
      int cmp2_order;

      if (!tree->left)
        {
          /* Give the root reference we hold to the caller.
           */
          return tree;
        }

      cmp2_order = cmp (type, position, tree->left);

      if (!cmp2_order)
        {
          /* zig
           *
           *               T
           *            /     \
           *          Tl        _Tr
           *         /   \
           *      _Tll  _Tlr
           * 
           * Tl is our desired tree-root.
           * 
           * Nodes beginning with _ might be NULL.
           * 
           * We need to make:
           * 
           *           Tl'
           *          /   \
           *       _Tll      T'
           *               /   \
           *            _Tlr   _Tr
           * 
           * 
           * Note that the preorder traversals are:
           * 
           *    before: _Tll   Tl   _Tlr   T   _Tr
           *    after:  _Tll   Tl'  _Tlr   T'  _Tr
           */
          t_fsplay_tree T_prime = 0;
          t_fsplay_tree Tl_prime = 0;

        Tl_is_target_root:

          T_prime = fsplay_make_splayed_node (type, tree, tree->left->right, tree->right);
          if (!T_prime)
            {
            zig_alloc_error:
              fsplay_unref (type, T_prime);
              fsplay_unref (type, Tl_prime);
              fsplay_unref (type, tree);
              return 0;
            }

          Tl_prime = fsplay_make_splayed_node (type, tree->left, tree->left->left, T_prime);
          if (!Tl_prime)
            goto zig_alloc_error;
          fsplay_unref (type, T_prime); 

          fsplay_unref (type, tree);
          /* keep the reference on Tl_prime and give it to the caller.
           */

          return Tl_prime;
        }
      else if (cmp2_order < 0)
        {
          /* zig-zig
           *
           *               T
           *            /     \
           *          Tl       _Tr
           *         /   \
           *      _Tll  _Tlr
           *     /   \
           *  _Tlll  _Tllr
           *
           * _Tll is our desired tree-root.
           * 
           * Nodes beginning with _ might be NULL.
           * 
           * If _Tll is null, then this reduces to the "Tl_is_target_root" rule.
           * 
           * Otherwise, we need to build:
           * 
           *                Tll'
           *            /         \
           *         _Tlll         Tl'
           *                    /       \
           *                 _Tllr       T'
           *                           /   \
           *                         _Tlr    _Tr
           * 
           *
           * Preorder traversals:
           * 
           *    before:  _Tlll   _Tll   _Tllr   _Tl   _Tlr   T   _Tr
           *    after:   _Tlll   _Tll'  _Tllr   _Tl'  _Tlr   T'  _Tr
           * 
           * What if there is no exactly matching node?
           * 
           * We now that the key is < T and < Tl.
           * 
           * If _Tll doesn't exist, we raise Tl and we're done with it.
           * 
           * Note that we want to handle the "no exact match" case without
           * looping infinitely.  If there is a "just to the left" match,
           * we eventually want to raise that.   If there isn't a "just to the 
           * left" match, we want to raise the "just to the right" match.
           * 
           * If this part of the raise process were part of a thrashing loop,
           * then after this raise we must, later, raise Tll' a second time.
           * We know that the key is < Tl.  So if it is greater than Tllr.
           * And if it is less than Tll we'll raise Tlll or one of its immediate
           * children.
           * 
           * [Please insert here a slightly tricky inductive proof that no thrashing
           *  will occur and replicate that by analogy in the next 4 cases.  Until then
           *  let's take D Sleator's word for it (and hope that the algorithm here is a 
           *  faithful transcription thereof. :-)]
           */

          if (!tree->left->left)
            goto Tl_is_target_root;
          else
            {
              t_fsplay_tree T_prime = 0;
              t_fsplay_tree Tl_prime = 0;
              t_fsplay_tree Tll_prime = 0;

              T_prime = fsplay_make_splayed_node (type, tree, tree->left->right, tree->right);
              if (!T_prime)
                {
                zig_zig_alloc_error:
                  fsplay_unref (type, T_prime);
                  fsplay_unref (type, Tl_prime);
                  fsplay_unref (type, Tll_prime);
                  fsplay_unref (type, tree);
                  return 0;
                }

              Tl_prime = fsplay_make_splayed_node (type, tree->left, tree->left->left->right, T_prime);
              if (!Tl_prime)
                goto zig_zig_alloc_error;
              fsplay_unref (type, T_prime);

              Tll_prime = fsplay_make_splayed_node (type, tree->left->left, tree->left->left->left, Tl_prime);
              if (!Tll_prime)
                goto zig_zig_alloc_error;
              fsplay_unref (type, Tl_prime);
              
              fsplay_unref (type, tree);
              /* keep the reference on Tll_prime and interate.
               */

              tree = Tll_prime;
              goto iterate;
            }
        }
      else
        {
          /* zig-zag
           *
           *               T
           *            /     \
           *          Tl          _Tr
           *         /   \
           *      _Tll  _Tlr
           *           /   \
           *        _Tlrl  _Tlrr
           *
           * _Tlr is our desired tree-root.
           * 
           * Nodes beginning with _ might be NULL.
           * 
           * If _Tlr is null, then this reduces to the "Tl_is_target_root" rule.
           * 
           * Otherwise, we need to build:
           * 
           *               Tlr'
           *            /         \
           *          Tl'          T'
           *        /    \       /    \
           *     _Tll   _Tlrl  _Tlrr   _Tr
           * 
           *
           * Preorder traversals:
           * 
           *    before:  _Tll   Tl   _Tlrl   _Tlr   _Tlrr   T   _Tr
           *    after:   _Tll   Tl'  _Tlrl   _Tlr'  _Tlrr   T'  _Tr
           * 
           */

          if (!tree->left->right)
            goto Tl_is_target_root;
          else
            {
              t_fsplay_tree T_prime = 0;
              t_fsplay_tree Tl_prime = 0;
              t_fsplay_tree Tlr_prime = 0;

              T_prime = fsplay_make_splayed_node (type, tree, tree->left->right->right, tree->right);
              if (!T_prime)
                {
                zig_zag_alloc_error:
                  fsplay_unref (type, T_prime);
                  fsplay_unref (type, Tl_prime);
                  fsplay_unref (type, Tlr_prime);
                  fsplay_unref (type, tree);
                  return 0;
                }


              Tl_prime = fsplay_make_splayed_node (type, tree->left, tree->left->left, tree->left->right->left);
              if (!Tl_prime)
                goto zig_zag_alloc_error;

              Tlr_prime = fsplay_make_splayed_node (type, tree->left->right, Tl_prime, T_prime);
              if (!Tlr_prime)
                goto zig_zag_alloc_error;
              fsplay_unref (type, Tl_prime);
              fsplay_unref (type, T_prime);

              fsplay_unref (type, tree);
              /* keep the reference on Tlr_prime and iterate.
               */

              tree = Tlr_prime;
              goto iterate;
            }
        }
    }
  else
    {
      /* The desired root node is in the right subtree, if the right subtree exists.
       */
      int cmp2_order;

      if (!tree->right)
        {
          /* Give the root reference we hold to the caller.
           */
          return tree;
        }

      cmp2_order = cmp (type, position, tree->right);

      if (!cmp2_order)
        {
          /* zag
           *
           *               T
           *            /     \
           *          _Tl        Tr
           *                   /   \
           *                 _Trl  _Trr
           * 
           * Tr is our desired tree-root.
           * 
           * Nodes beginning with _ might be NULL.
           * 
           * We need to make:
           * 
           *           Tr'
           *          /    \
           *         T'     _Trr
           *       /  \   
           *    _Tl   _Trl
           * 
           * 
           * Note that the preorder traversals are:
           * 
           *    before: _Tl   T   _Trl   Tr   _Trr
           *    after:  _Tl   T'  _Trl   Tr'  _Trr
           */

          t_fsplay_tree T_prime = 0;
          t_fsplay_tree Tr_prime = 0;

        Tr_is_target_root:

          T_prime = fsplay_make_splayed_node (type, tree, tree->left, tree->right->left);
          if (!T_prime)
            {
            zag_alloc_error:
              fsplay_unref (type, T_prime);
              fsplay_unref (type, Tr_prime);
              fsplay_unref (type, tree);
              return 0;
            }

          Tr_prime = fsplay_make_splayed_node (type, tree->right, T_prime, tree->right->right);
          if (!Tr_prime)
            goto zag_alloc_error;
          fsplay_unref (type, T_prime); 

          fsplay_unref (type, tree);
          /* keep the reference on Tr_prime and give it to the caller.
           */

          return Tr_prime;
        }
      else if (cmp2_order < 0)
        {
          /* zag-zag
           *
           *               T
           *            /     \
           *         _Tl       Tr
           *                 /   \
           *               _Trl  _Trr
           *                    /   \
           *                 _Trrl  _Trrr
           *
           * _Trr is our desired tree-root.
           * 
           * Nodes beginning with _ might be NULL.
           * 
           * If _Trr is null, then this reduces to the "Tr_is_target_root" rule.
           * 
           * Otherwise, we need to build:
           * 
           *               Trr'
           *            /         \
           *           Tr'         _Trrr
           *         /    \
           *       T'     _Trrl
           *     /   \
           *  _Tl    _Trl
           * 
           *
           * Preorder traversals:
           * 
           *    before:  _Tl   T   _Trl   Tr   _Trrl   _Trr   _Trrr
           *    after:   _Tl   T'  _Trl   Tr'  _Trrl   _Trr'  _Trrr
           * 
           */

          if (!tree->right->right)
            goto Tr_is_target_root;
          else
            {
              t_fsplay_tree T_prime = 0;
              t_fsplay_tree Tr_prime = 0;
              t_fsplay_tree Trr_prime = 0;

              T_prime = fsplay_make_splayed_node (type, tree, tree->left, tree->right->left);
              if (!T_prime)
                {
                zag_zag_alloc_error:
                  fsplay_unref (type, T_prime);
                  fsplay_unref (type, Tr_prime);
                  fsplay_unref (type, Trr_prime);
                  fsplay_unref (type, tree);
                  return 0;
                }

              Tr_prime = fsplay_make_splayed_node (type, tree->right, T_prime, tree->right->right->left);
              if (!Tr_prime)
                goto zag_zag_alloc_error;
              fsplay_unref (type, T_prime);

              Trr_prime = fsplay_make_splayed_node (type, tree->right->right, Tr_prime, tree->right->right->right);
              if (!Trr_prime)
                goto zag_zag_alloc_error;
              fsplay_unref (type, Tr_prime);
              
              fsplay_unref (type, tree);
              /* keep the reference on Trr_prime and iterate.
               */

              tree = Trr_prime;
              goto iterate;
            }
        }
      else
        {
          /* zag-zig
           *
           *               T
           *            /     \
           *          _Tl      Tr
           *                  /   \
           *               _Trl  _Trr
           *              /   \
           *           _Trlr  _Trlr
           *
           * _Trl is our desired tree-root.
           * 
           * Nodes beginning with _ might be NULL.
           * 
           * If _Trl is null, then this reduces to the "Tr_is_target_root" rule.
           * 
           * Otherwise, we need to build:
           * 
           *               Trl'
           *            /         \
           *           T'          Tr'
           *        /    \       /    \
           *      _Tl   _Trlr  _Trlr   _Trr
           * 
           *
           * Preorder traversals:
           * 
           *    before:  _Tl   T   _Trlr   _Trl   _Trlr   Tr   _Trr
           *    after:   _Tl   T'  _Trlr   _Trl'  _Trlr   Tr'  _Trr
           * 
           */

          if (!tree->right->left)
            goto Tr_is_target_root;
          else
            {
              t_fsplay_tree T_prime = 0;
              t_fsplay_tree Tr_prime = 0;
              t_fsplay_tree Trl_prime = 0;

              T_prime = fsplay_make_splayed_node (type, tree, tree->left, tree->right->left->right);
              if (!T_prime)
                {
                zag_zig_alloc_error:
                  fsplay_unref (type, T_prime);
                  fsplay_unref (type, Tr_prime);
                  fsplay_unref (type, Trl_prime);
                  fsplay_unref (type, tree);
                  return 0;
                }
              Tr_prime = fsplay_make_splayed_node (type, tree->right, tree->right->left->right, tree->right->right);
              if (!Tr_prime)
                goto zag_zig_alloc_error;

              Trl_prime = fsplay_make_splayed_node (type, tree->right->left, T_prime, Tr_prime);
              if (!Trl_prime)
                goto zag_zig_alloc_error;
              fsplay_unref (type, T_prime);
              fsplay_unref (type, Tr_prime);

              fsplay_unref (type, tree);
              /* keep the reference on Trl_prime and iterate.
               */

              tree = Trl_prime;
              goto iterate;
            }
        }
    }
}


t_fsplay_tree
fsplay_raise_min (t_fsplay_tree_type type,
                  t_fsplay_tree tree)
{
  return fsplay_raise (type, cmp_always_minus_1, 0, tree);
}


t_fsplay_tree
fsplay_raise_max (t_fsplay_tree_type type,
                  t_fsplay_tree tree)
{
  return fsplay_raise (type, cmp_always_plus_1, 0, tree);
}


static int
cmp_always_minus_1 (t_fsplay_tree_type type, void * position, t_fsplay_tree tree)
{
  return -1;
}


static int
cmp_always_plus_1 (t_fsplay_tree_type type, void * position, t_fsplay_tree tree)
{
  return 1;
}




/*(c fsplay_insert_after)
 * t_fsplay_tree fsplay_insert_after (t_fsplay_tree_type type,
 *                                    t_fsplay_tree tree,
 *                                    t_fsplay_tree new_tree);
 * 
 * Return a new tree which is `new_tree' inserted just after 
 * the root node of `tree'.
 */
t_fsplay_tree
fsplay_insert_after (t_fsplay_tree_type type,
                     t_fsplay_tree tree,
                     t_fsplay_tree new_tree)
{
  if (!tree)
    {
      fsplay_ref (type, new_tree);
      return new_tree;
    }
  else if (!new_tree)
    {
      fsplay_ref (type, tree);
      return tree;
    }
  else
    {
      /* insert-after
       * 
       *  Have:
       * 
       *        T
       *      /   \ 
       *    _Tl   _Tr
       * 
       * and want to insert N after T giving:
       * 
       * 
       *        N'
       *      /   \
       *     T'    _Tr
       *   /
       * _Tl
       * 
       * 
       * Preorder traversal:
       * 
       *   Before:      _Tl   T        _Tr
       *   After:       _Tl   T'   N'  _Tr
       */

      t_fsplay_tree T_prime = 0;
      t_fsplay_tree N_prime = 0;

      T_prime = fsplay_make_splayed_node (type, tree, tree->left, 0);
      if (!T_prime)
        {
        alloc_error:
          fsplay_unref (type, T_prime);
          fsplay_unref (type, N_prime);
          return 0;
        }

      N_prime = fsplay_make_splayed_node (type, new_tree, T_prime, tree->right);
      if (!N_prime)
        goto alloc_error;
      fsplay_unref (type, T_prime);

      return N_prime;
    }
}


/*(c fsplay_insert_before)
 * t_fsplay_tree fsplay_insert_before (t_fsplay_tree_type type,
 *                                     t_fsplay_tree tree,
 *                                     t_fsplay_tree new_tree);
 * 
 * Return a new tree which is `new_tree' inserted just befpre
 * the root node of `tree'.
 */
t_fsplay_tree
fsplay_insert_before (t_fsplay_tree_type type,
                      t_fsplay_tree tree,
                      t_fsplay_tree new_tree)
{
  if (!tree)
    {
      fsplay_ref (type, new_tree);
      return new_tree;
    }
  else if (!new_tree)
    {
      fsplay_ref (type, tree);
      return tree;
    }
  else
    {
      /* insert-before
       * 
       *  Have:
       * 
       *        T
       *      /   \ 
       *    _Tl   _Tr
       * 
       * and want to insert N after T giving:
       * 
       * 
       *        N'
       *      /   \
       *    _Tl    T'
       *            \   
       *             _Tr
       * 
       * 
       * Preorder traversal:
       * 
       *   Before:      _Tl       T   _Tr
       *   After:       _Tl   N'  T'  _Tr
       */

      t_fsplay_tree T_prime = 0;
      t_fsplay_tree N_prime = 0;

      T_prime = fsplay_make_splayed_node (type, tree, 0, tree->right);
      if (!T_prime)
        {
        alloc_error:
          fsplay_unref (type, T_prime);
          fsplay_unref (type, N_prime);
          return 0;
        }

      N_prime = fsplay_make_splayed_node (type, new_tree, tree->left, T_prime);
      if (!N_prime)
        goto alloc_error;
      fsplay_unref (type, T_prime);

      return N_prime;
    }
}



/*(c fsplay_delete)
 * t_fsplay_tree fsplay_delete (t_fsplay_tree_type type,
 *                              t_fsplay_tree tree);
 * 
 * Return a new tree which is `tree' with its root node removed.
 */
t_fsplay_tree
fsplay_delete (t_fsplay_tree_type type,
               t_fsplay_tree tree)
{
  if (!tree)
    return 0;
  else
    {
      /* delete
       * 
       * Have:
       * 
       *        T
       *     /     \
       *   _Tl     _Tr
       *
       * and want to remove T.
       * 
       * Removing T is the same as inserting _Tr after _Tl.
       */

      return fsplay_insert_after (type, tree->left, tree->right);
    }
}




/* tag: Tom Lord Fri Jan  2 09:31:58 2004 (fsplay.c)
 */
