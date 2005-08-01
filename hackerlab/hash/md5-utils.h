/* md5-utils.h:
 *
 ****************************************************************
 * Copyright (C) 2003 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__HASH__MD5_UTILS_H
#define INCLUDE__HASH__MD5_UTILS_H


#include "hackerlab/hash/md5.h"



/* automatically generated __STDC__ prototypes */
extern t_uchar * md5_ascii_for_str (alloc_limits limits,
                                    t_uchar * string);
extern t_uchar * md5_ascii_for_str_n (alloc_limits limits,
                                      t_uchar * string,
                                      size_t length);
extern void md5_for_str (t_uchar * result,
                         alloc_limits limits,
                         t_uchar * string);
extern void md5_for_str_n (t_uchar * result,
                           alloc_limits limits,
                           t_uchar * string,
                           size_t length);
#endif  /* INCLUDE__HASH__MD5_UTILS_H */


/* tag: Tom Lord Sun Dec 28 11:37:31 2003 (md5-utils.h)
 */
