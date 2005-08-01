/* oblist.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 * 
 */


#include "hackerlab/sort/qsort.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/oblists/oblist.h"


struct ol_closure;


/* __STDC__ prototypes for static functions */
static int oblist_move_gap (t_oblist * ol,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_oblist_type * type,
                            void * closure,
                            ssize_t wanted_gap);
static void init_ol_closure (struct ol_closure * olc,
                             t_oblist * ol,
                             alloc_limits limits,
                             ssize_t elt_size,
                             t_oblist_type * type,
                             void * closure);
static t_obqueue_type * translate_type (struct oblist_type * type);
static int forward_init (t_obqueue * oq,
                         alloc_limits limits,
                         ssize_t elt_size,
                         t_obqueue_type * type,
                         void * closure,
                         void * mem,
                         ssize_t n_elts);
static void forward_uninit (t_obqueue * oq,
                            alloc_limits limits,
                            ssize_t elt_size,
                            t_obqueue_type * type,
                            void * closure,
                            void * mem,
                            ssize_t n_elts);



#define LEFT(OL) (&(OL)->_left)
#define RIGHT(OL) (&(OL)->_right)

struct obqueue_type init_and_uninit = { forward_init, forward_uninit };
struct obqueue_type init_only = { forward_init, 0 };
struct obqueue_type uninit_only = { 0, forward_uninit };

struct ol_closure
{
  t_oblist * ol;
  t_oblist_type * type;
  void * closure;
};




int
init_oblist (t_oblist * ol,
             alloc_limits limits,
             ssize_t elt_size,
             t_oblist_type * type,
             void * closure)
{
  if (!ol)
    return -1;
  else
    {
      struct ol_closure olc;
      
      init_ol_closure (&olc, ol, limits, elt_size, type, closure);
      
      mem_set0 ((t_uchar *)ol, sizeof (*ol));
      if (0 > init_obqueue (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc))
        return -1;
      if (0 > init_obqueue (RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc))
        return -1;
      return 0;
    }
}


void
uninit_oblist (t_oblist * ol,
               alloc_limits limits,
               ssize_t elt_size,
               t_oblist_type * type,
               void * closure)
{
  if (ol)
    {
      struct ol_closure olc;
      
      init_ol_closure (&olc, ol, limits, elt_size, type, closure);
      
      uninit_obqueue (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc);
      uninit_obqueue (RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc);
      mem_set0 ((t_uchar *)ol, sizeof (*ol));
    }
}




int
oblist_is_empty (t_oblist * ol,
                 alloc_limits limits,
                 ssize_t elt_size,
                 t_oblist_type * type,
                 void * closure)
{
  if (!ol)
    return 1;

  return !oblist_size (ol, limits, elt_size, type, closure);
}


ssize_t
oblist_size (t_oblist * ol,
             alloc_limits limits,
             ssize_t elt_size,
             t_oblist_type * type,
             void * closure)
{
  struct ol_closure olc;

  if (!ol)
    return 0;

  init_ol_closure (&olc, ol, limits, elt_size, type, closure);

  return (  obqueue_size (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc)
          + obqueue_size (RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc));
}


ssize_t
oblist_room (t_oblist * ol,
             alloc_limits limits,
             ssize_t elt_size,
             t_oblist_type * type,
             void * closure)
{
  struct ol_closure olc;
  
  if (!ol)
    return 0;

  init_ol_closure (&olc, ol, limits, elt_size, type, closure);
  
  return (  obqueue__room (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc)
          + obqueue__room (RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc));
}




