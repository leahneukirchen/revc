/* buffer.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__BUFFERS__BUFFER_H
#define INCLUDE__BUFFERS__BUFFER_H


#include "hackerlab/rx-posix/posix.h"
#include "hackerlab/edstrings/edstring.h"



typedef struct buffer t_buffer;



struct buffer
{
  t_edstring _string;
};



/* automatically generated __STDC__ prototypes */
extern int init_buffer (t_buffer * bs,
                        alloc_limits limits);
extern int uninit_buffer (t_buffer * bs,
                          alloc_limits limits);
extern ssize_t buffer_room (t_buffer * bs,
                            alloc_limits limits);
extern int buffer_is_empty (t_buffer * bs,
                            alloc_limits limits);
extern ssize_t buffer_size (t_buffer * bs,
                            alloc_limits limits);
extern int buffer_n (t_buffer * bs,
                     alloc_limits limits,
                     ssize_t n);
extern t_uchar * buffer_burst (ssize_t * len_returned,
                               t_buffer * bs,
                               alloc_limits limits,
                               ssize_t n,
                               ssize_t len);
extern t_uchar * buffer_range (t_buffer * bs,
                               alloc_limits limits,
                               ssize_t n,
                               ssize_t len);
extern int buffer_insert_n (t_buffer * bs,
                            alloc_limits limits,
                            ssize_t before_pos,
                            t_uchar * data,
                            ssize_t len);
extern int buffer_delete_n (t_buffer * bs,
                            alloc_limits limits,
                            ssize_t pos,
                            ssize_t len);
extern ssize_t alloc_buffer_marker (t_buffer * bs,
                                    alloc_limits limits,
                                    ssize_t initial_pos);
extern int free_buffer_marker (t_buffer * bs,
                               alloc_limits limits,
                               ssize_t id);
extern ssize_t buffer_n_markers (t_buffer * bs,
                                 alloc_limits limits);
extern ssize_t buffer_marker_room (t_buffer * bs,
                                   alloc_limits limits);
extern ssize_t buffer_marker_pos (t_buffer * bs,
                                  alloc_limits limits,
                                  ssize_t id);
extern int buffer_set_marker_pos (t_buffer * bs,
                                  alloc_limits limits,
                                  ssize_t id,
                                  ssize_t new_pos);
extern ssize_t buffer_marker_handle_set (t_buffer * bs,
                                         alloc_limits limits,
                                         ssize_t marker_m);
#endif  /* INCLUDE__BUFFERS__BUFFER_H */


/* tag: Tom Lord Sun Nov 21 06:55:33 2004 (buffer.h)
 */
