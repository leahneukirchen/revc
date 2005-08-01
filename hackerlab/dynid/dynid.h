/* dynid.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__DYNID_H
#define INCLUDE__LIBAWIKI__DYNID_H


#include "hackerlab/oblists/oblist.h"



typedef struct dynid_table t_dynid_table;
typedef struct dynid_record t_dynid_record;



struct dynid_table
{
  t_oblist id_table;
};

struct dynid_record
{
  void * key;
  ssize_t id;
};



/* automatically generated __STDC__ prototypes */
extern int init_dynid_table (t_dynid_table * dt, alloc_limits limits);
extern void uninit_dynid_table (t_dynid_table * dt, alloc_limits limits);
extern ssize_t dynid_find (t_dynid_table * dt, alloc_limits limits, void * key);
extern ssize_t dynid_ensure (t_dynid_table * dt, alloc_limits limits, void * key);
extern ssize_t find_global_dynid (void * key);
extern ssize_t ensure_global_dynid (void * key);
#endif  /* INCLUDE__LIBAWIKI__DYNID_H */


/* tag: Tom Lord Sun Oct 24 21:49:16 2004 (dynid.h)
 */