void *
oblist_burst (ssize_t * len_returned,
              t_oblist * ol,
              alloc_limits limits,
              ssize_t elt_size,
              t_oblist_type * type,
              void * closure,
              ssize_t n,
              ssize_t len)
{
  struct ol_closure olc;
  ssize_t left_size;

  if (n < 0)
    return 0;

  init_ol_closure (&olc, ol, limits, elt_size, type, closure);

  left_size = obqueue_size (LEFT (ol), limits, elt_size, translate_type (type), (void *)&olc);

  if (n < left_size)
    {
      ssize_t len_avail;

      if ((n + len) <= left_size)
        len_avail = len;
      else
        len_avail = (left_size - n);

      return obqueue_burst (len_returned, LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc, n, len_avail);
    }

  n = (n - obqueue_size (LEFT (ol), limits, elt_size, translate_type (type), (void *)&olc));

  return obqueue_burst (len_returned, RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc, n, len);
}


void *
oblist_range (t_oblist * ol,
              alloc_limits limits,
              ssize_t elt_size,
              t_oblist_type * type,
              void * closure,
              ssize_t n,
              ssize_t len)
{
  ssize_t gap;
  struct ol_closure olc;

  if (n < 0)
    return 0;

  init_ol_closure (&olc, ol, limits, elt_size, type, closure);

  if ((len < 0) || ((n + len) < 0) || ((n + len) > oblist_size (ol, limits, elt_size, type, closure)))
    return 0;

  gap = obqueue_size (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc);

  if ((n < gap) && ((n + len) <= gap))
    {
      return obqueue_range (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc, n, len);
    }

  if (gap <= n)
    {
      return obqueue_range (RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc, n - gap, len);
    }

  if ((gap - n) < ((n + len) - gap))
    {
      if (0 > oblist_move_gap (ol, limits, elt_size, type, closure, n))
        return 0;
      return obqueue_range (RIGHT (ol), limits, elt_size, translate_type (type), (void *)&olc, 0, len);
    }
  else
    {
      if (0 > oblist_move_gap (ol, limits, elt_size, type, closure, n + len))
        return 0;
      return obqueue_range (LEFT (ol), limits, elt_size, translate_type (type), (void *)&olc, n, len);
    }
}


static int
oblist_move_gap (t_oblist * ol,
                 alloc_limits limits,
                 ssize_t elt_size,
                 t_oblist_type * type,
                 void * closure,
                 ssize_t wanted_gap)
{
  struct ol_closure olc;
  ssize_t current_gap;
  ssize_t size;

  init_ol_closure (&olc, ol, limits, elt_size, type, closure);

  current_gap = obqueue_size (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc);
  size = oblist_size (ol, limits, elt_size, type, closure);

  if (current_gap == wanted_gap)
    return 0;

  if (wanted_gap < 0)
    return -1;

  if (wanted_gap > size)
    return -1;

  if (wanted_gap < current_gap)
    {
      ssize_t amt_to_move;
      ssize_t amt_moved;
      ssize_t source_pos;
      ssize_t dest_pos;

      amt_to_move = current_gap - wanted_gap;
      amt_moved = 0;

      source_pos = wanted_gap;
      dest_pos = 0;

      if (0 > obqueue_push_n (RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc, amt_to_move))
        return -1;

      while (amt_moved < amt_to_move)
        {
          t_uchar * left_burst;
          ssize_t lb_len;
          t_uchar * right_burst;
          ssize_t rb_len;
          ssize_t useful_len;

          left_burst = obqueue_burst (&lb_len, LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc, source_pos, (amt_to_move - amt_moved));
          right_burst = obqueue_burst (&rb_len, RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc, dest_pos, (amt_to_move - amt_moved));
          if (lb_len < rb_len)
            useful_len = lb_len;
          else
            useful_len = rb_len;

          mem_move (right_burst, left_burst, useful_len * elt_size);

          amt_moved += useful_len;
          source_pos += useful_len;
          dest_pos += useful_len;
        }

      obqueue_pop_last_n (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc, amt_to_move);
      return 0;
    }
  else /* (wanted_gap > current_gap) */
    {
      ssize_t amt_to_move;
      ssize_t amt_moved;
      ssize_t source_pos;
      ssize_t dest_pos;

      amt_to_move = wanted_gap - current_gap;
      amt_moved = 0;

      source_pos = 0;
      dest_pos = current_gap;

      if (0 > obqueue_append_n (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc, amt_to_move))
        return -1;

      while (amt_moved < amt_to_move)
        {
          t_uchar * left_burst;
          ssize_t lb_len;
          t_uchar * right_burst;
          ssize_t rb_len;
          ssize_t useful_len;

          left_burst = obqueue_burst (&lb_len, LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc, dest_pos, (amt_to_move - amt_moved));
          right_burst = obqueue_burst (&rb_len, RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc, source_pos, (amt_to_move - amt_moved));
          if (lb_len < rb_len)
            useful_len = lb_len;
          else
            useful_len = rb_len;

          mem_move (left_burst, right_burst, useful_len * elt_size);

          amt_moved += useful_len;
          source_pos += useful_len;
          dest_pos += useful_len;
        }

      obqueue_pop_n (RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc, amt_to_move);
      return 0;
    }
}



