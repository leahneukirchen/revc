/* symbol-table.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "hackerlab/identifiers/symbol-table.h"


/* __STDC__ prototypes for static functions */
static int find_cmp (void * vk, void * ve, void * vcl);



#define SIZEOF_ELT (elt_size - 1 + sizeof (t_symbol_table_entry))

int
init_symbol_table (t_symbol_table * const table,
                   alloc_limits const limits,
                   ssize_t const elt_size)
{
  if (!table)
    return -1;

  mem_set0 ((t_uchar *)table, sizeof (*table));

  if (0 > init_obtable (&table->_kv, limits, SIZEOF_ELT, 0, 0))
    return -1;

  if (0 > init_oblist (&table->_order, limits, sizeof(ssize_t), 0, 0))
    return -1;

  return 0;
}


int
uninit_symbol_table (t_symbol_table * const table,
                     alloc_limits const limits,
                     ssize_t const elt_size)
{
  ssize_t room;
  ssize_t x;

  if (!table)
    return -1;

  room = obtable_room (&table->_kv, limits, SIZEOF_ELT, 0, 0);

  for (x = 0; x < room; ++x)
    {
      if (!obtable_is_free (&table->_kv, limits, SIZEOF_ELT, 0, 0, x))
        {
          t_symbol_table_entry * e;
          e = (t_symbol_table_entry *)obtable_ref (&table->_kv, limits, SIZEOF_ELT, 0, 0, x);
          if (!e)
            return -1;
          lim_free (limits, e->_key);
          e->_key = 0;
        }
    }

  uninit_obtable (&table->_kv, limits, SIZEOF_ELT, 0, 0);
  uninit_oblist (&table->_order, limits, sizeof(ssize_t), 0, 0);

  mem_set0 ((t_uchar *)table, sizeof (*table));

  return 0;
}



ssize_t
symbol_table_n_entries (t_symbol_table * const table,
                        alloc_limits const limits,
                        ssize_t const elt_size)
{
  if (!table)
    return 0;

  return obtable_n_elts (&table->_kv, limits, SIZEOF_ELT, 0, 0);
}


ssize_t
symbol_table_room (t_symbol_table * const table,
                   alloc_limits const limits,
                   ssize_t const elt_size)
{
  if (!table)
    return 0;

  return obtable_room (&table->_kv, limits, SIZEOF_ELT, 0, 0);
}


void *
symbol_table_entry_n (const t_uchar ** const key_return,
                      t_symbol_table * const table,
                      alloc_limits const limits,
                      ssize_t const elt_size,
                      ssize_t const n)
{
  t_symbol_table_entry * e;

  if (!table)
    return 0;

  e = (t_symbol_table_entry *)obtable_ref (&table->_kv, limits, SIZEOF_ELT, 0, 0, n);

  if (!e)
    return 0;

  if (key_return)
    {
      *key_return = e->_key;
    }

  return (void *)e->_value;
}


void *
symbol_table_nth_entry (const t_uchar ** const key_return,
                        t_symbol_table * const table,
                        alloc_limits const limits,
                        ssize_t const elt_size,
                        ssize_t const n)
{
  ssize_t * k_box;
  ssize_t k;
  t_symbol_table_entry * e;

  if (!table)
    return 0;

  k_box = (ssize_t *)oblist_burst (0, &table->_order, limits, sizeof (ssize_t), 0, 0, n, 1);
  if (!k_box)
    return 0;

  k = *k_box;

  e = (t_symbol_table_entry *)obtable_ref (&table->_kv, limits, SIZEOF_ELT, 0, 0, k);

  if (!e)
    return 0;

  if (key_return)
    {
      *key_return = e->_key;
    }

  return (void *)e->_value;
}


struct find_cmp_closure
{
  t_symbol_table * table;
  alloc_limits limits;
  ssize_t elt_size;
};


static int
find_cmp (void * const vk, void * const ve, void * const vcl)
{
  t_uchar * key;
  ssize_t * entry;
  struct find_cmp_closure * cl;
  const t_uchar * entry_key;

  key = (t_uchar *)vk;
  entry = (ssize_t *)ve;
  cl = (struct find_cmp_closure *)vcl;

  if (!key && !entry)
    return 0;

  if (!key)
    return -1;

  if (!entry)
    return 1;

  if (!symbol_table_entry_n (&entry_key, cl->table, cl->limits, cl->elt_size, *entry))
    {
      return 1;
    }

  return str_cmp (key, entry_key);
}



