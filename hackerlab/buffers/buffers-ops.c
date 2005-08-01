/* buffers-ops.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/buffers/buffers.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */



ssize_t
buf_room (ssize_t session, ssize_t buf)
{
  t_buffer * bf;

  bf = buffer_session_buffer_n (0, session, buf);
  if (!bf)
    return -1;

  return buffer_room (bf, buffer_session_limits (session));
}


int
buf_is_empty (ssize_t session, ssize_t buf)
{
  t_buffer * bf;

  bf = buffer_session_buffer_n (0, session, buf);
  if (!bf)
    return -1;

  return buffer_is_empty (bf, buffer_session_limits (session));
}


ssize_t
buf_size (ssize_t session, ssize_t buf)
{
  t_buffer * bf;

  bf = buffer_session_buffer_n (0, session, buf);
  if (!bf)
    return -1;

  return buffer_size (bf, buffer_session_limits (session));
}


int
buf_n (ssize_t session, ssize_t buf, ssize_t n)
{
  t_buffer * bf;

  bf = buffer_session_buffer_n (0, session, buf);
  if (!bf)
    return -1;

  return buffer_n (bf, buffer_session_limits (session), n);
}


t_uchar *
buf_burst (ssize_t * len_returned,
           ssize_t session,
           ssize_t buf,
           ssize_t n,
           ssize_t len)
{
  t_buffer * bf;

  bf = buffer_session_buffer_n (0, session, buf);
  if (!bf)
    return 0;

  return buffer_burst (len_returned, bf, buffer_session_limits (session), n, len);
}


t_uchar *
buf_range (ssize_t session,
           ssize_t buf,
           ssize_t n,
           ssize_t len)
{
  t_buffer * bf;

  bf = buffer_session_buffer_n (0, session, buf);
  if (!bf)
    return 0;

  return buffer_range (bf, buffer_session_limits (session), n, len);
}


int
buf_insert_str_n (ssize_t session,
                  ssize_t buf,
                  ssize_t before_pos,
                  t_uchar * data,
                  ssize_t len)
{
  t_buffer * bf;

  bf = buffer_session_buffer_n (0, session, buf);
  if (!bf)
    return -1;

  return buffer_insert_n (bf, buffer_session_limits (session), before_pos, data, len);
}


int
buf_delete_n (ssize_t session,
              ssize_t buf,
              ssize_t pos,
              ssize_t len)
{
  t_buffer * bf;

  bf = buffer_session_buffer_n (0, session, buf);
  if (!bf)
    return -1;

  return buffer_delete_n (bf, buffer_session_limits (session), pos, len);
}



/* tag: Tom Lord Sun Nov 21 10:10:14 2004 (buffers-ops.c)
 */
