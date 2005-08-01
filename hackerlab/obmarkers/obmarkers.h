/* obmarkers.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBDS__OBMARKERS_H
#define INCLUDE__LIBDS__OBMARKERS_H


#include "hackerlab/obtables/obtable.h"
#include "hackerlab/oblists/oblist.h"



typedef struct obmarker_table t_obmarker_table;
typedef struct obmarker t_obmarker;



struct obmarker_table
{
  t_obtable _markers;
  t_oblist _in_order;
  ssize_t _gap;
  ssize_t _size;
};

struct obmarker
{
  struct obtable_elt _obtable_elt;
  ssize_t _pos;
  ssize_t _handle_set;
};


/* automatically generated __STDC__ prototypes */
extern int init_obmarker_table (t_obmarker_table * table,
                                alloc_limits limits);
extern int uninit_obmarker_table (t_obmarker_table * table,
                                  alloc_limits limits);
extern ssize_t obmarker_alloc (t_obmarker_table * table,
                               alloc_limits limits,
                               ssize_t init_pos);
extern int obmarker_free (t_obmarker_table * table,
                          alloc_limits limits,
                          ssize_t m);
extern ssize_t obmarker_n_markers (t_obmarker_table * table,
                                   alloc_limits limits);
extern ssize_t obmarker_room (t_obmarker_table * table,
                              alloc_limits limits);
extern ssize_t obmarker_pos (t_obmarker_table * table,
                             alloc_limits limits,
                             ssize_t m);
extern int obmarker_set_pos (t_obmarker_table * table,
                             alloc_limits limits,
                             ssize_t m,
                             ssize_t to);
extern int obmarker_insert_n (t_obmarker_table * table,
                              alloc_limits limits,
                              ssize_t pos,
                              ssize_t n);
extern int obmarker_delete_n (t_obmarker_table * table,
                              alloc_limits limits,
                              ssize_t pos,
                              ssize_t n);
extern ssize_t obmarker_handle_set (t_obmarker_table * table,
                                    alloc_limits limits,
                                    ssize_t id);
#endif  /* INCLUDE__LIBDS__OBMARKERS_H */


/* tag: Tom Lord Sat Oct 30 13:58:48 2004 (obmarkers.h)
 */
