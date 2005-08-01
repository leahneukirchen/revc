/* obqueue.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/arrays/ar.h"
#include "hackerlab/obqueues/obqueue.h"


/* __STDC__ prototypes for static functions */
static ssize_t obqueue__head (t_obqueue * oq,
                              alloc_limits limits,
                              ssize_t elt_size,
                              t_obqueue_type * type,
                              void * closure);
static ssize_t obqueue__tail (t_obqueue * oq,
                              alloc_limits limits,
                              ssize_t elt_size,
                              t_obqueue_type * type,
                              void * closure);
static ssize_t ideal_room_for_size (ssize_t s);
static int ensure_room (t_obqueue * oq,
                        alloc_limits limits,
                        ssize_t elt_size,
                        t_obqueue_type * type,
                        void * closure,
                        ssize_t n_more);
static void uninit_elements (t_obqueue * oq,
                             alloc_limits limits,
                             ssize_t elt_size,
                             t_obqueue_type * type,
                             void * closure,
                             ssize_t from,
                             ssize_t to);
static int init_elements (t_obqueue * oq,
                          alloc_limits limits,
                          ssize_t elt_size,
                          t_obqueue_type * type,
                          void * closure,
                          ssize_t from,
                          ssize_t to);


int
init_obqueue (t_obqueue * oq,
              alloc_limits limits,
              ssize_t elt_size,
              t_obqueue_type * type,
              void * closure)
{
  if (!oq)
    return -1;

  mem_set0 ((t_uchar *)oq, sizeof (*oq));
  return 0;
}


void
uninit_obqueue (t_obqueue * oq,
                alloc_limits limits,
                ssize_t elt_size,
                t_obqueue_type * type,
                void * closure)
{
  if (!oq)
    return;

  if (type && type->uninit)
    {
      ssize_t r;
      ssize_t h;
      ssize_t t;
      
      r = obqueue__room (oq, limits, elt_size, type, closure);
      h = obqueue__head (oq, limits, elt_size, type, closure);
      t = obqueue__tail (oq, limits, elt_size, type, closure);

      if (h < t)
        {
          type->uninit (oq, limits, elt_size, type, closure, (void *)(oq->_storage + h * elt_size), t - h);
        }
      else if (t < h)
        {
          type->uninit (oq, limits, elt_size, type, closure, (void *)(oq->_storage), t);
          type->uninit (oq, limits, elt_size, type, closure, (void *)(oq->_storage + h * elt_size), r - h);
        }
    }
  ar_free ((void **)&oq->_storage, limits);
  mem_set0 ((t_uchar *)oq, sizeof (*oq));
}



ssize_t
obqueue__room (t_obqueue * oq,
               alloc_limits limits,
               ssize_t elt_size,
               t_obqueue_type * type,
               void * closure)
{
  if (!oq)
    return 0;
  else
    return (ssize_t)ar_size ((void *)oq->_storage, limits, elt_size);
}


static ssize_t
obqueue__head (t_obqueue * oq,
               alloc_limits limits,
               ssize_t elt_size,
               t_obqueue_type * type,
               void * closure)
{
  if (!oq)
    return 0;
  else
    return oq->_head;
}


static ssize_t
obqueue__tail (t_obqueue * oq,
               alloc_limits limits,
               ssize_t elt_size,
               t_obqueue_type * type,
               void * closure)
{
  if (!oq)
    return 0;
  else
    return oq->_tail;
}



int
obqueue_is_empty (t_obqueue * oq,
                  alloc_limits limits,
                  ssize_t elt_size,
                  t_obqueue_type * type,
                  void * closure)
{
  return (   obqueue__head (oq, limits, elt_size, type, closure)
          == obqueue__tail (oq, limits, elt_size, type, closure));
}


int
obqueue_size (t_obqueue * oq,
              alloc_limits limits,
              ssize_t elt_size,
              t_obqueue_type * type,
              void * closure)
{
  ssize_t h;
  ssize_t t;
  ssize_t r;

  h = obqueue__head (oq, limits, elt_size, type, closure);
  t = obqueue__tail (oq, limits, elt_size, type, closure);
  r = obqueue__room (oq, limits, elt_size, type, closure);

  if (h <= t)
    return t - h;
  else
    return (t + (r - h));
}


