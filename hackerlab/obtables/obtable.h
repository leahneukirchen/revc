/* obtable.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__OBTABLE_H
#define INCLUDE__LIBAWIKI__OBTABLE_H


#include "hackerlab/mem/alloc-limits.h"


/* General idea, in lieu of proper documentation for now.
 *
 * 
 * An 'obtable' is a pool from which memory regions of a 
 * fixed size (per obtable) can be allocated and freed.
 * These regions of memory are /not/ at a fixed location:
 * a later allocation or free in the same obtable can move
 * them.
 * 
 * Therefore, instead of using pointers to refer to a region
 * of memory allocated from an obtable, a small non-negative 
 * integer is used (similar to a file descriptor).   You can 
 * request the current physical address of a given region within
 * an obtable by providing the index of that object ('obtable_ref').
 * 
 * To create an obtable, allocate storage for type 't_obtable'
 * and pass the address of that storage to 'init_obtable'.
 * To free an obtable (and everything allocated from it) use
 * 'uninit_obtable'.
 * 
 * Use 'obtable_alloc' and 'obtable_free' to allocate and free
 * elements from the table.   Use 'obtable_ref' to get the 
 * temporary address of an object: the address ceases to be
 * valid after the next 'alloc' or 'free' operation on the
 * same table.
 * 
 * 'obtable_n_elts' tells you how many elements are currently
 * allocated.  It returns 0 if the indicated element is not currently
 * allocated.
 * 
 * 'obtable_room' tells you a current upper bound on the integer
 * index of allocated elements.   Thus, you can use 'obtable_room'
 * as an upper bound when iterating over object indexes to find
 * all allocated objects.
 * 
 * Sample:
 * 
 *      // a table of 'struct foo' records.
 *
 * 	t_obtable table;
 *	ssize_t an_elt_idx;
 *	struct foo * an_elt;
 * 
 * 	if (0 > init_obtable (&table, 0, sizeof (struct foo), 0, 0))
 *        ... uh oh, some kind of internal error ...;
 * 
 *      an_elt_idx = obtable_alloc (&table, 0, sizeof (struct foo), 0, 0);
 * 	if (an_elt_idx < 0)
 *        .... uh oh, allocation failure ...;
 * 
 *      an_elt = obtable_ref (&table, 0, sizeof (struct foo), 0, 0, an_elt_idx);
 *      invariant (!!an_elt);   // we know that an_elt_idx is valid 
 * 
 *      // later:
 *      obtable_free (&table, 0, sizeof (struct foo), 0, 0, an_elt_idx);
 * 
 * The extra, usually 0 parameters in the above calls are part of an 
 * interface for hooks to specially initialize and uninitialize individual
 * elements within an obtable.
 */




typedef struct obtable t_obtable;
typedef struct obtable_type t_obtable_type;

typedef int (*t_obtable_init_fn) (t_obtable * ot,
                                  alloc_limits limits,
                                  ssize_t elt_size,
                                  t_obtable_type * type,
                                  void * closure,
                                  void * mem,
                                  ssize_t n_elts);
typedef void (*t_obtable_uninit_fn) (t_obtable * ot,
                                     alloc_limits limits,
                                     ssize_t elt_size,
                                     t_obtable_type * type,
                                     void * closure,
                                     void * mem,
                                     ssize_t n_elts);



struct obtable_type
{
  t_obtable_init_fn init;
  t_obtable_uninit_fn uninit;
};

struct obtable
{
  void * _storage;
  ssize_t _n_free;
  ssize_t _free_list;
  ssize_t * _free_list_link;
};

struct obtable_elt
{
};



#define OBTABLE_STATIC_TABLE(STEM,ELT_TYPE) \
  static t_obtable * \
  STEM ## _obtable (void) \
  { \
    static t_obtable table; \
    static int initialized = 0; \
    \
    if (!initialized) \
      { \
        if (0 > init_obtable (&table, lim_use_malloc, sizeof (ELT_TYPE), 0, 0)) \
          return 0; \
        initialized = 1; \
      } \
    return &table; \
  } \
  static ssize_t STEM ## _alloc_id (void) { return obtable_alloc (STEM ## _obtable (), lim_use_malloc, sizeof (ELT_TYPE), 0, 0); } \
  static void STEM ## _free_id (ssize_t e) { obtable_free (STEM ## _obtable (), lim_use_malloc, sizeof (ELT_TYPE), 0, 0, e); } \
  static ELT_TYPE * STEM ## _ref_id (ssize_t e) { return (ELT_TYPE *)obtable_ref (STEM ## _obtable (), lim_use_malloc, sizeof (ELT_TYPE), 0, 0, e); } \
  extern ssize_t unused__ ## STEM ## __unused





/* automatically generated __STDC__ prototypes */
extern int init_obtable (t_obtable * ot,
                         alloc_limits limits,
                         ssize_t elt_size,
                         t_obtable_type * type,
                         void * closure);
extern void uninit_obtable (t_obtable * ot,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_obtable_type * type,
                            void * closure);
extern ssize_t obtable_alloc (t_obtable * ot,
                              alloc_limits limits,
                              ssize_t elt_size,
                              t_obtable_type * type,
                              void * closure);
extern void obtable_free (t_obtable * ot,
                          alloc_limits limits,
                          ssize_t elt_size,
                          t_obtable_type * type,
                          void * closure,
                          ssize_t it);
extern ssize_t obtable_n_elts (t_obtable * ot,
                               alloc_limits limits,
                               ssize_t elt_size,
                               t_obtable_type * type,
                               void * closure);
extern ssize_t obtable_is_free (t_obtable * ot,
                                alloc_limits limits,
                                ssize_t elt_size,
                                t_obtable_type * type,
                                void * closure,
                                ssize_t n);
extern void * obtable_ref (t_obtable * ot,
                           alloc_limits limits,
                           ssize_t elt_size,
                           t_obtable_type * type,
                           void * closure,
                           ssize_t n);
extern ssize_t obtable_room (t_obtable * ot,
                             alloc_limits limits,
                             ssize_t elt_size,
                             t_obtable_type * type,
                             void * closure);
#endif  /* INCLUDE__LIBAWIKI__OBTABLE_H */


/* tag: Tom Lord Fri Oct 22 15:14:34 2004 (obtable.h)
 */
