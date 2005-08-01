/* buffers-ops.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__BUFFERS__BUFFERS_OPS_H
#define INCLUDE__BUFFERS__BUFFERS_OPS_H



/* automatically generated __STDC__ prototypes */
extern ssize_t buf_room (ssize_t session, ssize_t buf);
extern int buf_is_empty (ssize_t session, ssize_t buf);
extern ssize_t buf_size (ssize_t session, ssize_t buf);
extern int buf_n (ssize_t session, ssize_t buf, ssize_t n);
extern t_uchar * buf_burst (ssize_t * len_returned,
                            ssize_t session,
                            ssize_t buf,
                            ssize_t n,
                            ssize_t len);
extern t_uchar * buf_range (ssize_t session,
                            ssize_t buf,
                            ssize_t n,
                            ssize_t len);
extern int buf_insert_str_n (ssize_t session,
                             ssize_t buf,
                             ssize_t before_pos,
                             t_uchar * data,
                             ssize_t len);
extern int buf_delete_n (ssize_t session,
                         ssize_t buf,
                         ssize_t pos,
                         ssize_t len);
#endif  /* INCLUDE__BUFFERS__BUFFERS_OPS_H */


/* tag: Tom Lord Sun Nov 21 10:10:19 2004 (buffers-ops.h)
 */
