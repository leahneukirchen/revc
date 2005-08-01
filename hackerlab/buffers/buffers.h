/* buffers.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__BUFFERS__BUFFERS_H
#define INCLUDE__BUFFERS__BUFFERS_H


#include "hackerlab/properties/property.h"
#include "hackerlab/buffers/buffer.h"
#include "hackerlab/buffers/points.h"
#include "hackerlab/buffers/file-buffers.h"
#include "hackerlab/buffers/buffers-ops.h"
#include "hackerlab/buffers/edit-ops.h"


/* automatically generated __STDC__ prototypes */
extern ssize_t buffer_session_open (alloc_limits const limits,
                                    ssize_t const representation_type);
extern int buffer_session_close (alloc_limits const limits,
                                 ssize_t const id);
extern alloc_limits buffer_session_limits (ssize_t const session);
extern ssize_t buffer_session_representation_type (ssize_t const session);
extern ssize_t buffer_session_n_entries (ssize_t const session);
extern ssize_t buffer_session_room (ssize_t const session);
extern t_buffer * buffer_session_buffer_n (const t_uchar ** const name_ret,
                                           ssize_t const session,
                                           ssize_t const n);
extern t_buffer * buffer_session_nth_buffer (const t_uchar ** const name_ret,
                                             ssize_t const session,
                                             ssize_t const n);
extern t_buffer * buffer_session_find (ssize_t const session,
                                       t_uchar * const name);
extern t_buffer * buffer_session_ensure (ssize_t const session,
                                         t_uchar * const name);
extern ssize_t buffer_session_index (ssize_t const session,
                                     t_uchar * const name);
extern ssize_t buffer_session_index_create (ssize_t const session,
                                            t_uchar * const name);
extern int buffer_session_delete (ssize_t const session,
                                  t_uchar * const name);
extern int buffer_session_delete_nth (ssize_t const session,
                                      ssize_t const n);
extern void * buffer_session_property_soft_ref (ssize_t const session,
                                                ssize_t const elt_size,
                                                ssize_t const buffer,
                                                ssize_t const global_property_id);
extern void * buffer_session_property_ref (ssize_t const session,
                                           ssize_t const elt_size,
                                           ssize_t const buffer,
                                           ssize_t const global_property_id,
                                           t_property_type * const type);
#endif  /* INCLUDE__BUFFERS__BUFFERS_H */


/* tag: Tom Lord Sun Nov 21 08:01:56 2004 (buffers.h)
 */
