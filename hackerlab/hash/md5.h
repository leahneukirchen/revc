/* md5.h:
 *
 ****************************************************************
 * Copyright (C) 2003 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__HASH__MD5_H
#define INCLUDE__HASH__MD5_H


#include "hackerlab/machine/types.h"
#include "hackerlab/mem/alloc-limits.h"



struct md5_context;
typedef struct md5_context * md5_context_t;



/* automatically generated __STDC__ prototypes */
extern md5_context_t make_md5_context (alloc_limits limits);
extern void md5_context_reset (md5_context_t ctx);
extern void free_md5_context (alloc_limits limits, md5_context_t ctx);
extern void md5_scan (md5_context_t hd, t_uchar * inbuf, size_t inlen);
extern void md5_final (t_uchar * result, md5_context_t hd);
extern t_uchar * md5_alloc_ascii (alloc_limits limits, t_uchar * result);
extern void md5_ascii (t_uchar * answer, t_uchar * result);
#endif  /* INCLUDE__HASH__MD5_H */


/* tag: Tom Lord Fri Oct 24 08:03:07 2003 (md5.h)
 */
