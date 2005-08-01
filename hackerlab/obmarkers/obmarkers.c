/* obmarkers.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/obmarkers/obmarkers.h"
#include "hackerlab/handles/handle.h"


/* __STDC__ prototypes for static functions */
static int cmp (void * va, void * vb, void * vcl);
static t_obmarker * obmarker__n (t_obmarker_table * table,
                                 alloc_limits limits,
                                 ssize_t m);
static int gap_cmp (void * va, void * vb, void * vcl);
static ssize_t find_gap_order (t_obmarker_table * table,
                               alloc_limits limits,
                               ssize_t pos);
static int move_gap (t_obmarker_table * table,
                     alloc_limits limits,
                     ssize_t pos);



int
init_obmarker_table (t_obmarker_table * table,
                     alloc_limits limits)
{
  if (!table)
    return -1;

  mem_set0 ((t_uchar *)table, sizeof (*table));

  if (0 > init_obtable (&table->_markers, limits, sizeof (t_obmarker), 0, 0))
    return -1;

  if (0 > init_oblist (&table->_in_order, limits, sizeof (ssize_t), 0, 0))
    {
      uninit_obtable (&table->_markers, limits, sizeof (t_obmarker), 0, 0);
      return -1;
    }

  table->_gap = 0;
  table->_size = 0;

  return 0;
}


int
uninit_obmarker_table (t_obmarker_table * table,
                       alloc_limits limits)
{
  ssize_t bound;
  ssize_t x;

  if (!table)
    return -1;

  bound = obtable_room (&table->_markers, limits, sizeof (t_obmarker), 0, 0);
  for (x = 0; x < bound; ++x)
    {
      t_obmarker * m;

      m = (t_obmarker *)obtable_ref (&table->_markers, limits, sizeof (t_obmarker), 0, 0, x);
      if (   m
          && (0 <= m->_handle_set)
          && (0 > free_handle_set (m->_handle_set)))
        return -1;
    }
  
  uninit_oblist (&table->_in_order, limits, sizeof (ssize_t), 0, 0);
  uninit_obtable (&table->_markers, limits, sizeof (t_obmarker), 0, 0);
  mem_set0 ((t_uchar *)table, sizeof (*table));
  
  return 0;
}



struct cmp_closure
{
  t_obmarker_table * table;
  alloc_limits limits;
};



static int
cmp (void * va, void * vb, void * vcl)
{
  ssize_t * a;
  ssize_t * b;
  struct cmp_closure * cl;
  ssize_t a_id;
  ssize_t b_id;
  ssize_t a_pos;
  ssize_t b_pos;

  a = (ssize_t *)va;
  b = (ssize_t *)vb;
  cl = (struct cmp_closure *)vcl;

  a_id = *a;
  b_id = *b;

  a_pos = obmarker_pos (cl->table, cl->limits, a_id);
  b_pos = obmarker_pos (cl->table, cl->limits, b_id);

  if (a_pos < b_pos)
    return -1;
  else if (a_pos > b_pos)
    return 1;
  else if (a_id < b_id)
    return -1;
  else if (a_id > b_id)
    return 1;
  else
    return 0;
}


ssize_t
obmarker_alloc (t_obmarker_table * table,
                alloc_limits limits,
                ssize_t init_pos)
{
  ssize_t answer;
  t_obmarker * it;
  ssize_t order;

  if (!table)
    return -1;

  if ((init_pos < 0) || (init_pos > table->_size))
    return -1;

  answer = obtable_alloc (&table->_markers, limits, sizeof (t_obmarker), 0, 0);
  if (answer < 0)
    return -1;

  it = (t_obmarker *)obtable_ref (&table->_markers, limits, sizeof (t_obmarker), 0, 0, answer);
  if (!it)
    {
      obtable_free  (&table->_markers, limits, sizeof (t_obmarker), 0, 0, answer);
      return -1;
    }

  it->_handle_set = -1;
  if (init_pos < table->_gap)
    {
      it->_pos = init_pos;
    }
  else
    {
      it->_pos = init_pos - table->_size - 1;
    }

  {
    struct cmp_closure cl;
    cl.table = table;
    cl.limits = limits;
    (void)oblist_find (&order, &table->_in_order, limits, sizeof (ssize_t), 0, 0, (void *)&answer, cmp, (void *)&cl);
  }

  if (0 > oblist_insert_n (&table->_in_order, limits, sizeof (ssize_t), 0, 0, order, 1, &answer))
    {
      obtable_free  (&table->_markers, limits, sizeof (t_obmarker), 0, 0, answer);
      return -1;
    }

  return answer;
}


