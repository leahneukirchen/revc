/* buffer.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/buffers/buffer.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */


/* code */

int
init_buffer (t_buffer * bs,
             alloc_limits limits)
{
  return init_edstring (&bs->_string, limits, sizeof (t_uchar));
}

int
uninit_buffer (t_buffer * bs,
               alloc_limits limits)
{
  return uninit_edstring (&bs->_string, limits, sizeof (t_uchar));
}


ssize_t
buffer_room (t_buffer * bs,
             alloc_limits limits)
{
  return edstring_room (&bs->_string, limits, sizeof (t_uchar));
}


int
buffer_is_empty (t_buffer * bs,
                 alloc_limits limits)
{
  return edstring_is_empty (&bs->_string, limits, sizeof (t_uchar));
}


ssize_t
buffer_size (t_buffer * bs,
             alloc_limits limits)
{
  return edstring_size (&bs->_string, limits, sizeof (t_uchar));
}


int
buffer_n (t_buffer * bs,
          alloc_limits limits,
          ssize_t n)
{
  t_uchar * box;

  box = buffer_burst (0, bs, limits, n, 1);
  if (!box)
    return -1;

  return *box;
}


t_uchar *
buffer_burst (ssize_t * len_returned,
              t_buffer * bs,
              alloc_limits limits,
              ssize_t n,
              ssize_t len)
{
  return (t_uchar *)edstring_burst (len_returned, &bs->_string, limits, sizeof (t_uchar), n, len);
}


t_uchar *
buffer_range (t_buffer * bs,
              alloc_limits limits,
              ssize_t n,
              ssize_t len)
{
  return (t_uchar *)edstring_range (&bs->_string, limits, sizeof (t_uchar), n, len);
}


int
buffer_insert_n (t_buffer * bs,
                 alloc_limits limits,
                 ssize_t before_pos,
                 t_uchar * data,
                 ssize_t len)
{
  return edstring_insert_n (&bs->_string, limits, sizeof(t_uchar), before_pos, data, len);
}


int
buffer_delete_n (t_buffer * bs,
                 alloc_limits limits,
                 ssize_t pos,
                 ssize_t len)
{
  return edstring_delete_n (&bs->_string, limits, sizeof(t_uchar), pos, len);
}




ssize_t
alloc_buffer_marker (t_buffer * bs,
                     alloc_limits limits,
                     ssize_t initial_pos)
{
  return alloc_edstring_marker (&bs->_string, limits, sizeof (t_uchar), initial_pos);
}


int
free_buffer_marker (t_buffer * bs,
                    alloc_limits limits,
                    ssize_t id)
{
  return free_edstring_marker (&bs->_string, limits, sizeof (t_uchar), id);
}


ssize_t
buffer_n_markers (t_buffer * bs,
                  alloc_limits limits)
{
  return edstring_n_markers (&bs->_string, limits, sizeof (t_uchar));
}


ssize_t
buffer_marker_room (t_buffer * bs,
                    alloc_limits limits)
{
  return edstring_marker_room (&bs->_string, limits, sizeof (t_uchar));
}


ssize_t
buffer_marker_pos (t_buffer * bs,
                   alloc_limits limits,
                   ssize_t id)
{
  return edstring_marker_pos (&bs->_string, limits, sizeof (t_uchar), id);
}


int
buffer_set_marker_pos (t_buffer * bs,
                       alloc_limits limits,
                       ssize_t id,
                       ssize_t new_pos)
{
  return edstring_set_marker_pos (&bs->_string, limits, sizeof (t_uchar), id, new_pos);
}




ssize_t 
buffer_marker_handle_set (t_buffer * bs,
                          alloc_limits limits,
                          ssize_t marker_id)
{
  return edstring_marker_handle_set (&bs->_string, limits, sizeof (t_uchar), marker_id);
}



/* tag: Tom Lord Sun Nov 21 06:55:36 2004 (buffer.c)
 */
