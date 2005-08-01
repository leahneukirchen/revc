/* obtable.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/arrays/ar.h"
#include "hackerlab/obtables/obtable.h"


/* __STDC__ prototypes for static functions */



int
init_obtable (t_obtable * ot,
              alloc_limits limits,
              ssize_t elt_size,
              t_obtable_type * type,
              void * closure)
{
  if (!ot)
    return -1;
  else
    {
      mem_set0 ((t_uchar *)ot, sizeof (*ot));
      ot->_free_list = -1;
      return 0;
    }
}


void
uninit_obtable (t_obtable * ot,
                alloc_limits limits,
                ssize_t elt_size,
                t_obtable_type * type,
                void * closure)
{
  if (!ot)
    return;

  if (type && type->uninit)
    {
      t_uchar * mem;
      ssize_t bound;
      ssize_t x;
      
      mem = ot->_storage;
      bound = obtable_room (ot, limits, elt_size, type, closure);

      for (x = 0; x < bound; ++x)
        {
          if (x == ot->_free_list_link[x])
            type->uninit (ot, limits, elt_size, type, closure, (void *)(mem + x * elt_size), 1);
        }
    }

  ar_free ((void **)&ot->_storage, limits);
  ar_free ((void **)&ot->_free_list_link, limits);
}



ssize_t
obtable_alloc (t_obtable * ot,
               alloc_limits limits,
               ssize_t elt_size,
               t_obtable_type * type,
               void * closure)
{
  if (!ot)
    return -1;

  if (0 <= ot->_free_list)
    {
      ssize_t answer;
      ssize_t new_free_list;
      void * elt;

      answer = ot->_free_list;
      elt = (void *)((t_uchar *)ot->_storage + answer * elt_size);
      new_free_list = ot->_free_list_link[answer];


      if (!type || !type->init)
        {
          mem_set0 ((t_uchar *)elt, sizeof (*elt));
        }
      else
        {
          if (0 > type->init (ot, limits, elt_size, type, closure, elt, 1))
            return -1;
        }

      ot->_free_list_link[answer] = answer;
      ot->_free_list = new_free_list;
      --ot->_n_free;

      return answer;
    }
  else
    {
      ssize_t answer;
      void * elt;
      ssize_t * free_link;

      answer = ar_size ((void *)ot->_storage, limits, elt_size);

      elt = (void *)ar_push ((void **)&ot->_storage, limits, elt_size);
      if (!elt)
        return -1;

      free_link = (ssize_t *)ar_push ((void **)&ot->_free_list_link, limits, sizeof (ssize_t));
      if (!free_link)
        return -1;

      if (type && type->init)
        {
          if (0 > type->init (ot, limits, elt_size, type, closure, elt, 1))
            return -1;
        }

      invariant (free_link == &ot->_free_list_link[answer]);
      *free_link = answer;

      return answer;
    }
}


void
obtable_free (t_obtable * ot,
              alloc_limits limits,
              ssize_t elt_size,
              t_obtable_type * type,
              void * closure,
              ssize_t it)
{
  void * elt;

  elt = obtable_ref (ot, limits, elt_size, type, closure, it);

  if (elt)
    {
      if (!type || !type->uninit)
        mem_set0 ((t_uchar *)elt, sizeof (*elt));
      else
        type->uninit (ot, limits, elt_size, type, closure, elt, 1);
      ot->_free_list_link[it] = ot->_free_list;
      ot->_free_list = it;
      ++ot->_n_free;
    }
}




ssize_t
obtable_n_elts (t_obtable * ot,
                alloc_limits limits,
                ssize_t elt_size,
                t_obtable_type * type,
                void * closure)
{
  if (!ot)
    return 0;
  else
    return obtable_room (ot, limits, elt_size, type, closure) - ot->_n_free;
}


ssize_t
obtable_is_free (t_obtable * ot,
                 alloc_limits limits,
                 ssize_t elt_size,
                 t_obtable_type * type,
                 void * closure,
                 ssize_t n)
{
  if (!ot)
    return -1;
  else
    return !!obtable_ref (ot, limits, elt_size, type, closure, n);
}


void *
obtable_ref (t_obtable * ot,
             alloc_limits limits,
             ssize_t elt_size,
             t_obtable_type * type,
             void * closure,
             ssize_t n)
{
  void * elt;

  if (!ot)
    return 0;

  if (n < 0)
    return 0;

  if (n >= obtable_room (ot, limits, elt_size, type, closure))
    return 0;

  elt = (void *)((t_uchar *)ot->_storage + n * elt_size);

  if (n != ot->_free_list_link[n])
    return 0;

  return elt;
}


ssize_t
obtable_room (t_obtable * ot,
              alloc_limits limits,
              ssize_t elt_size,
              t_obtable_type * type,
              void * closure)
{
  if (!ot)
    return 0;
  else
    return (ssize_t)ar_size ((void *)ot->_storage, limits, elt_size);
}



/* tag: Tom Lord Fri Oct 22 15:16:24 2004 (obtable.c)
 */