ssize_t
obqueue__nth_place (t_obqueue * oq,
                    alloc_limits limits,
                    ssize_t elt_size,
                    t_obqueue_type * type,
                    void * closure,
                    ssize_t n)
{
  ssize_t s;
  ssize_t h;
  ssize_t t;
  ssize_t r;

  s = obqueue_size (oq, limits, elt_size, type, closure);

  if (n < 0)
    n += s;

  if (n < 0)
    return -1;

  if (n >= s)
    return -1;

  h = obqueue__head (oq, limits, elt_size, type, closure);
  t = obqueue__tail (oq, limits, elt_size, type, closure);
  r = obqueue__room (oq, limits, elt_size, type, closure);

  if (h < t)
    {
      return h + n;
    }
  else if ((r - h) > n)
    {
      return h + n;
    }
  else
    {
      return n - (r - h);
    }
}


void *
obqueue_peek (t_obqueue * oq,
              alloc_limits limits,
              ssize_t elt_size,
              t_obqueue_type * type,
              void * closure)
{
  return obqueue_n (oq, limits, elt_size, type, closure, 0);
}


void *
obqueue_n (t_obqueue * oq,
           alloc_limits limits,
           ssize_t elt_size,
           t_obqueue_type * type,
           void * closure,
           ssize_t n)
{
  ssize_t place;

  place = obqueue__nth_place (oq, limits, elt_size, type, closure, n);

  if (place < 0)
    return 0;

  return oq->_storage + place * elt_size;
}


void *
obqueue_burst (ssize_t * len_returned,
               t_obqueue * oq,
               alloc_limits limits,
               ssize_t elt_size,
               t_obqueue_type * type,
               void * closure,
               ssize_t n,
               ssize_t len)
{
  ssize_t s;
  ssize_t h;
  ssize_t t;
  ssize_t r;
  ssize_t n_place;
  ssize_t end_place;
  void * answer;
  

  s = obqueue_size (oq, limits, elt_size, type, closure);
  h = obqueue__head (oq, limits, elt_size, type, closure);
  t = obqueue__tail (oq, limits, elt_size, type, closure);
  r = obqueue__room (oq, limits, elt_size, type, closure);

  n_place = obqueue__nth_place (oq, limits, elt_size, type, closure, n);
  end_place = obqueue__nth_place (oq, limits, elt_size, type, closure, n + len - 1);

  if ((n_place < 0) || (end_place < 0))
    return 0;

  answer = oq->_storage + n_place * elt_size;

  if (len_returned)
    {
      if (n_place >= h)
        {
          if (end_place >= n_place)
            *len_returned = len;
          else
            *len_returned = (r - n_place);
        }
      else
        {
          if (end_place < t)
            *len_returned = len;
          else
            *len_returned = t - n_place;
        }
    }
  return answer;
}


void *
obqueue_range (t_obqueue * oq,
               alloc_limits limits,
               ssize_t elt_size,
               t_obqueue_type * type,
               void * closure,
               ssize_t n,
               ssize_t len)
{
  ssize_t s;
  ssize_t h;
  ssize_t t;
  ssize_t r;
  ssize_t n_place;
  ssize_t end_place;
  void * answer;
  
  s = obqueue_size (oq, limits, elt_size, type, closure);
  h = obqueue__head (oq, limits, elt_size, type, closure);
  t = obqueue__tail (oq, limits, elt_size, type, closure);
  r = obqueue__room (oq, limits, elt_size, type, closure);

  n_place = obqueue__nth_place (oq, limits, elt_size, type, closure, n);
  end_place = obqueue__nth_place (oq, limits, elt_size, type, closure, n + len - 1);

  if ((n_place < 0) || (end_place < 0))
    return 0;

  answer = oq->_storage + n_place * elt_size;

  if (n_place >= h)
    {
      if (end_place >= n_place)
        return answer;
    }
  else
    {
      if (end_place <= t)
        return answer;
    }

  /* buffer needs to be rotated.
   *
   * stuff-a tail=gap head=stuff-b
   *   			     ^
   *                         N
   */
  {
    t_uchar * new_array = 0;
    ssize_t new_r;
    ssize_t h_len;

    new_r = ideal_room_for_size (s);
    
    if (0 > ar_setsize ((void **)&new_array, limits, new_r, elt_size))
      {
        return 0;
      }

    h_len = (r - h);

    mem_move (new_array, oq->_storage + h * elt_size, h_len * elt_size);
    mem_move (new_array + h_len * elt_size, oq->_storage, t * elt_size);
    ar_free ((void **)&oq->_storage, limits);
    oq->_storage = new_array;
    oq->_head = 0;
    oq->_tail = s;
  }

  return oq->_storage + n * elt_size;
}

