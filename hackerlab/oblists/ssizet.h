/* ssizet.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__OBLISTS__SSIZET_H
#define INCLUDE__OBLISTS__SSIZET_H


#include "hackerlab/oblists/oblist.h"



typedef struct list_ssize_t t_list_ssize_t;



struct list_ssize_t
{
  t_oblist list;
};


/* automatically generated __STDC__ prototypes */
extern int init_list_ssize_t (t_list_ssize_t * list,
                              alloc_limits limits);
extern void uninit_list_ssize_t (t_list_ssize_t * list, alloc_limits limits);
extern int list_ssize_t_is_empty (t_list_ssize_t * list,
                                  alloc_limits limits);
extern ssize_t list_ssize_t_pop (alloc_limits limits,
                                 t_list_ssize_t * list);
extern ssize_t list_ssize_t_bag_add (alloc_limits limits,
                                     t_list_ssize_t * list,
                                     ssize_t value);
extern int list_ssize_t_remove (alloc_limits limits,
                                t_list_ssize_t * list,
                                ssize_t value);
#endif  /* INCLUDE__OBLISTS__SSIZET_H */


/* tag: Tom Lord Sat Nov 20 16:45:25 2004 (ssizet.h)
 */
