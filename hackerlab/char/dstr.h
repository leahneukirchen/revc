/* dstr.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__CHAR__DSTR_H
#define INCLUDE__CHAR__DSTR_H


#include "hackerlab/char/str.h"



struct dstr_handle;
typedef struct dstr_handle * t_dstr;



/* automatically generated __STDC__ prototypes */
extern t_dstr dstr_save (alloc_limits limits, const t_uchar * str);
extern t_dstr dstr_make_n (alloc_limits limits, size_t len);
extern t_dstr dstr_save_n (alloc_limits limits, const t_uchar * str, size_t len);
extern t_dstr dstr_copy (alloc_limits limits, t_dstr orig);
extern t_dstr dstr_take (alloc_limits limits,
                         t_uchar * str, size_t len);
extern void dstr_ref (t_dstr s);
extern void dstr_unref (t_dstr s);
extern size_t dstr_length (t_dstr x);
extern t_uchar * dstr_data (size_t * length, t_dstr x);
extern unsigned long dstr_hash (t_dstr str);
extern int dstr_cmp (t_dstr ad, t_dstr bd);
extern int dstr_casecmp (t_dstr ad, t_dstr bd);
extern int dstr_cat (t_dstr to, t_dstr from);
extern t_dstr dstr_append (alloc_limits limits,
                           t_dstr left, t_dstr right);
extern void dstr_shrink (t_dstr str, size_t start, size_t end);
extern t_dstr dstr_substr (alloc_limits limits,
                           t_dstr str,
                           size_t start, size_t end);
#endif  /* INCLUDE__CHAR__DSTR_H */


/* tag: Tom Lord Thu Jan  1 10:49:53 2004 (dstr.h)
 */
