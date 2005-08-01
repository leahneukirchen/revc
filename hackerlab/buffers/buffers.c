/* buffers.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/dynid/dynid.h"
#include "hackerlab/buffers/buffer-set.h"
#include "hackerlab/buffers/buffers.h"


/* typedefs */

typedef struct buffer_set_entry t_buffer_set_entry;

struct buffer_set_entry
{
  t_buffer_set set;
};

OBTABLE_STATIC_TABLE (buffer_set, t_buffer_set_entry);


/* __STDC__ prototypes for static functions */


/* declarations */


/* code */

ssize_t
buffer_session_open (alloc_limits const limits,
                     ssize_t const representation_type_spec)
{
  ssize_t it_desc;
  struct buffer_set_entry * it;
  ssize_t representation_type;

  it_desc = buffer_set_alloc_id ();
  it = buffer_set_ref_id (it_desc);
  if (!it)
    return -1;


  if (representation_type > 0)
    {
      representation_type = representation_type_spec;
    }
  else
    {
      representation_type = ensure_global_dynid ((void *)buffer_session_open);
      if (representation_type <= 0)
        return -1;
    }

  if (0 > init_buffer_set (&it->set, limits, representation_type))
    return -1;

  return it_desc;
}


int
buffer_session_close (alloc_limits const limits,
                      ssize_t const id)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (id);
  if (!it)
    return -1;

  if (0 > uninit_buffer_set (&it->set))
    return -1;

  buffer_set_free_id (id);

  return 0;
}




alloc_limits
buffer_session_limits (ssize_t const session)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_limits (&it->set);
}


ssize_t
buffer_session_representation_type (ssize_t const session)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_representation_type (&it->set);
}



ssize_t
buffer_session_n_entries (ssize_t const session)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_n_entries (&it->set);
}


ssize_t
buffer_session_room (ssize_t const session)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_room (&it->set);
}


t_buffer *
buffer_session_buffer_n (const t_uchar ** const name_ret,
                         ssize_t const session,
                         ssize_t const n)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_buffer_n (name_ret, &it->set, n);
}


t_buffer *
buffer_session_nth_buffer (const t_uchar ** const name_ret,
                           ssize_t const session,
                           ssize_t const n)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_nth_buffer (name_ret, &it->set, n);
}


t_buffer *
buffer_session_find (ssize_t const session,
                     t_uchar * const name)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_find (&it->set, name);
}


t_buffer *
buffer_session_ensure (ssize_t const session,
                       t_uchar * const name)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_ensure (&it->set, name);
}


ssize_t
buffer_session_index (ssize_t const session,
                      t_uchar * const name)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return -1;

  return buffer_set_index (&it->set, name);
}


ssize_t
buffer_session_index_create (ssize_t const session,
                             t_uchar * const name)
{
  ssize_t answer;

  answer = buffer_session_index (session, name);
  if (answer < 0)
    {
      (void)buffer_session_ensure (session, name);
      answer = buffer_session_index (session, name);
    }

  return answer;
}


int
buffer_session_delete (ssize_t const session,
                       t_uchar * const name)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_delete (&it->set, name);
}


int
buffer_session_delete_nth (ssize_t const session,
                           ssize_t const n)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_delete_nth (&it->set, n);
}



void * 
buffer_session_property_soft_ref (ssize_t const session,
                                  ssize_t const elt_size,
                                  ssize_t const buffer,
                                  ssize_t const global_property_id)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_property_soft_ref (&it->set, elt_size, buffer, global_property_id);
}


void * 
buffer_session_property_ref (ssize_t const session,
                             ssize_t const elt_size,
                             ssize_t const buffer,
                             ssize_t const global_property_id,
                             t_property_type * const type)
{
  struct buffer_set_entry * it;

  it = buffer_set_ref_id (session);
  if (!it)
    return 0;

  return buffer_set_property_ref (&it->set, elt_size, buffer, global_property_id, type);
}




/* tag: Tom Lord Sun Nov 21 08:02:31 2004 (buffers.c)
 */