int
oblist_insert_n (t_oblist * ol,
                 alloc_limits limits,
                 ssize_t elt_size,
                 t_oblist_type * type,
                 void * closure,
                 ssize_t before_pos,
                 ssize_t n,
                 void * mem)
{
  struct ol_closure olc;
  ssize_t s;

  s = oblist_size (ol, limits, elt_size, type, closure);

  if (before_pos < 0)
    return -1;

  if (before_pos > s)
    return -1;

  init_ol_closure (&olc, ol, limits, elt_size, type, closure);

  if (before_pos == 0)
    {
      if (0 > obqueue_push_n (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc, n))
        return -1;
    }
  else if (before_pos == s)
    {
      if (0 > obqueue_append_n (RIGHT(ol), limits, elt_size, translate_type (type), (void *)&olc, n))
        return -1;
    }
  else
    {
      if (0 > oblist_move_gap (ol, limits, elt_size, type, closure, before_pos))
        return -1;

      if (0 > obqueue_append_n (LEFT(ol), limits, elt_size, translate_type (type), (void *)&olc, n))
        return -1;
    }

  if (mem)
    mem_move (oblist_range (ol, limits, elt_size, type, closure, before_pos, n), mem, n * elt_size);

  return 0;
}


int
oblist_delete_n (t_oblist * ol,
                 alloc_limits limits,
                 ssize_t elt_size,
                 t_oblist_type * type,
                 void * closure,
                 ssize_t pos,
                 ssize_t n)
{
  struct ol_closure olc;
  ssize_t s;
  ssize_t gap;
  ssize_t pos_distance;
  ssize_t end_distance;
  ssize_t new_gap;

  s = oblist_size (ol, limits, elt_size, type, closure);

  if (pos < 0)
    return -1;

  if (pos > s)
    return -1;

  if (n < 0)
    return -1;

  if (((pos + n) < 0) || ((pos + n) > s))
    n = s - pos;

  init_ol_closure (&olc, ol, limits, elt_size, type, closure);

  gap = obqueue_size (LEFT (ol), limits, elt_size, translate_type (type), (void *)&olc);

  if (gap > pos)
    pos_distance = gap - n;
  else
    pos_distance = n - gap;

  if (gap > (pos + n))
    end_distance = gap - (pos + n);
  else
    end_distance = (pos + n) - gap;

  if (pos_distance < end_distance)
    new_gap = pos;
  else
    new_gap = (pos + n);

  if (0 > oblist_move_gap (ol, limits, elt_size, type, closure, new_gap))
    return -1;

  if (new_gap == pos)
    {
      obqueue_pop_n (RIGHT (ol), limits, elt_size, translate_type (type), (void *)&olc, n);
      return 0;
    }
  else
    {
      obqueue_pop_last_n (LEFT (ol), limits, elt_size, translate_type (type), (void *)&olc, n);
      return 0;
    }
}




