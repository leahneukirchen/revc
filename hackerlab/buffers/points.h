/* points.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__BUFFERS__POINTS_H
#define INCLUDE__BUFFERS__POINTS_H


typedef struct buffer_point t_buffer_point;



struct buffer_point
{
  ssize_t _session;
  ssize_t _buffer;
  ssize_t _marker_handle;
};



/* automatically generated __STDC__ prototypes */
extern void init_bufpt (t_buffer_point * pt);
extern int uninit_bufpt (t_buffer_point * pt);
extern int bufpt_valid (t_buffer_point * pt);
extern ssize_t bufpt_session (t_buffer_point * pt);
extern ssize_t bufpt_buffer (t_buffer_point * pt);
extern ssize_t bufpt_pos (t_buffer_point * pt);
extern int bufpt_leave_position (t_buffer_point * pt);
extern int bufpt_leave_buffer (t_buffer_point * pt);
extern int bufpt_leave_session (t_buffer_point * pt);
extern int bufpt_enter_session (t_buffer_point * pt,
                                ssize_t session);
extern int bufpt_enter_buffer (t_buffer_point * pt,
                               ssize_t session,
                               ssize_t buffer);
extern int bufpt_goto_abs (t_buffer_point * pt,
                           ssize_t session,
                           ssize_t buffer,
                           ssize_t pos);
extern int bufpt_goto (t_buffer_point * pt,
                       ssize_t pos);
extern int bufpt_switch_to_buffer (t_buffer_point * pt,
                                   ssize_t buffer,
                                   ssize_t pos);
extern int bufpt_copy_from (t_buffer_point * pt_a,
                            t_buffer_point * pt_b);
extern t_uchar * bufpt_range (t_buffer_point * pt, ssize_t len);
extern t_uchar * bufpt_burst (ssize_t * len_return,
                              t_buffer_point * pt,
                              ssize_t len);
#endif  /* INCLUDE__BUFFERS__POINTS_H */


/* tag: Tom Lord Sun Nov 21 08:39:32 2004 (points.h)
 */
