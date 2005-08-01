/* obqueue.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__OBQUEUE_H
#define INCLUDE__LIBAWIKI__OBQUEUE_H


#include "hackerlab/mem/alloc-limits.h"



typedef struct obqueue t_obqueue;
typedef struct obqueue_type t_obqueue_type;

typedef int (*t_obqueue_init_fn) (t_obqueue * ot,
                                  alloc_limits limits,
                                  ssize_t elt_size,
                                  t_obqueue_type * type,
                                  void * closure,
                                  void * mem,
                                  ssize_t n_elts);
typedef void (*t_obqueue_uninit_fn) (t_obqueue * ot,
                                     alloc_limits limits,
                                     ssize_t elt_size,
                                     t_obqueue_type * type,
                                     void * closure,
                                     void * mem,
                                     ssize_t n_elts);



struct obqueue_type
{
  t_obqueue_init_fn init;
  t_obqueue_uninit_fn uninit;
};


struct obqueue
{
  ssize_t _head;
  ssize_t _tail;
  t_uchar * _storage;
};



/* automatically generated __STDC__ prototypes */
extern int init_obqueue (t_obqueue * oq,
                         alloc_limits limits,
                         ssize_t elt_size,
                         t_obqueue_type * type,
                         void * closure);
extern void uninit_obqueue (t_obqueue * oq,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_obqueue_type * type,
                            void * closure);
extern ssize_t obqueue__room (t_obqueue * oq,
                              alloc_limits limits,
                              ssize_t elt_size,
                              t_obqueue_type * type,
                              void * closure);
extern int obqueue_is_empty (t_obqueue * oq,
                             alloc_limits limits,
                             ssize_t elt_size,
                             t_obqueue_type * type,
                             void * closure);
extern int obqueue_size (t_obqueue * oq,
                         alloc_limits limits,
                         ssize_t elt_size,
                         t_obqueue_type * type,
                         void * closure);
extern ssize_t obqueue__nth_place (t_obqueue * oq,
                                   alloc_limits limits,
                                   ssize_t elt_size,
                                   t_obqueue_type * type,
                                   void * closure,
                                   ssize_t n);
extern void * obqueue_peek (t_obqueue * oq,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_obqueue_type * type,
                            void * closure);
extern void * obqueue_n (t_obqueue * oq,
                         alloc_limits limits,
                         ssize_t elt_size,
                         t_obqueue_type * type,
                         void * closure,
                         ssize_t n);
extern void * obqueue_burst (ssize_t * len_returned,
                             t_obqueue * oq,
                             alloc_limits limits,
                             ssize_t elt_size,
                             t_obqueue_type * type,
                             void * closure,
                             ssize_t n,
                             ssize_t len);
extern void * obqueue_range (t_obqueue * oq,
                             alloc_limits limits,
                             ssize_t elt_size,
                             t_obqueue_type * type,
                             void * closure,
                             ssize_t n,
                             ssize_t len);
extern void * obqueue_push (t_obqueue * oq,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_obqueue_type * type,
                            void * closure);
extern int obqueue_push_n (t_obqueue * oq,
                           alloc_limits limits,
                           ssize_t elt_size,
                           t_obqueue_type * type,
                           void * closure,
                           ssize_t n);
extern void obqueue_pop (t_obqueue * oq,
                         alloc_limits limits,
                         ssize_t elt_size,
                         t_obqueue_type * type,
                         void * closure);
extern void obqueue_pop_n (t_obqueue * oq,
                           alloc_limits limits,
                           ssize_t elt_size,
                           t_obqueue_type * type,
                           void * closure,
                           ssize_t n);
extern void * obqueue_append (t_obqueue * oq,
                              alloc_limits limits,
                              ssize_t elt_size,
                              t_obqueue_type * type,
                              void * closure);
extern int obqueue_append_n (t_obqueue * oq,
                             alloc_limits limits,
                             ssize_t elt_size,
                             t_obqueue_type * type,
                             void * closure,
                             ssize_t n);
extern void obqueue_pop_last (t_obqueue * oq,
                              alloc_limits limits,
                              ssize_t elt_size,
                              t_obqueue_type * type,
                              void * closure);
extern void obqueue_pop_last_n (t_obqueue * oq,
                                alloc_limits limits,
                                ssize_t elt_size,
                                t_obqueue_type * type,
                                void * closure,
                                ssize_t n);
#endif  /* INCLUDE__LIBAWIKI__OBQUEUE_H */


/* tag: Tom Lord Fri Oct 22 16:25:11 2004 (obqueue.h)
 */
