/* diff.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBDIFF__DIFF_H
#define INCLUDE__LIBDIFF__DIFF_H


#include "hackerlab/machine/types.h"

enum diff_op
  {
    DIFF_MATCH = 1,
    DIFF_DELETE,
    DIFF_INSERT
  };

struct diff_edit
{
  short op;
  ssize_t off;
  ssize_t len;
};
typedef enum diff_op diff_op;

typedef const void *(*idx_fn) (const void * s, size_t idx, void * context);
typedef int (*cmp_fn) (const void * a, const void * b, void * context);


/* automatically generated __STDC__ prototypes */
extern ssize_t diff(struct diff_edit ** sesl,
                    const void * a, ssize_t n,
                    const void * b, ssize_t m,
                    idx_fn idx, cmp_fn cmp, void *context,
                    t_ulong dmax);
#endif  /* INCLUDE__LIBDIFF__DIFF_H */


/* arch-tag: Tom Lord Mon Jul 11 13:19:44 2005 (diff.h)
 */



