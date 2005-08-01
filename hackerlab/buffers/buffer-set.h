/* buffer-set.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__BUFFER_SET_H
#define INCLUDE__LIBAWIKI__BUFFER_SET_H


#include "hackerlab/identifiers/symbol-table.h"
#include "hackerlab/properties/property.h"
#include "hackerlab/buffers/buffer.h"



typedef struct buffer_set t_buffer_set;



struct buffer_set
{
  t_symbol_table _buffers;
  alloc_limits _limits;
  ssize_t _representation_type;
  t_property_table _property_table;
};



/* automatically generated __STDC__ prototypes */
extern int init_buffer_set (t_buffer_set * const bs,
                            alloc_limits const limits,
                            ssize_t const representation_type);
extern int uninit_buffer_set (t_buffer_set * const bs);
extern alloc_limits buffer_set_limits (t_buffer_set * const bs);
extern ssize_t buffer_set_representation_type (t_buffer_set * const bs);
extern ssize_t buffer_set_n_entries (t_buffer_set * const bs);
extern ssize_t buffer_set_room (t_buffer_set * const bs);
extern t_buffer * buffer_set_buffer_n (const t_uchar ** const name_ret,
                                       t_buffer_set * const bs,
                                       ssize_t const n);
extern t_buffer * buffer_set_nth_buffer (const t_uchar ** const name_ret,
                                         t_buffer_set * bs,
                                         ssize_t n);
extern t_buffer * buffer_set_find (t_buffer_set * const bs,
                                   const t_uchar * const name);
extern t_buffer * buffer_set_ensure (t_buffer_set * const bs,
                                     const t_uchar * const name);
extern ssize_t buffer_set_index (t_buffer_set * const bs,
                                 const t_uchar * const name);
extern int buffer_set_delete (t_buffer_set * const bs,
                              const t_uchar * const name);
extern int buffer_set_delete_nth (t_buffer_set * const bs,
                                  ssize_t const n);
extern void * buffer_set_property_soft_ref (t_buffer_set * const bs,
                                            ssize_t const elt_size,
                                            ssize_t const buffer,
                                            ssize_t const global_property_id);
extern void * buffer_set_property_ref (t_buffer_set * const bs,
                                       ssize_t const elt_size,
                                       ssize_t const buffer,
                                       ssize_t const global_property_id,
                                       t_property_type * const type);
#endif  /* INCLUDE__LIBAWIKI__BUFFER_SET_H */


/* tag: Tom Lord Wed Oct 27 12:29:30 2004 (buffer-set.h)
 */
