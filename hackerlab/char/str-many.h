/* str-many.h: decls for vararg string functions
 *
 ****************************************************************
 * Copyright (C) 2003 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__CHAR__STR_MANY_H
#define INCLUDE__CHAR__STR_MANY_H


#include "hackerlab/machine/types.h"
#include "hackerlab/os/stdarg.h"
#include "hackerlab/mem/alloc-limits.h"



extern t_uchar str_end[];



/* automatically generated __STDC__ prototypes */
extern t_uchar * str_cat_many (t_uchar * to, ...);
extern t_uchar * str_alloc_cat_many (alloc_limits limits, ...);
extern t_uchar * str_realloc_cat_many (alloc_limits limits,
                                       t_uchar * str1,
                                       ...);
#endif  /* INCLUDE__CHAR__STR_MANY_H */


/* tag: Tom Lord Sat Jan  5 15:26:10 2002 (str-many.h)
 */
