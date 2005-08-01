/* edstring.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/edstrings/edstring.h"


/* __STDC__ prototypes for static functions */



int
init_edstring (t_edstring * bs,
               alloc_limits limits,
               ssize_t elt_size)
{
  if (!bs)
    return -1;

  if (0 > init_oblist (&bs->_str, limits, elt_size, 0, 0))
    return -1;

  if (0 > init_obmarker_table (&bs->_markers, limits))
    return -1;

  return 0;
}


int
uninit_edstring (t_edstring * bs,
                 alloc_limits limits,
                 ssize_t elt_size)
{
  if (!bs)
    return -1;

  if (0 > uninit_obmarker_table (&bs->_markers, limits))
    return -1;

  uninit_oblist (&bs->_str, limits, elt_size, 0, 0);

  return 0;
}




ssize_t
edstring_room (t_edstring * bs,
               alloc_limits limits,
               ssize_t elt_size)
{
  if (!bs)
    return 0;
  return oblist_room (&bs->_str, limits, elt_size, 0, 0);
}


int
edstring_is_empty (t_edstring * bs,
                   alloc_limits limits,
                   ssize_t elt_size)
{
  if (!bs)
    return 0;
  return oblist_is_empty (&bs->_str, limits, elt_size, 0, 0);
}


ssize_t
edstring_size (t_edstring * bs,
               alloc_limits limits,
               ssize_t elt_size)
{
  if (!bs)
    return 0;
  return oblist_size (&bs->_str, limits, elt_size, 0, 0);
}


t_uchar *
edstring_burst (ssize_t * len_returned,
                t_edstring * bs,
                alloc_limits limits,
                ssize_t elt_size,
                ssize_t n,
                ssize_t len)
{
  if (!bs)
    return 0;
  return (t_uchar *)oblist_burst (len_returned, &bs->_str, limits, elt_size, 0, 0, n, len);
}


t_uchar *
edstring_range (t_edstring * bs,
                alloc_limits limits,
                ssize_t elt_size,
                ssize_t n,
                ssize_t len)
{
  if (!bs)
    return 0;
  return (t_uchar *)oblist_range (&bs->_str, limits, elt_size, 0, 0, n, len);
}



int
edstring_insert_n (t_edstring * bs,
                   alloc_limits limits,
                   ssize_t elt_size,
                   ssize_t before_pos,
                   t_uchar * data,
                   ssize_t len)
{
  if (!bs)
    return -1;
  if (0 > oblist_insert_n (&bs->_str, limits, elt_size, 0, 0, before_pos, len, (void *)data))
    return -1;
  if (0 > obmarker_insert_n (&bs->_markers, limits, before_pos, len))
    return -1;
  return 0;
}


int
edstring_delete_n (t_edstring * bs,
                   alloc_limits limits,
                   ssize_t elt_size,
                   ssize_t pos,
                   ssize_t len)
{
  if (!bs)
    return -1;
  if (0 > oblist_delete_n (&bs->_str, limits, elt_size, 0, 0, pos, len))
    return -1;
  if (0 > obmarker_delete_n (&bs->_markers, limits, pos, len))
    return -1;
  return 0;
}




ssize_t
alloc_edstring_marker (t_edstring * bs,
                       alloc_limits limits,
                       ssize_t elt_size,
                       ssize_t initial_pos)
{
  ssize_t str_size;

  if (!bs)
    return -1;

  str_size = edstring_size (bs, limits, elt_size);
  if ((initial_pos < -1) || (initial_pos > str_size))
    return -1;

  return obmarker_alloc (&bs->_markers, limits, initial_pos);
}


int
free_edstring_marker (t_edstring * bs,
                      alloc_limits limits,
                      ssize_t elt_size,
                      ssize_t id)
{
  return obmarker_free (&bs->_markers, limits, id);
}


ssize_t 
edstring_n_markers (t_edstring * bs,
                    alloc_limits limits,
                    ssize_t elt_size)
{
  if (!bs)
    return 0;
  else
    return obmarker_n_markers (&bs->_markers, limits);
}


ssize_t 
edstring_marker_room (t_edstring * bs,
                      alloc_limits limits,
                      ssize_t elt_size)
{
  if (!bs)
    return 0;
  else
    return obmarker_room (&bs->_markers, limits);
}



ssize_t
edstring_marker_pos (t_edstring * bs,
                     alloc_limits limits,
                     ssize_t elt_size,
                     ssize_t id)
{
  if (!bs)
    return -1;

  return obmarker_pos (&bs->_markers, limits, id);
}


int
edstring_set_marker_pos (t_edstring * bs,
                         alloc_limits limits,
                         ssize_t elt_size,
                         ssize_t id,
                         ssize_t new_pos)
{
  if (!bs)
    return -1;

  return obmarker_set_pos (&bs->_markers, limits, id, new_pos);
}




ssize_t
edstring_marker_handle_set (t_edstring * bs,
                            alloc_limits limits,
                            ssize_t elt_size,
                            ssize_t m)
{
  if (!bs)
    return -1;

  return obmarker_handle_set (&bs->_markers, limits, m);
}



/* tag: Tom Lord Sat Nov 20 20:31:40 2004 (edstrings/edstring.c)
 */
