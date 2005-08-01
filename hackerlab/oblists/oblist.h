/* oblist.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__OBLIST_H
#define INCLUDE__LIBAWIKI__OBLIST_H


#include "hackerlab/obqueues/obqueue.h"


typedef struct oblist t_oblist;
typedef struct oblist_type t_oblist_type;


typedef int (*t_oblist_cmp_fn) (void * a, void * b, void * closure);
typedef int (*t_oblist_init_fn) (t_oblist * ot,
                                 alloc_limits limits,
                                 ssize_t elt_size,
                                 t_oblist_type * type,
                                 void * closure,
                                 void * mem,
                                 ssize_t n_elts);
typedef void (*t_oblist_uninit_fn) (t_oblist * ot,
                                    alloc_limits limits,
                                    ssize_t elt_size,
                                    t_oblist_type * type,
                                    void * closure,
                                    void * mem,
                                    ssize_t n_elts);



struct oblist_type
{
  t_oblist_init_fn init;
  t_oblist_uninit_fn uninit;
};


struct oblist
{
  struct obqueue _left;
  struct obqueue _right;
};



/* automatically generated __STDC__ prototypes */
extern int init_oblist (t_oblist * ol,
                        alloc_limits limits,
                        ssize_t elt_size,
                        t_oblist_type * type,
                        void * closure);
extern void uninit_oblist (t_oblist * ol,
                           alloc_limits limits,
                           ssize_t elt_size,
                           t_oblist_type * type,
                           void * closure);
extern int oblist_is_empty (t_oblist * ol,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_oblist_type * type,
                            void * closure);
extern ssize_t oblist_size (t_oblist * ol,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_oblist_type * type,
                            void * closure);
extern ssize_t oblist_room (t_oblist * ol,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_oblist_type * type,
                            void * closure);
extern void * oblist_burst (ssize_t * len_returned,
                            t_oblist * ol,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_oblist_type * type,
                            void * closure,
                            ssize_t n,
                            ssize_t len);
extern void * oblist_range (t_oblist * ol,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_oblist_type * type,
                            void * closure,
                            ssize_t n,
                            ssize_t len);
extern int oblist_insert_n (t_oblist * ol,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_oblist_type * type,
                            void * closure,
                            ssize_t before_pos,
                            ssize_t n,
                            void * mem);
extern int oblist_delete_n (t_oblist * ol,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_oblist_type * type,
                            void * closure,
                            ssize_t pos,
                            ssize_t n);
extern int oblist_sort (t_oblist * ol,
                        alloc_limits limits,
                        ssize_t elt_size,
                        t_oblist_type * type,
                        void * lclosure,
                        t_oblist_cmp_fn cmp,
                        void * closure);
extern ssize_t oblist_find (ssize_t * would_be_before,
                            t_oblist * ol,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_oblist_type * type,
                            void * lclosure,
                            void * key,
                            t_oblist_cmp_fn cmp,
                            void * closure);
extern ssize_t oblist_sorted_insert (t_oblist * ol,
                                     alloc_limits limits,
                                     ssize_t elt_size,
                                     t_oblist_type * type,
                                     void * lclosure,
                                     void * key,
                                     t_oblist_cmp_fn cmp,
                                     void * closure,
                                     int copy_from_key,
                                     int not_if_present);
#endif  /* INCLUDE__LIBAWIKI__OBLIST_H */


/* tag: Tom Lord Sun Oct 24 08:12:41 2004 (oblist.h)
 */