int
obmarker_free (t_obmarker_table * table,
               alloc_limits limits,
               ssize_t m)
{
  t_obmarker * it;
  ssize_t order;

  if (!table)
    return -1;

  it = (t_obmarker *)obtable_ref (&table->_markers, limits, sizeof (t_obmarker), 0, 0, m);
  if (!it)
    return -1;

  if (   (0 <= it->_handle_set)
      && (0 > free_handle_set (it->_handle_set)))
    return -1;

  it->_handle_set = -1;

  {
    struct cmp_closure cl;
    cl.table = table;
    cl.limits = limits;
    order = oblist_find (0, &table->_in_order, limits, sizeof (ssize_t), 0, 0, (void *)&m, cmp, (void *)&cl);
  }

  obtable_free  (&table->_markers, limits, sizeof (t_obmarker), 0, 0, m);

  if (0 > oblist_delete_n (&table->_in_order, limits, sizeof (ssize_t), 0, 0, order, 1))
    return -1;

  return 0;
}



static t_obmarker *
obmarker__n (t_obmarker_table * table,
             alloc_limits limits,
             ssize_t m)
{
  if (!table)
    return 0;

  return (t_obmarker *)obtable_ref (&table->_markers, limits, sizeof (t_obmarker), 0, 0, m);
}

ssize_t
obmarker_n_markers (t_obmarker_table * table,
                    alloc_limits limits)
{
  if (!table)
    return -1;
  return obtable_n_elts (&table->_markers, limits, sizeof (t_obmarker), 0, 0);
}


ssize_t
obmarker_room (t_obmarker_table * table,
               alloc_limits limits)
{
  if (!table)
    return -1;
  return obtable_room (&table->_markers, limits, sizeof (t_obmarker), 0, 0);
}


ssize_t
obmarker_pos (t_obmarker_table * table,
              alloc_limits limits,
              ssize_t m)
{
  t_obmarker * mark;

  mark = obmarker__n (table, limits, m);
  if (!mark)
    return -1;

  if (mark->_pos >= 0)
    {
      return mark->_pos;
    }
  else
    {
      return mark->_pos + 1 + table->_size;
    }
}



int
obmarker_set_pos (t_obmarker_table * table,
                  alloc_limits limits,
                  ssize_t m,
                  ssize_t to)
{
  t_obmarker * mark;
  ssize_t order;

  if (!table)
    return -1;

  if ((to < 0) || (to > table->_size))
    return -1;

  {
    struct cmp_closure cl;
    cl.table = table;
    cl.limits = limits;
    order = oblist_find (0, &table->_in_order, limits, sizeof (ssize_t), 0, 0, (void *)&m, cmp, (void *)&cl);
    if (order < 0)
      return -1;
  }

  if (0 > oblist_delete_n (&table->_in_order, limits, sizeof (ssize_t), 0, 0, order, 1))
    return -1;
  
  mark = obmarker__n (table, limits, m);
  if (!mark)
    {
      (void)oblist_insert_n (&table->_in_order, limits, sizeof (ssize_t), 0, 0, order, 1, (void *)&m);
      return -1;
    }

  if (to < table->_gap)
    {
      mark->_pos = to;
    }
  else
    {
      mark->_pos = (to - table->_size) - 1;
    }

  {
    struct cmp_closure cl;
    cl.table = table;
    cl.limits = limits;
    (void)oblist_find (&order, &table->_in_order, limits, sizeof (ssize_t), 0, 0, (void *)&m, cmp, (void *)&cl);
  }

  return oblist_insert_n (&table->_in_order, limits, sizeof (ssize_t), 0, 0, order, 1, (void *)&m);
}



static ssize_t
obmarker__n_markers (t_obmarker_table * table,
                    alloc_limits limits)
{
  if (!table)
    return 0;

  return obtable_n_elts (&table->_markers, limits, sizeof (t_obmarker), 0, 0);
}


int
obmarker_insert_n (t_obmarker_table * table,
                   alloc_limits limits,
                   ssize_t pos,
                   ssize_t n)
{
  if (0 > move_gap (table, limits, pos))
    return -1;

  table->_size += n;

  return 0;
}

