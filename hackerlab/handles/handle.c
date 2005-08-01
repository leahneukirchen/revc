/* handle.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/obtables/obtable.h"
#include "hackerlab/oblists/ssizet.h"
#include "hackerlab/handles/handle.h"


/* typedefs */

typedef struct handle_data t_handle_data;

struct handle_data
{
  ssize_t set_id;
  ssize_t resource;
};

OBTABLE_STATIC_TABLE(handle, t_handle_data);

typedef struct handle_set_data t_handle_set_data;

struct handle_set_data
{
  t_list_ssize_t handles;
};

OBTABLE_STATIC_TABLE(handle_set, t_handle_set_data);



/* __STDC__ prototypes for static functions */


/* declarations */




/* code */

ssize_t
alloc_handle (void)
{
  ssize_t answer;
  t_handle_data * data;

  answer = handle_alloc_id ();
  if (answer < 0)
    return answer;

  data = handle_ref_id (answer);
  if (!data)
    return -1;

  data->set_id = -1;
  data->resource = -1;

  return answer;
}


int
free_handle (ssize_t h)
{
  if (0 > handle_set (h, -1, -1))
    return -1;

  handle_free_id (h);

  return 0;
}


ssize_t
handle_ref (ssize_t h)
{
  t_handle_data * data;

  data = handle_ref_id (h);
  if (!data)
    return -1;

  return data->resource;
}


int
handle_set (ssize_t h, ssize_t s, ssize_t r)
{
  t_handle_data * h_data;

  h_data = handle_ref_id (h);
  if (!h_data)
    return -1;

  if ((h_data->set_id == s) && (h_data->resource == r))
    return 0;

  if (h_data->set_id >= 0)
    {
      t_handle_set_data * prev_s_data;

      prev_s_data = handle_set_ref_id (h_data->set_id);
      if (!prev_s_data)
        return -1;

      if (0 > list_ssize_t_remove (lim_use_malloc, &prev_s_data->handles, h))
        return -1;

      h_data->set_id = -1;
      h_data->resource = -1;
    }

  if (s >= 0)
    {
      t_handle_set_data * s_data;
      
      s_data = handle_set_ref_id (s);
      if (!s_data)
        return -1;

      if (0 > list_ssize_t_bag_add (lim_use_malloc, &s_data->handles, h))
        return -1;

      h_data->set_id = s;
      h_data->resource = r;
    }

  return 0;
}



ssize_t
alloc_handle_set (void)
{
  ssize_t answer;
  t_handle_set_data * data;

  answer = handle_set_alloc_id ();
  if (answer < 0)
    return answer;

  data = handle_set_ref_id (answer);
  if (!data)
    return -1;

  if (0 > init_list_ssize_t (&data->handles, lim_use_malloc))
    {
      handle_set_free_id (answer);
      return -1;
    }

  return answer;
}

int
free_handle_set (ssize_t s)
{
  t_handle_set_data * data;

  data = handle_set_ref_id (s);
  if (!data)
    return -1;

  while (!list_ssize_t_is_empty (&data->handles, lim_use_malloc))
    {
      ssize_t h;

      h = list_ssize_t_pop (lim_use_malloc, &data->handles);
      if (h < 0)
        return -1;

      if (0 > handle_set (h, -1, -1))
        return -1;
    }

  uninit_list_ssize_t (&data->handles, lim_use_malloc);

  handle_set_free_id (s);
  return 0;
}



/* tag: Tom Lord Sat Nov 20 15:52:28 2004 (handle.c)
 */
