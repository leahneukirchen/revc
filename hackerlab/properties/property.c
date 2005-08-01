/* property.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/arrays/ar.h"
#include "hackerlab/dynid/dynid.h"
#include "hackerlab/properties/property.h"


/* __STDC__ prototypes for static functions */
static t_dynid_table * property_global_id_table (void);
static t_dynid_table * property_representation_id_table (void);



ssize_t
property_representation_id (void * key)
{
  return dynid_ensure (property_representation_id_table (), lim_use_malloc, key);
}


ssize_t
property_global_id (void * key)
{
  return dynid_ensure (property_global_id_table (), lim_use_malloc, key);
}


ssize_t
property_id (ssize_t representation_id, ssize_t global_id)
{
  static ssize_t ** table = 0;
  ssize_t ** row_box;
  ssize_t * col_box;

  row_box = ar_ref ((void **)&table, lim_use_malloc, representation_id, sizeof (ssize_t *));
  if (!row_box)
    return -1;

  col_box = ar_ref ((void **)row_box, lim_use_malloc, global_id, sizeof (ssize_t));
  if (!col_box)
    return -1;

  if (!*col_box)
    {
      static ssize_t * row_count = 0;
      ssize_t * row_count_box;

      row_count_box = (ssize_t *)ar_ref ((void **)&row_count, lim_use_malloc, representation_id, sizeof (ssize_t));
      if (!row_count_box)
        return -1;

      *row_count_box += 1;
      *col_box = *row_count_box;
    }

  return *col_box;
}



static t_dynid_table *
property_global_id_table (void)
{
  static int initialized = 0;
  static t_dynid_table table;

  if (!initialized)
    {
      if (0 > init_dynid_table (&table, lim_use_malloc))
        return 0;
      initialized = 1;
    }

  return &table;
}

static t_dynid_table *
property_representation_id_table (void)
{
  static int initialized = 0;
  static t_dynid_table table;

  if (!initialized)
    {
      if (0 > init_dynid_table (&table, lim_use_malloc))
        return 0;
      initialized = 1;
    }

  return &table;
}



int
init_property_table (t_property_table * table,
                     alloc_limits limits)
{
  if (!table)
    return -1;

  mem_set0 ((t_uchar *)table, sizeof (*table));

  return 0;
}


int
uninit_property_table (t_property_table * table,
                       alloc_limits limits)
{
  ssize_t n_rows;
  ssize_t x;

  if (!table)
    return -1;

  n_rows = ar_size ((void *)*table, limits, sizeof (t_property_row));

  for (x = 0; x < n_rows; ++x)
    {
      if ((*table)[x]._type && (*table)[x]._type->fns && (*table)[x]._type->fns->uninit)
        {
          ssize_t n_cols;
          ssize_t y;

          n_cols = ar_size ((void *)(*table)[x]._row, limits, (*table)[x]._type->fns->elt_size);
          for (y = 0; y < n_cols; ++y)
            {
              (*table)[x]._type->fns->uninit ((*table)[x]._type, limits, y, (void *)((t_uchar *)(*table)[x]._row + y * (*table)[x]._type->fns->elt_size));
            }
        }
      ar_free ((void **)&(*table)[x]._row, limits);
    }

  ar_free ((void **)table, limits);

  mem_set0 ((t_uchar *)table, sizeof (*table));

  return 0;
}



void *
property_soft_ref (t_property_table * table,
                   alloc_limits limits,
                   ssize_t elt_size,
                   ssize_t representation_id,
                   ssize_t global_id,
                   ssize_t n)
{
  ssize_t id;
  t_property_row * row;

  id = property_id (representation_id, global_id);
  if (id < 0)
    return 0;

  if (id >= ar_size ((void *)*table, limits, sizeof (t_property_row)))
    return 0;

  row = &(*table)[id];

  if (n < 0)
    return 0;

  if (n >= ar_size ((void *)row->_row, limits, elt_size))
    return 0;

  return (void *)((t_uchar *)row->_row + n * elt_size);
}


void *
property_ref (t_property_table * table,
              alloc_limits limits,
              ssize_t elt_size,
              ssize_t representation_id,
              ssize_t global_id,
              ssize_t n,
              t_property_type * type)
{
  ssize_t id;
  t_property_row * row;
  ssize_t was_size;
  void * answer;

  id = property_id (representation_id, global_id);
  if (id < 0)
    return 0;

  row = (t_property_row *)ar_ref ((void **)table, lim_use_malloc, id, sizeof (t_property_row));

  if (!row)
    return 0;

  if (type && !row->_type)
    row->_type = type;

  if (n < 0)
    return 0;

  was_size = ar_size ((void *)row->_row, lim_use_malloc, elt_size);
  answer = ar_ref ((void **)&row->_row, lim_use_malloc, n, elt_size);

  if (!answer)
    return 0;

  if (row->_type && row->_type->fns && row->_type->fns->init)
    {
      ssize_t y;

      for (y = was_size; y <= n; ++y)
        {
          row->_type->fns->init (row->_type, limits, y, row->_row + y * row->_type->fns->elt_size);
        }
    }

  return answer;
}



/* tag: Tom Lord Wed Oct 27 09:29:18 2004 (property.c)
 */