static int
gap_cmp (void * va, void * vb, void * vcl)
{
  ssize_t * b;
  struct cmp_closure * cl;
  ssize_t b_id;
  ssize_t a_pos;
  ssize_t b_pos;

  a_pos = (ssize_t)va;
  b = (ssize_t *)vb;
  cl = (struct cmp_closure *)vcl;

  b_id = *b;
  b_pos = obmarker_pos (cl->table, cl->limits, b_id);

  if (a_pos < b_pos)
    return -1;
  else if (a_pos > b_pos)
    return 1;
  else
    return 0;
}

static ssize_t
find_gap_order (t_obmarker_table * table,
                alloc_limits limits,
                ssize_t pos)
{
  ssize_t at;
  ssize_t would;
  ssize_t answer;
  struct cmp_closure cl;

  cl.table = table;
  cl.limits = limits;
  at = oblist_find  (&would, &table->_in_order, limits, sizeof (ssize_t), 0, 0, (void *)&pos, gap_cmp, (void *)&cl);

  if (at >= 0)
    answer = at;
  else
    answer = would;

  return answer;
}

static int
move_gap (t_obmarker_table * table,
          alloc_limits limits,
          ssize_t pos)
{
  ssize_t current_gap_order;
  ssize_t desired_gap_order;

  if (!table || (pos < 0) || (pos > table->_size))
    return -1;

  current_gap_order = find_gap_order (table, limits, table->_gap);
  desired_gap_order = find_gap_order (table, limits, pos);

  if (current_gap_order < desired_gap_order)
    {
      ssize_t x;

      for (x = current_gap_order; x < desired_gap_order; ++x)
        {
          ssize_t * m_id_box;
          ssize_t m_id;
          t_obmarker * mark;

          m_id_box = (ssize_t *)oblist_burst (0, &table->_in_order, limits, sizeof (ssize_t), 0, 0, x, 1);
          if (!m_id_box)
            return -1;

          m_id = *m_id_box;

          mark = obmarker__n (table, limits, m_id);
          if (!mark)
            return -1;

          mark->_pos = ((mark->_pos + 1) + table->_size);
        }
    }
  else
    {
      ssize_t x;

      for (x = current_gap_order - 1; x >= desired_gap_order; --x)
        {
          ssize_t * m_id_box;
          ssize_t m_id;
          t_obmarker * mark;

          m_id_box = (ssize_t *)oblist_burst (0, &table->_in_order, limits, sizeof (ssize_t), 0, 0, x, 1);
          if (!m_id_box)
            return -1;

          m_id = *m_id_box;

          mark = obmarker__n (table, limits, m_id);
          if (!mark)
            return -1;

          mark->_pos = (mark->_pos - table->_size - 1);
        }
    }

  table->_gap = pos;
  return 0;
}



int
obmarker_delete_n (t_obmarker_table * table,
                   alloc_limits limits,
                   ssize_t pos,
                   ssize_t n)
{
  ssize_t n_markers;
  ssize_t gap_order;
  ssize_t x;
  ssize_t new_size;

  if (!table)
    return -1;

  if ((pos < 0) || (pos > table->_size))
    return -1;

  if ((n < 0) || ((pos + n) < 0) || ((pos + n) > table->_size))
    return -1;

  new_size = table->_size - n;

  if (0 > move_gap (table, limits, pos))
    return -1;

  n_markers = obmarker__n_markers (table, limits);
  gap_order = find_gap_order (table, limits, pos);

  for (x = gap_order; x < n_markers; ++x)
    {
      ssize_t * m_id_box;
      ssize_t m_id;
      t_obmarker * mark;
      ssize_t old_mark_pos;

      m_id_box = (ssize_t *)oblist_burst (0, &table->_in_order, limits, sizeof (ssize_t), 0, 0, x, 1);
      if (!m_id_box)
        return -1;
      
      m_id = *m_id_box;
      
      mark = obmarker__n (table, limits, m_id);
      if (!mark)
        return -1;

      old_mark_pos = table->_size + mark->_pos + 1;

      if (old_mark_pos >= (pos + n))
        break;
      else
        mark->_pos = (pos + n) - table->_size - 1;
    }

  table->_size = new_size;

  return 0;
}



ssize_t
obmarker_handle_set (t_obmarker_table * table,
                     alloc_limits limits,
                     ssize_t id)
{
  t_obmarker * mark;

  mark = obmarker__n (table, limits, id);
  if (!mark)
    return -1;

  if (mark->_handle_set < 0)
    {
      mark->_handle_set = alloc_handle_set ();
    }

  return mark->_handle_set;
}


/* tag: Tom Lord Sat Oct 30 13:58:52 2004 (obmarkers.c)
 */