static ssize_t
ideal_room_for_size (ssize_t s)
{
  s += 1;
  if (s <= 0)
    return 0;
  if (s < 256)
    return 16 * ((s + 15) / 16);
  if (s < 10240)
    return 256 * ((s + 255) / 256);
  return 10240 * ((s + 10239) / 10240);
}

void *
obqueue_push (t_obqueue * oq,
              alloc_limits limits,
              ssize_t elt_size,
              t_obqueue_type * type,
              void * closure)
{
  ssize_t h;
  ssize_t r;
  void * answer;

  if (0 > ensure_room (oq, limits, elt_size, type, closure, 1))
    return 0;

  h = obqueue__head (oq, limits, elt_size, type, closure);
  r = obqueue__room (oq, limits, elt_size, type, closure);

  --h;
  if (h < 0)
    h = (r - 1);

  answer = oq->_storage + h * elt_size;

  if (type && type->init)
    {
      if (0 > type->init (oq, limits, elt_size, type, closure, answer, 1))
        return 0;
    }
  else
    {
      mem_set0 ((t_uchar *)answer, elt_size);
    }

  oq->_head = h;

  return answer;
}


int
obqueue_push_n (t_obqueue * oq,
                alloc_limits limits,
                ssize_t elt_size,
                t_obqueue_type * type,
                void * closure,
                ssize_t n)
{
  ssize_t h;
  ssize_t saved_head;
  ssize_t r;

  if (0 > ensure_room (oq, limits, elt_size, type, closure, n))
    return -1;

  h = obqueue__head (oq, limits, elt_size, type, closure);
  saved_head = h;
  r = obqueue__room (oq, limits, elt_size, type, closure);

  h = h - n;
  if (h < 0)
    h = (r + h);

  oq->_head = h;
  if (0 > init_elements (oq, limits, elt_size, type, closure, 0, n))
    {
      oq->_head = saved_head;
      return -1;
    }

  return 0;
}


void
obqueue_pop (t_obqueue * oq,
             alloc_limits limits,
             ssize_t elt_size,
             t_obqueue_type * type,
             void * closure)
{
  if (obqueue_is_empty (oq, limits, elt_size, type, closure))
    return;

  if (type && type->uninit)
    type->uninit (oq, limits, elt_size, type, closure, (void *)(oq->_storage + oq->_head * elt_size), 1);

  ++oq->_head;

  if (oq->_head >= obqueue__room (oq, limits, elt_size, type, closure))
    oq->_head = 0;
}


void
obqueue_pop_n (t_obqueue * oq,
               alloc_limits limits,
               ssize_t elt_size,
               t_obqueue_type * type,
               void * closure,
               ssize_t n)
{
  if (obqueue_is_empty (oq, limits, elt_size, type, closure))
    return;

  if (n > obqueue_size (oq, limits, elt_size, type, closure))
    n = obqueue_size (oq, limits, elt_size, type, closure);

  if (n <= 0)
    return;

  uninit_elements (oq, limits, elt_size, type, closure, 0, n);

  oq->_head += n;

  if (oq->_head >= obqueue__room (oq, limits, elt_size, type, closure))
    oq->_head = (oq->_head - obqueue__room (oq, limits, elt_size, type, closure));
}


void *
obqueue_append (t_obqueue * oq,
                alloc_limits limits,
                ssize_t elt_size,
                t_obqueue_type * type,
                void * closure)
{
  void * answer;
  ssize_t t;

  if (0 > ensure_room (oq, limits, elt_size, type, closure, 1))
    return 0;

  answer = oq->_storage + oq->_tail * elt_size;

  t = oq->_tail + 1;
  if (t >= obqueue__room (oq, limits, elt_size, type, closure))
    t = 0;

  if (type && type->init)
    {
      if (0 > type->init (oq, limits, elt_size, type, closure, answer, 1))
        return 0;
    }
  else
    {
      mem_set0 ((t_uchar *)answer, elt_size);
    }

  oq->_tail = t;

  return answer;
}


int
obqueue_append_n (t_obqueue * oq,
                  alloc_limits limits,
                  ssize_t elt_size,
                  t_obqueue_type * type,
                  void * closure,
                  ssize_t n)
{
  ssize_t saved_s;
  ssize_t t;
  ssize_t saved_t;
  ssize_t r;

  if (0 > ensure_room (oq, limits, elt_size, type, closure, n))
    return -1;

  saved_s = obqueue_size (oq, limits, elt_size, type, closure);

  saved_t = obqueue__tail (oq, limits, elt_size, type, closure);
  t = n + saved_t;
  r = obqueue__room (oq, limits, elt_size, type, closure);

  if (t >= r)
    t = t - r;

  oq->_tail = t;

  if (0 > init_elements (oq, limits, elt_size, type, closure, saved_s, saved_s + n))
    {
      oq->_tail = saved_t;
      return -1;
    }
  return 0;
}


