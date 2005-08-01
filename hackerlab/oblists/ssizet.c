/* ssizet.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/oblists/ssizet.h"


/* typedefs */


/* __STDC__ prototypes for static functions */
static int cmp (void * ap, void * bp, void * ign);


/* declarations */


/* code */

int
init_list_ssize_t (t_list_ssize_t * list,
                   alloc_limits limits)
{
  if (!list)
    return -1;

  return init_oblist (&list->list, limits, sizeof (ssize_t), 0, 0);
}


void
uninit_list_ssize_t (t_list_ssize_t * list, alloc_limits limits)
{
  if (!list)
    return;

  return uninit_oblist (&list->list, limits, sizeof (ssize_t), 0, 0);
}


int
list_ssize_t_is_empty (t_list_ssize_t * list,
                       alloc_limits limits)
{
  ssize_t l;

  l = oblist_size (&list->list, limits, sizeof (ssize_t), 0, 0);
  if (l < 0)
    return -1;

  return (l == 0);
}


ssize_t
list_ssize_t_pop (alloc_limits limits,
                  t_list_ssize_t * list)
{
  int is_empty;
  ssize_t * box;
  ssize_t answer;

  is_empty = list_ssize_t_is_empty (list, limits);
  if (is_empty)
    return is_empty;

  box = (ssize_t *)oblist_range (&list->list, limits, sizeof (ssize_t), 0, 0, 0, 1);
  if (!box)
    return -1;

  answer = *box;

  if (0 > oblist_delete_n (&list->list, limits, sizeof (ssize_t), 0, 0, 0, 1))
    return -1;

  return answer;
}


ssize_t
list_ssize_t_bag_add (alloc_limits limits,
                      t_list_ssize_t * list,
                      ssize_t value)
{
  if (!list)
    return -1;

  return oblist_sorted_insert (&list->list, limits, sizeof (ssize_t), 0, 0, &value, cmp, 0, 1, 0);
}


int
list_ssize_t_remove (alloc_limits limits,
                     t_list_ssize_t * list,
                     ssize_t value)
{
  ssize_t where;

  if (!list)
    return -1;

  where = oblist_find (0, &list->list, limits, sizeof (ssize_t), 0, 0, &value, cmp, 0);
  if (where < 0)
    return -1;

  return oblist_delete_n (&list->list, limits, sizeof (ssize_t), 0, 0, where, 1);
}


static int
cmp (void * ap, void * bp, void * ign)
{
  ssize_t a;
  ssize_t b;

  a = *(ssize_t *)ap;
  b = *(ssize_t *)bp;

  if (a < b)
    return -1;
  else if (a > b)
    return 1;
  else
    return 0;
}






/* tag: Tom Lord Sat Nov 20 16:45:28 2004 (ssizet.c)
 */
