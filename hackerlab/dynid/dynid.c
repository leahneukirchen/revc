/* dynid.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/dynid/dynid.h"


/* typedefs */


/* __STDC__ prototypes for static functions */
static int id_key_cmp (void * vkey, void * velt, void * ign);
static t_dynid_table * global_table (void);


/* declarations */


/* code */

int
init_dynid_table (t_dynid_table * dt, alloc_limits limits)
{
  if (!dt)
    return -1;

  mem_set0 ((t_uchar *)dt, sizeof (*dt));

  if (0 > init_oblist (&dt->id_table, limits, sizeof (t_dynid_record), 0, 0))
    return -1;

  return 0;
}


void
uninit_dynid_table (t_dynid_table * dt, alloc_limits limits)
{
  if (!dt)
    return;

  uninit_oblist (&dt->id_table, limits, sizeof (t_dynid_record), 0, 0);

  mem_set0 ((t_uchar *)dt, sizeof (*dt));
}



ssize_t
dynid_find (t_dynid_table * dt, alloc_limits limits, void * key)
{
  ssize_t rec;

  rec = oblist_find (0, &dt->id_table, limits, sizeof (t_dynid_record), 0, 0, key, id_key_cmp, 0);

  if (rec < 0)
    return -1;

  return ((t_dynid_record *)oblist_burst (0, &dt->id_table, limits, sizeof (t_dynid_record), 0, 0, rec, 1))->id;
}


ssize_t
dynid_ensure (t_dynid_table * dt, alloc_limits limits, void * key)
{
  ssize_t rec;
  ssize_t rec_would_be;

  rec = oblist_find (&rec_would_be, &dt->id_table, limits, sizeof (t_dynid_record), 0, 0, key, id_key_cmp, 0);

  if (0 <= rec)
    {
      return ((t_dynid_record *)oblist_burst (0, &dt->id_table, limits, sizeof (t_dynid_record), 0, 0, rec, 1))->id;
    }
  else
    {
      ssize_t answer;
      t_dynid_record * r;

      answer = 1 + oblist_size (&dt->id_table, limits, sizeof (t_dynid_record), 0, 0);
      if (0 > oblist_insert_n (&dt->id_table, limits, sizeof (t_dynid_record), 0, 0, rec_would_be, 1, 0))
        return -1;
      r = (t_dynid_record *)oblist_burst (0, &dt->id_table, limits, sizeof (t_dynid_record), 0, 0, rec_would_be, 1);
      if (!r)
        return -1;
      r->key = key;
      r->id = answer;
      return answer;
    }

}



static int
id_key_cmp (void * vkey, void * velt, void * ign)
{
  t_dynid_record * elt;

  elt = (t_dynid_record *)velt;

  if (vkey < elt->key)
    return -1;
  else if (vkey > elt->key)
    return 1;
  else
    return 0;
}




static t_dynid_table *
global_table (void)
{
  static int initialized = 0;
  static t_dynid_table table;

  if (!initialized)
    {
      if (0 > init_dynid_table (&table, lim_use_malloc))
        return 0;
    }

  return &table;
}

ssize_t
find_global_dynid (void * key)
{
  return dynid_find (global_table (), lim_use_malloc, key);
}

ssize_t
ensure_global_dynid (void * key)
{
  return dynid_ensure (global_table (), lim_use_malloc, key);
}



/* tag: Tom Lord Sun Oct 24 21:49:19 2004 (dynid.c)
 */
