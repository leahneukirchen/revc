/* error.h:
 *
 ****************************************************************
 * Copyright (C) 2003 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__BUGS__ERROR_H
#define INCLUDE__BUGS__ERROR_H


#include "hackerlab/machine/types.h"



struct err_type_vtable;

struct err_type_description
{
  /* Applications should not access these fields
   * directly.
   */

  char * name;
  char * msg;
  struct err_type_vtable * vtable;

  char * domain;
  t_uint32 code;

  int is_proxy;
  struct err_type_description * proxy_for;

#if 0
  t_uint64 domhash_hi;
  t_uint64 domhash_lo;
#endif

  t_uint domseq;
  t_uint typeseq;
};

typedef struct err_type_description err_type_t;

struct err_value
{
  /* Applications should not access these fields
   * directly.
   */
  err_type_t * type;

  t_uint event_n;
  t_uint32 logging_done;

  union
    {
      struct err_value * e;

      t_uchar * s;
      t_uchar ** sa;

      int i;
      t_uint u;

      t_int32 i32;
      t_uint32 u32;

      float f;
      double d;

      void * v;
    } params;
};

typedef struct err_value err_t;



#define ERR_TYPE_DECL(NAME,MSG,VTBL) \
  struct err_type_description NAME = { #NAME, MSG, VTBL, 0, }

#define ERR_STDTYPE_DECL(NAME,MSG,VTBL,DOM,CODE) \
  struct err_type_description NAME = { #NAME, MSG, VTBL, DOM, CODE, 0, }

#define ERR_INIT() { 0, }


/* automatically generated __STDC__ prototypes */
extern int err_types_equal (err_type_t * a, err_type_t * b);
extern int err_is_type (err_type_t * type, err_t * err);
extern void err_set_type (err_t * dest, err_type_t * type);
extern err_type_t err_type (err_t * err);
extern t_uchar * err_name (err_t * err);
extern t_uchar * err_msg (err_t * err);
extern void err_type_coordinates (err_type_t * type);
extern void err_clean (err_t * err);
extern void err_propogate (err_t * to, err_t * from);
#endif  /* INCLUDE__BUGS__ERROR_H */


/* tag: Tom Lord Thu Oct 23 15:10:31 2003 (error.h)
 */
