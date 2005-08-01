/* points.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/handles/handle.h"
#include "hackerlab/buffers/buffers.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */


/* code */

void
init_bufpt (t_buffer_point * pt)
{
  if (pt)
    {
      pt->_session = -1;
      pt->_buffer = -1;
      pt->_marker_handle = -1;
    }
}


int
uninit_bufpt (t_buffer_point * pt)
{
  if (   pt
      && (0 <= pt->_marker_handle)
      && (0 > free_handle (pt->_marker_handle)))
    {
      return -1;
    }

  init_bufpt (pt);

  return 0;
}



int
bufpt_valid (t_buffer_point * pt)
{
  if (!pt)
    return 0;

  if (   (0 > pt->_session)
      || (0 > pt->_buffer)
      || (0 > pt->_marker_handle)
      || (0 > handle_ref (pt->_marker_handle)))
    return 0;

  return 1;
}


ssize_t
bufpt_session (t_buffer_point * pt)
{
  if (!pt)
    return -1;

  return pt->_session;
}


ssize_t
bufpt_buffer (t_buffer_point * pt)
{
  if (!pt)
    return -1;

  return pt->_buffer;
}


ssize_t
bufpt_pos (t_buffer_point * pt)
{
  ssize_t marker;
  t_buffer * buf;

  if (!pt)
    return -1;

  if (   (0 > pt->_session)
      || (0 > pt->_buffer)
      || (0 > pt->_marker_handle))
    return -1;

  marker = handle_ref (pt->_marker_handle);
  if (marker < 0)
    return -1;

  buf = buffer_session_buffer_n (0, pt->_session, pt->_buffer);
  if (!buf)
    return -1;

  return buffer_marker_pos (buf, buffer_session_limits (pt->_session), marker);
}


int
bufpt_leave_position (t_buffer_point * pt)
{
  ssize_t marker;
  t_buffer * buf;

  if (   (0 > pt->_session)
      || (0 > pt->_buffer)
      || (0 > pt->_marker_handle))
    return 0;

  marker = handle_ref (pt->_marker_handle);
  if (marker < 0)
    return 0;

  buf = buffer_session_buffer_n (0, pt->_session, pt->_buffer);
  if (!buf)
    return -1;

  return free_buffer_marker (buf, buffer_session_limits (pt->_session), marker);
}


int
bufpt_leave_buffer (t_buffer_point * pt)
{
  if (bufpt_leave_position (pt))
    return -1;

  pt->_buffer = -1;
  return 0;
}


int
bufpt_leave_session (t_buffer_point * pt)
{
  if (bufpt_leave_buffer (pt))
    return -1;

  pt->_session = -1;
  return 0;
}



int
bufpt_enter_session (t_buffer_point * pt,
                     ssize_t session)
{
  if (!pt)
    return -1;

  if (pt->_session == session)
    return 0;

  if (bufpt_leave_session (pt))
    return -1;

  pt->_session = session;

  return 0;
}


int
bufpt_enter_buffer (t_buffer_point * pt,
                    ssize_t session,
                    ssize_t buffer)
{
  if (0 > bufpt_enter_session (pt, session))
    return -1;

  if (pt->_buffer == buffer)
    return 0;

  if (0 > bufpt_leave_buffer (pt))
    return -1;

  pt->_buffer = buffer;

  return 0;
}


int
bufpt_goto_abs (t_buffer_point * pt,
                ssize_t session,
                ssize_t buffer,
                ssize_t pos)
{
  t_buffer * buf;
  ssize_t marker;

  if (0 > bufpt_enter_buffer (pt, session, buffer))
    return -1;

  buf = buffer_session_buffer_n (0, pt->_session, pt->_buffer);
  if (!buf)
    return -1;

  if (pt->_marker_handle < 0)
    {
      pt->_marker_handle = alloc_handle ();
      if (pt->_marker_handle < 0)
        return -1;
    }

  marker = handle_ref (pt->_marker_handle);
  if (marker < 0)
    {
      marker = alloc_buffer_marker (buf, buffer_session_limits (pt->_session), pos);
      if (marker < 0)
        return -1;

      if (0 > handle_set (pt->_marker_handle, buffer_marker_handle_set (buf, buffer_session_limits (pt->_session), marker), marker))
        return -1;

      return 0;
    }
  else
    {
      if (0 > buffer_set_marker_pos (buf, buffer_session_limits (pt->_session), marker, pos))
        return -1;

      return 0;
    }
}



int
bufpt_goto (t_buffer_point * pt,
            ssize_t pos)
{
  return bufpt_goto_abs (pt, bufpt_session (pt), bufpt_buffer (pt), pos);
}


int
bufpt_switch_to_buffer (t_buffer_point * pt,
                        ssize_t buffer,
                        ssize_t pos)
{
  return bufpt_goto_abs (pt, bufpt_session (pt), buffer, pos);
}


int
bufpt_copy_from (t_buffer_point * pt_a,
                 t_buffer_point * pt_b)
{
  return bufpt_goto_abs (pt_a, bufpt_session (pt_b), bufpt_buffer (pt_b), bufpt_pos (pt_b));
}




t_uchar *
bufpt_range (t_buffer_point * pt, ssize_t len)
{
  ssize_t bset;
  ssize_t buf;
  ssize_t pos;

  bset = bufpt_session (pt);
  buf = bufpt_buffer (pt);
  pos = bufpt_pos (pt);

  if ((bset < 0) || (buf < 0) || (pos < 0))
    return 0;

  return buf_range (bset, buf, pos, len);
}


t_uchar *
bufpt_burst (ssize_t * len_return,
             t_buffer_point * pt,
             ssize_t len)
{
  ssize_t bset;
  ssize_t buf;
  ssize_t pos;

  bset = bufpt_session (pt);
  buf = bufpt_buffer (pt);
  pos = bufpt_pos (pt);

  if ((bset < 0) || (buf < 0) || (pos < 0))
    return 0;

  return buf_burst (len_return, bset, buf, pos, len);
}






/* tag: Tom Lord Sun Nov 21 08:40:05 2004 (points.c)
 */
