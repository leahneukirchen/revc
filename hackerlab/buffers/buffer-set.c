/* buffer-set.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/dynid/dynid.h"
#include "hackerlab/buffers/buffer.h"
#include "hackerlab/buffers/buffer-set.h"



int
init_buffer_set (t_buffer_set * const bs,
                 alloc_limits const limits,
                 ssize_t const representation_type)
{
  if (!bs)
    return -1;

  mem_set0 ((t_uchar *)bs, sizeof (*bs));

  if (0 > init_symbol_table (&bs->_buffers, limits, sizeof (t_buffer)))
    return -1;

  bs->_limits = limits;

  if (representation_type > 0)
    bs->_representation_type = representation_type;
  else
    {
      bs->_representation_type = ensure_global_dynid ((void *)init_buffer_set);
      if (bs->_representation_type < 0)
        return -1;
    }

  if (0 > init_property_table (&bs->_property_table, limits))
    return -1;

  return 0;
}



int
uninit_buffer_set (t_buffer_set * const bs)
{
  if (!bs)
    return -1;

  while (buffer_set_n_entries (bs))
    {
      if (0 > buffer_set_delete_nth (bs, 0))
        return -1;
    }

  if (0 > uninit_symbol_table (&bs->_buffers, bs->_limits, sizeof (t_buffer)))
    return -1;

  if (0 > uninit_property_table (&bs->_property_table, bs->_limits))
    return -1;

  mem_set0 ((t_uchar *)bs, sizeof (*bs));

  return 0;
}



alloc_limits
buffer_set_limits (t_buffer_set * const bs)
{
  if (!bs)
    return 0;

  return bs->_limits;
}


ssize_t
buffer_set_representation_type (t_buffer_set * const bs)
{
  if (!bs)
    return 0;

  return bs->_representation_type;
}


ssize_t
buffer_set_n_entries (t_buffer_set * const bs)
{
  if (!bs)
    return 0;

  return symbol_table_n_entries (&bs->_buffers, bs->_limits, sizeof (t_buffer));
}


ssize_t
buffer_set_room (t_buffer_set * const bs)
{
  if (!bs)
    return 0;

  return symbol_table_room (&bs->_buffers, bs->_limits, sizeof (t_buffer));
}


t_buffer *
buffer_set_buffer_n (const t_uchar ** const name_ret,
                     t_buffer_set * const bs,
                     ssize_t const n)
{
  if (!bs)
    return 0;

  return (t_buffer *)symbol_table_entry_n (name_ret, &bs->_buffers, bs->_limits, sizeof (t_buffer), n);
}


t_buffer *
buffer_set_nth_buffer (const t_uchar ** const name_ret,
                       t_buffer_set * bs,
                       ssize_t n)
{
  if (!bs)
    return 0;

  return (t_buffer *)symbol_table_nth_entry (name_ret, &bs->_buffers, bs->_limits, sizeof (t_buffer), n);
}


t_buffer *
buffer_set_find (t_buffer_set * const bs,
                 const t_uchar * const name)
{
  if (!bs)
    return 0;

  return (t_buffer *)symbol_table_entry_find (&bs->_buffers, bs->_limits, sizeof (t_buffer), name);
}


t_buffer *
buffer_set_ensure (t_buffer_set * const bs,
                   const t_uchar * const name)
{
  t_buffer * answer;

  if (!bs)
    return 0;

  answer = buffer_set_find (bs, name);
  if (answer)
    return answer;

  answer = (t_buffer *)symbol_table_entry_ensure (&bs->_buffers, bs->_limits, sizeof (t_buffer), name);
  if (!answer)
    return 0;

  if (0 > init_buffer (answer, bs->_limits))
    return 0;

  return answer;
}


ssize_t
buffer_set_index (t_buffer_set * const bs,
                  const t_uchar * const name)
{
  return symbol_table_entry_index (&bs->_buffers, bs->_limits, sizeof (t_buffer), name);
}




int
buffer_set_delete (t_buffer_set * const bs,
                   const t_uchar * const name)
{
  t_buffer * answer;

  answer = buffer_set_find (bs, name);
  if (!answer)
    return -1;

  if (0 > uninit_buffer (answer, bs->_limits))
    return -1;

  return symbol_table_entry_delete (&bs->_buffers, bs->_limits, sizeof (t_buffer), name);
}


int
buffer_set_delete_nth (t_buffer_set * const bs,
                       ssize_t const n)
{
  const t_uchar * name;
  t_buffer * answer;


  answer = buffer_set_nth_buffer (&name, bs, n);
  if (!answer)
    return -1;

  if (0 > uninit_buffer (answer, bs->_limits))
    return -1;

  return symbol_table_entry_delete (&bs->_buffers, bs->_limits, sizeof (t_buffer), name);
}



void *
buffer_set_property_soft_ref (t_buffer_set * const bs,
                              ssize_t const elt_size,
                              ssize_t const buffer,
                              ssize_t const global_property_id)
{
  if (!bs)
    return 0;

  return property_soft_ref (&bs->_property_table, bs->_limits, elt_size, bs->_representation_type, global_property_id, buffer);
}


void *
buffer_set_property_ref (t_buffer_set * const bs,
                         ssize_t const elt_size,
                         ssize_t const buffer,
                         ssize_t const global_property_id,
                         t_property_type * const type)
{
  if (!bs)
    return 0;

  return property_ref (&bs->_property_table, bs->_limits, elt_size, bs->_representation_type, global_property_id, buffer, type);
}




/* tag: Tom Lord Wed Oct 27 12:35:15 2004 (buffer-set.c)
 */
