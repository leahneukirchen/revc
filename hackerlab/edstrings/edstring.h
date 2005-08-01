/* edstring.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__EDSTRING_H
#define INCLUDE__LIBAWIKI__EDSTRING_H


#include "hackerlab/oblists/oblist.h"
#include "hackerlab/obmarkers/obmarkers.h"



typedef struct edstring t_edstring;



struct edstring
{
  t_oblist _str;
  t_obmarker_table _markers;
};



/* automatically generated __STDC__ prototypes */
extern int init_edstring (t_edstring * bs,
                          alloc_limits limits,
                          ssize_t elt_size);
extern int uninit_edstring (t_edstring * bs,
                            alloc_limits limits,
                            ssize_t elt_size);
extern ssize_t edstring_room (t_edstring * bs,
                              alloc_limits limits,
                              ssize_t elt_size);
extern int edstring_is_empty (t_edstring * bs,
                              alloc_limits limits,
                              ssize_t elt_size);
extern ssize_t edstring_size (t_edstring * bs,
                              alloc_limits limits,
                              ssize_t elt_size);
extern t_uchar * edstring_burst (ssize_t * len_returned,
                                 t_edstring * bs,
                                 alloc_limits limits,
                                 ssize_t elt_size,
                                 ssize_t n,
                                 ssize_t len);
extern t_uchar * edstring_range (t_edstring * bs,
                                 alloc_limits limits,
                                 ssize_t elt_size,
                                 ssize_t n,
                                 ssize_t len);
extern int edstring_insert_n (t_edstring * bs,
                              alloc_limits limits,
                              ssize_t elt_size,
                              ssize_t before_pos,
                              t_uchar * data,
                              ssize_t len);
extern int edstring_delete_n (t_edstring * bs,
                              alloc_limits limits,
                              ssize_t elt_size,
                              ssize_t pos,
                              ssize_t len);
extern ssize_t alloc_edstring_marker (t_edstring * bs,
                                      alloc_limits limits,
                                      ssize_t elt_size,
                                      ssize_t initial_pos);
extern int free_edstring_marker (t_edstring * bs,
                                 alloc_limits limits,
                                 ssize_t elt_size,
                                 ssize_t id);
extern ssize_t edstring_n_markers (t_edstring * bs,
                                   alloc_limits limits,
                                   ssize_t elt_size);
extern ssize_t edstring_marker_room (t_edstring * bs,
                                     alloc_limits limits,
                                     ssize_t elt_size);
extern ssize_t edstring_marker_pos (t_edstring * bs,
                                    alloc_limits limits,
                                    ssize_t elt_size,
                                    ssize_t id);
extern int edstring_set_marker_pos (t_edstring * bs,
                                    alloc_limits limits,
                                    ssize_t elt_size,
                                    ssize_t id,
                                    ssize_t new_pos);
extern ssize_t edstring_marker_handle_set (t_edstring * bs,
                                           alloc_limits limits,
                                           ssize_t elt_size,
                                           ssize_t m);
#endif  /* INCLUDE__LIBAWIKI__EDSTRING_H */


/* tag: Tom Lord Sat Nov 20 20:30:58 2004 (edstrings/edstring.h)
 */