void
obqueue_pop_last (t_obqueue * oq,
                  alloc_limits limits,
                  ssize_t elt_size,
                  t_obqueue_type * type,
                  void * closure)
{
  if (obqueue_is_empty (oq, limits, elt_size, type, closure))
    return;

  --oq->_tail;

  if (oq->_tail < 0)
    oq->_tail = (obqueue__room (oq, limits, elt_size, type, closure) - 1);

  if (type && type->uninit)
    {
      type->uninit (oq, limits, elt_size, type, closure, (void *)(oq->_storage + oq->_tail * elt_size), 1);
    }
}



void
obqueue_pop_last_n (t_obqueue * oq,
                    alloc_limits limits,
                    ssize_t elt_size,
                    t_obqueue_type * type,
                    void * closure,
                    ssize_t n)
{
  ssize_t s;

  if (obqueue_is_empty (oq, limits, elt_size, type, closure))
    return;

  s = obqueue_size (oq, limits, elt_size, type, closure);

  if (n > s)
    n = s;

  uninit_elements (oq, limits, elt_size, type, closure, (s - n), s);

  oq->_tail -= n;

  if (oq->_tail < 0)
    oq->_tail = (obqueue__room (oq, limits, elt_size, type, closure) + oq->_tail);
}



static int
ensure_room (t_obqueue * oq,
             alloc_limits limits,
             ssize_t elt_size,
             t_obqueue_type * type,
             void * closure,
             ssize_t n_more)
{
  ssize_t h;
  ssize_t t;
  ssize_t s;
  ssize_t r;
  ssize_t new_r;

  if (n_more < 0)
    return -1;

  h = obqueue__head (oq, limits, elt_size, type, closure);
  t = obqueue__tail (oq, limits, elt_size, type, closure);
  s = obqueue_size (oq, limits, elt_size, type, closure);
  r = obqueue__room (oq, limits, elt_size, type, closure);

  if ((r - s) > n_more)
    return 0;

  new_r = ideal_room_for_size (s + n_more);

  if (0 > ar_setsize ((void **)&oq->_storage, limits, new_r, elt_size))
    return -1;

  if (h > t)
    {
      mem_move (oq->_storage + (new_r - (r - h)) * elt_size,
                oq->_storage + h * elt_size,
                (r - h) * elt_size);
      oq->_head = (new_r - (r - h));
    }

  return 0;
}


static void
uninit_elements (t_obqueue * oq,
                 alloc_limits limits,
                 ssize_t elt_size,
                 t_obqueue_type * type,
                 void * closure,
                 ssize_t from,
                 ssize_t to)
{
  if (type && type->uninit)
    {
      ssize_t pos;
      ssize_t todo;

      pos = from;
      todo = (to - from);
      while (todo)
        {
          void * burst;
          ssize_t burst_amt;

          burst = obqueue_burst (&burst_amt, oq, limits, elt_size, type, closure, pos, todo);
          type->uninit (oq, limits, elt_size, type, closure, burst, burst_amt);
          pos += burst_amt;
          todo -= burst_amt;
        }
    }
}


static int
init_elements (t_obqueue * oq,
               alloc_limits limits,
               ssize_t elt_size,
               t_obqueue_type * type,
               void * closure,
               ssize_t from,
               ssize_t to)
{
  ssize_t pos;
  ssize_t todo;

  pos = from;
  todo = (to - from);
  while (todo)
    {
      void * burst;
      ssize_t burst_amt;

      burst = obqueue_burst (&burst_amt, oq, limits, elt_size, type, closure, pos, todo);
      if (!type || !type->init)
        {
          mem_set0 ((t_uchar *)burst, burst_amt);
        }
      else
        {
          if (0 > type->init (oq, limits, elt_size, type, closure, burst, burst_amt))
            {
              uninit_elements (oq, limits, elt_size, type, closure, from, to - todo);
              return -1;
            }
        }
      pos += burst_amt;
      todo -= burst_amt;
    }
  return 0;
}







/* tag: Tom Lord Fri Oct 22 16:27:00 2004 (obqueue.c)
 */