ssize_t
symbol_table_entry_index (t_symbol_table * const table,
                          alloc_limits const limits,
                          ssize_t const elt_size,
                          const t_uchar * const key)
{
  struct find_cmp_closure cl;
  ssize_t order;
  ssize_t * kv_pos_box;
  ssize_t kv_pos;

  if (!table || !key)
    return -1;

  cl.table = table;
  cl.limits = limits;
  cl.elt_size = elt_size;

  order = oblist_find (0, &table->_order, limits, sizeof (ssize_t), 0, 0, (void *)key, find_cmp, (void *)&cl);
  if (order < 0)
    return -1;

  kv_pos_box = (ssize_t *)oblist_burst (0, &table->_order, limits, sizeof (ssize_t), 0, 0, order, 1);
  if (!kv_pos_box)
    return -1;

  kv_pos = *kv_pos_box;

  return kv_pos;
}


void *
symbol_table_entry_find (t_symbol_table * const table,
                         alloc_limits const limits,
                         ssize_t const elt_size,
                         const t_uchar * const key)
{
  struct find_cmp_closure cl;
  ssize_t order;
  ssize_t * kv_pos_box;
  ssize_t kv_pos;
  t_symbol_table_entry * e;

  if (!table || !key)
    return 0;

  cl.table = table;
  cl.limits = limits;
  cl.elt_size = elt_size;

  order = oblist_find (0, &table->_order, limits, sizeof (ssize_t), 0, 0, (void *)key, find_cmp, (void *)&cl);
  if (order < 0)
    return 0;

  kv_pos_box = (ssize_t *)oblist_burst (0, &table->_order, limits, sizeof (ssize_t), 0, 0, order, 1);
  if (!kv_pos_box)
    return 0;

  kv_pos = *kv_pos_box;

  e = (t_symbol_table_entry *)obtable_ref (&table->_kv, limits, SIZEOF_ELT, 0, 0, kv_pos);
  if (!e)
    return 0;

  return (void *)e->_value;
}


void *
symbol_table_entry_ensure (t_symbol_table * const table,
                           alloc_limits const limits,
                           ssize_t const elt_size,
                           const t_uchar * const key)
{
  struct find_cmp_closure cl;
  ssize_t order;
  ssize_t order_should_be;
  ssize_t kv_pos;
  t_symbol_table_entry * e;
  ssize_t * kv_pos_box;
  
  if (!table || !key)
    return 0;

  cl.table = table;
  cl.limits = limits;
  cl.elt_size = elt_size;

  order = oblist_find (&order_should_be, &table->_order, limits, sizeof (ssize_t), 0, 0, (void *)key, find_cmp, &cl);
  if (order >= 0)
    {
      kv_pos_box = (ssize_t *)oblist_burst (0, &table->_order, limits, sizeof (ssize_t), 0, 0, order, 1);
      if (!kv_pos_box)
        return 0;

      kv_pos = *kv_pos_box;

      e = (t_symbol_table_entry *)obtable_ref (&table->_kv, limits, SIZEOF_ELT, 0, 0, kv_pos);
      if (!e)
        return 0;

      return (void *)e->_value;
    }
  else
    {
      t_uchar * saved_key;

      saved_key = str_save (limits, key);
      if (!saved_key)
        return 0;

      kv_pos = obtable_alloc (&table->_kv, limits, SIZEOF_ELT, 0, 0);
      e = (t_symbol_table_entry *)obtable_ref (&table->_kv, limits, SIZEOF_ELT, 0, 0, kv_pos);
      if (!e)
        {
          lim_free (limits, saved_key);
          return 0;
        }

      e->_key = saved_key;

      if (0 > oblist_insert_n (&table->_order, limits, sizeof (ssize_t), 0, 0, order_should_be, 1, (void *)&kv_pos))
        {
          obtable_free (&table->_kv, limits, SIZEOF_ELT, 0, 0, kv_pos);
          lim_free (limits, saved_key);
          return 0;
        }

      return (void *)e->_value;
    }
}




int
symbol_table_entry_delete (t_symbol_table * const table,
                           alloc_limits const limits,
                           ssize_t const elt_size,
                           const t_uchar * const key)
{
  ssize_t order;
  ssize_t * kv_pos_box;
  ssize_t kv_pos;
  
  if (!table || !key)
    return 0;
  
  order = oblist_find (00, &table->_order, limits, sizeof (ssize_t), 0, 0, (void *)key, find_cmp, 0);
  if (order < 0)
    return -1;
  
  kv_pos_box = (ssize_t *)oblist_burst (0, &table->_order, limits, sizeof (ssize_t), 0, 0, order, 1);
  if (!kv_pos_box)
    return -1;
  
  kv_pos = *kv_pos_box;
  
  obtable_free (&table->_kv, limits, SIZEOF_ELT, 0, 0, kv_pos);

  if (0 > oblist_delete_n (&table->_order, limits, sizeof (ssize_t), 0, 0, order, 1))
    return -1;
  
  return 0;
}



/* tag: Tom Lord Wed Oct 27 11:39:44 2004 (symbol-table.c)
 */