int
oblist_sort (t_oblist * ol,
             alloc_limits limits,
             ssize_t elt_size,
             t_oblist_type * type,
             void * lclosure,
             t_oblist_cmp_fn cmp,
             void * closure)
{
  ssize_t s;
  void * it;

  s = oblist_size (ol, limits, elt_size, type, lclosure);

  it = oblist_range (ol, limits, elt_size, type, lclosure, 0, s);

  if (!s)
    return -1;

  quicksort (it, s, elt_size, cmp, closure);

  return 0;
}


ssize_t
oblist_find (ssize_t * would_be_before,
             t_oblist * ol,
             alloc_limits limits,
             ssize_t elt_size,
             t_oblist_type * type,
             void * lclosure,
             void * key,
             t_oblist_cmp_fn cmp,
             void * closure)
{
  ssize_t lo;
  ssize_t hi;
  ssize_t leftmost_found;

  lo = 0;
  hi = oblist_size (ol, limits, elt_size, type, lclosure);
  leftmost_found = -1;

  while (lo < hi)
    {
      ssize_t mid;
      int c;

      mid = lo + ((hi - lo) >> 1);

      c = cmp (key, oblist_burst (0, ol, limits, elt_size, type, lclosure, mid, 1), closure);

      if (!c)
        {
          leftmost_found = mid;
          hi = mid;
        }
      else if (c < 0)
        {
          hi = mid;
        }
      else
        {
          lo = mid + 1;
        }
    }

  if (leftmost_found >= 0)
    return leftmost_found;
  else
    {
      if (would_be_before)
        {
          *would_be_before = lo;
        }
      
      return -1;
    }
}


ssize_t
oblist_sorted_insert (t_oblist * ol,
                      alloc_limits limits,
                      ssize_t elt_size,
                      t_oblist_type * type,
                      void * lclosure,
                      void * key,
                      t_oblist_cmp_fn cmp,
                      void * closure,
                      int copy_from_key,
                      int not_if_present)
{
  ssize_t found_at;
  ssize_t would_be_before;
  ssize_t insert_at;

  found_at = oblist_find (&would_be_before, ol, limits, elt_size, type, lclosure, key, cmp, closure);

  if (not_if_present && (found_at >= 0))
    {
      return found_at;
    }

  if (found_at >= 0)
    insert_at = found_at;
  else
    insert_at = would_be_before;

  if (0 > oblist_insert_n (ol, limits, elt_size, type, lclosure, insert_at, 1, 0))
    return -1;

  if (copy_from_key && key)
    mem_move (oblist_range (ol, limits, elt_size, type, closure, insert_at, 1), key, elt_size);

  return insert_at;
}




static void
init_ol_closure (struct ol_closure * olc,
                 t_oblist * ol,
                 alloc_limits limits,
                 ssize_t elt_size,
                 t_oblist_type * type,
                 void * closure)
{
  olc->ol = ol;
  olc->type = type;
  olc->closure = closure;
}


static t_obqueue_type *
translate_type (struct oblist_type * type)
{
  if (!type || (!type->init && !type->uninit))
    return 0;
  else if (!type->uninit)
    return &init_only;
  else if (!type->init)
    return &uninit_only;
  else
    return &init_and_uninit;
}


static int
forward_init (t_obqueue * oq,
              alloc_limits limits,
              ssize_t elt_size,
              t_obqueue_type * type,
              void * closure,
              void * mem,
              ssize_t n_elts)
{
  struct ol_closure * olc;

  olc = (struct ol_closure *)closure;

  if (!olc || !olc->type || !olc->type->init)
    return -1;

  return olc->type->init (olc->ol, limits, elt_size, olc->type, olc->closure, mem, n_elts);
}


static void
forward_uninit (t_obqueue * oq,
                alloc_limits limits,
                ssize_t elt_size,
                t_obqueue_type * type,
                void * closure,
                void * mem,
                ssize_t n_elts)
{
  struct ol_closure * olc;

  if (!olc || !olc->type || !olc->type->uninit)
    return;

  olc->type->uninit (olc->ol, limits, elt_size, olc->type, olc->closure, mem, n_elts);
}



/* tag: Tom Lord Sun Oct 24 08:12:32 2004 (oblist.c)
 */
