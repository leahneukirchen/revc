/* error.c: 
 *
 ****************************************************************
 * Copyright (C) 2003 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/error.h"


/* __STDC__ prototypes for static functions */
static int err_is_proxy_for (err_type_t * a, err_type_t * b);
static int err_equivalent_proxies (err_t * a, err_t * b);



int
err_types_equal (err_type_t * a, err_type_t * b)
{
  if (!a)
    return 0;
  else if (!b)
    return 0;
  else if (err_is_proxy_for (a, b))
    return 1;
  else if (err_is_proxy_for (b, a))
    return 1;
  else
    return (a == b);
  
  return 0;
}


static int
err_is_proxy_for (err_type_t * a, err_type_t * b)
{
  if (a->proxy_for == b)
    return 1;
  else if (b->proxy_for == a)
    return 1;
  else if (!a->is_proxy && !b->is_proxy)
    return 0;
  else if (a->is_proxy && b->is_proxy)
    return err_equivalent_proxies (a, b);
  else if (!a->is_proxy && b->proxy_for && (b->proxy_for != a))
    return 0;
  else if (!b->is_proxy && a->proxy_for && (a->proxy_for != b))
    return 0;
  else
    {
      err_type_t * proxy;
      err_type_t * canonical;

      invariant (!(a->is_proxy && b->is_proxy));
      invariant (a->is_proxy || b->is_proxy);

      if (a->is_proxy)
        {
          proxy = a;
          canonical = b;
        }
      else
        {
          proxy = b;
          canonical = a;
        }

      return err_is_canonical_for (canonical, proxy);
    }
}


static int
err_equivalent_proxies (err_t * a, err_t * b)
{
#undef fix
  


  return 0;
}



int
err_is_type (err_type_t * type, err_t * err)
{
  return err_types_equal (type, err->type);
}


void
err_set_type (err_t * dest, err_type_t * type)
{
  err_clean (dest);
  dest->type = type;
}


err_type_t
err_type (err_t * err)
{
  return dest->type;
}


t_uchar *
err_name (err_t * err)
{
  if (!err->type)
    return 0;
  else
    return (t_uchar *)err->type->name;
}


t_uchar *
err_msg (err_t * err)
{
  if (!err->type)
    return 0;
  else
    return (t_uchar *)err->type->msg;
}


void
err_type_coordinates (err_type_t * type)
{
  
}




void
err_clean (err_t * err)
{
  if (err->vtable)
    err->vtable->clean (err);

  mem_set0 ((t_uchar *)err, sizeof (*err));
}




void
err_propogate (err_t * to, err_t * from)
{
  err_clean (to);
  *to = *from;
  mem_set0 ((t_uchar *)from, sizeof (*from));
}







/* tag: Tom Lord Thu Oct 23 15:51:34 2003 (error.c)
 */
