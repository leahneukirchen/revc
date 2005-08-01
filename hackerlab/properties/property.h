/* property.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__PROPERTY_H
#define INCLUDE__LIBAWIKI__PROPERTY_H


#include "hackerlab/mem/alloc-limits.h"



typedef struct property_row * t_property_table;
typedef struct property_row t_property_row;
typedef struct property_type t_property_type;
typedef struct property_fns t_property_fns;

typedef void (*t_property_init_fn) (t_property_type * type,
                                    alloc_limits limits,
                                    ssize_t pos,
                                    void * mem);

typedef void (*t_property_uninit_fn) (t_property_type * type,
                                      alloc_limits limits,
                                      ssize_t pos,
                                      void * mem);



struct property_row
{
  struct property_type * _type;
  void * _row;
};

struct property_type
{
  t_property_fns * fns;
};

struct property_fns
{
  t_uchar * debugging_name;
  ssize_t elt_size;
  t_property_init_fn init;
  t_property_uninit_fn uninit;
};



/* automatically generated __STDC__ prototypes */
extern ssize_t property_representation_id (void * key);
extern ssize_t property_global_id (void * key);
extern ssize_t property_id (ssize_t representation_id, ssize_t global_id);
extern int init_property_table (t_property_table * table,
                                alloc_limits limits);
extern int uninit_property_table (t_property_table * table,
                                  alloc_limits limits);
extern void * property_soft_ref (t_property_table * table,
                                 alloc_limits limits,
                                 ssize_t elt_size,
                                 ssize_t representation_id,
                                 ssize_t global_id,
                                 ssize_t n);
extern void * property_ref (t_property_table * table,
                            alloc_limits limits,
                            ssize_t elt_size,
                            ssize_t representation_id,
                            ssize_t global_id,
                            ssize_t n,
                            t_property_type * type);
#endif  /* INCLUDE__LIBAWIKI__PROPERTY_H */


/* tag: Tom Lord Wed Oct 27 09:22:25 2004 (property.h)
 */
