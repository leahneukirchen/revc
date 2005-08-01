/* diff.c: compute a shortest edit script (SES) given two sequences
 *
 ****************************************************************
 * Modifications Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 * 
 * Derived from:
 * 
 * Copyright (c) 2004 Michael B. Allen <mba2000 ioplex.com>
 *
 * The MIT License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */



#include "hackerlab/machine/types.h"
#include "hackerlab/arrays/ar.h"
#include "libdiff/diff.h"



/* This algorithm is basically Myers' solution to SES/LCS with
 * the Hirschberg linear space refinement as described in the
 * following publication:
 *
 *   E. Myers, ``An O(ND) Difference Algorithm and Its Variations,''
 *   Algorithmica 1, 2 (1986), 251-266.
 *   http://www.cs.arizona.edu/people/gene/PAPERS/diff.ps
 *
 * This is the same algorithm used by GNU diff(1).
 */


#define FV(k) v(ctx, (k), 0)
#define RV(k) v(ctx, (k), 1)

struct ctx
{
  idx_fn idx;
  cmp_fn cmp;
  void * context;
  ssize_t * buf;
  struct diff_edit ** ses;
  ssize_t si;
  ssize_t dmax;
};

struct middle_snake
{
  ssize_t x, y, u, v;
};



/* __STDC__ prototypes for static functions */



static void edit (struct ctx * ctx, ssize_t op, ssize_t off, ssize_t len);
static ssize_t ses (const void * a, ssize_t aoff, ssize_t n,
                    const void * b, ssize_t boff, ssize_t m,
                    struct ctx * ctx);
static ssize_t find_middle_snake (const void * a, ssize_t aoff, ssize_t n,
                                  const void * b, ssize_t boff, ssize_t m,
                                  struct ctx * ctx,
                                  struct middle_snake * ms);
static void setv (struct ctx *ctx, ssize_t k, ssize_t r, ssize_t val);
static ssize_t v(struct ctx *ctx, ssize_t k, ssize_t r);






ssize_t
diff(struct diff_edit ** sesl,
     const void * a, ssize_t n,
     const void * b, ssize_t m,
     idx_fn idx, cmp_fn cmp, void *context,
     t_ulong dmax)
{
  struct ctx ctx;
  ssize_t d, x, y;
  struct diff_edit *e = NULL;

  if (!idx != !cmp)
    {
      /* ensure both NULL or both non-NULL */
      /* dangle */
      /* errno = EINVAL; */
      return -1;
    }

  ctx.idx = idx;
  ctx.cmp = cmp;
  ctx.context = context;
  ctx.buf = 0;

  ctx.ses = sesl;
  ctx.si = 0;
  ctx.dmax = dmax ? dmax : (ssize_t)((~(size_t)0) >> 1);

  if (sesl)
    {
      e = (struct diff_edit *)ar_ref ((void **)ctx.ses, 0, 0, sizeof (struct diff_edit));
      e->op = 0;
    }

  /* The ses function assumes the SES will begin or end with a delete
   * or insert. The following will insure this is true by eating any
   * beginning matches. This is also a quick to process sequences
   * that match entirely.
   */
  x = y = 0;
  if (cmp)
    {
      while ((x < n)
             && (y < m)
             && !cmp (idx (a, x, context), idx (b, y, context), context))
        {
          x++;
          y++;
        }
    }
  else
    {
      const unsigned char * a0 = (const unsigned char *)a;
      const unsigned char * b0 = (const unsigned char *)b;
      while (x < n && y < m && a0[x] == b0[y])
        {
          x++;
          y++;
        }
    }

  edit (&ctx, DIFF_MATCH, 0, x);

  if ((d = ses (a, x, n - x, b, y, m - y, &ctx)) == -1)
    {
      ar_free ((void **)&ctx.buf, 0);
      return -1;
    }

  ar_free ((void **)&ctx.buf, 0);

  return d;
}



static void
edit (struct ctx * ctx, ssize_t op, ssize_t off, ssize_t len)
{
  struct diff_edit *e;

  if (len == 0 || ctx->ses == NULL)
    {
      return;
    }
  /* Add an edit to the SES (or
   * coalesce if the op is the same)
s   */
  e = (struct diff_edit *)ar_ref ((void **)ctx->ses, 0, ctx->si, sizeof (struct diff_edit));
  if (e->op != op)
    {
      if (e->op)
        {
          ctx->si++;
          e = (struct diff_edit *)ar_ref ((void **)ctx->ses, 0, ctx->si, sizeof (struct diff_edit));
        }
      e->op = op;
      e->off = off;
      e->len = len;
    }
  else
    {
      e->len += len;
    }
}


static ssize_t
ses (const void * a, ssize_t aoff, ssize_t n,
     const void * b, ssize_t boff, ssize_t m,
     struct ctx * ctx)
{
  struct middle_snake ms;
  ssize_t d;

  if (n == 0)
    {
      edit (ctx, DIFF_INSERT, boff, m);
      d = m;
    }
  else if (m == 0)
    {
      edit (ctx, DIFF_DELETE, aoff, n);
      d = n;
    }
  else
    {
      /* Find the middle "snake" around which we
       * recursively solve the sub-problems.
       */
      d = find_middle_snake (a, aoff, n, b, boff, m, ctx, &ms);
      if (d == -1)
        {
          return -1;
        }
      else if (d >= ctx->dmax)
        {
          return ctx->dmax;
        }
      else if (ctx->ses == NULL)
        {
          return d;
        }
      else if (d > 1)
        {
          if (ses (a, aoff, ms.x, b, boff, ms.y, ctx) == -1)
            {
              return -1;
            }

          edit (ctx, DIFF_MATCH, aoff + ms.x, ms.u - ms.x);

          aoff += ms.u;
          boff += ms.v;
          n -= ms.u;
          m -= ms.v;
          if (ses (a, aoff, n, b, boff, m, ctx) == -1)
            {
              return -1;
            }
        }
      else
        {
          ssize_t x = ms.x;
          ssize_t u = ms.u;

          /* There are only 4 base cases when the
           * edit distance is 1.
           *
           * n > m   m > n
           *
           *   -       |
           *    \       \    x != u
           *     \       \
           *
           *   \       \
           *    \       \    x == u
           *     -       |
           */

          if (m > n)
            {
              if (x == u)
                {
                  edit(ctx, DIFF_MATCH, aoff, n);
                  edit(ctx, DIFF_INSERT, boff + (m - 1), 1);
                }
              else
                {
                  edit(ctx, DIFF_INSERT, boff, 1);
                  edit(ctx, DIFF_MATCH, aoff, n);
                }
            }
          else
            {
              if (x == u)
                {
                  edit(ctx, DIFF_MATCH, aoff, m);
                  edit(ctx, DIFF_DELETE, aoff + (n - 1), 1);
                }
              else
                {
                  edit(ctx, DIFF_DELETE, aoff, 1);
                  edit(ctx, DIFF_MATCH, aoff + 1, m);
                }
            }
        }
    }
  
  return d;
}

static ssize_t
find_middle_snake (const void * a, ssize_t aoff, ssize_t n,
                   const void * b, ssize_t boff, ssize_t m,
                   struct ctx * ctx,
                   struct middle_snake * ms)
{
  ssize_t delta, odd, mid, d;

  delta = n - m;
  odd = delta & 1;
  mid = (n + m) / 2;
  mid += odd;

  setv (ctx, 1, 0, 0);
  setv (ctx, delta - 1, 1, n);

  for (d = 0; d <= mid; d++) {
    ssize_t k, x, y;

    if ((2 * d - 1) >= ctx->dmax)
      {
        return ctx->dmax;
      }

    for (k = d; k >= -d; k -= 2)
      {
        if (k == -d || (k != d && FV(k - 1) < FV(k + 1)))
          {
            x = FV(k + 1);
          }
        else
          {
            x = FV(k - 1) + 1;
          }
        y = x - k;

        ms->x = x;
        ms->y = y;
        if (ctx->cmp)
          {
            while (x < n && y < m && ctx->cmp(ctx->idx(a, aoff + x, ctx->context),
                                              ctx->idx(b, boff + y, ctx->context), ctx->context) == 0)
              {
                x++;
                y++;
              }
          }
        else
          {
            const unsigned char *a0 = (const unsigned char *)a + aoff;
            const unsigned char *b0 = (const unsigned char *)b + boff;
            while (x < n && y < m && a0[x] == b0[y])
              {
                x++;
                y++;
              }
          }

        setv (ctx, k, 0, x);

        if (odd && k >= (delta - (d - 1)) && k <= (delta + (d - 1)))
          {
            if (x >= RV(k))
              {
                ms->u = x;
                ms->v = y;
                return 2 * d - 1;
              }
          }
      }

    for (k = d; k >= -d; k -= 2)
      {
        ssize_t kr = (n - m) + k;

        if (k == d || (k != -d && RV(kr - 1) < RV(kr + 1)))
          {
            x = RV(kr - 1);
          }
        else
          {
            x = RV(kr + 1) - 1;
          }
        y = x - kr;

        ms->u = x;
        ms->v = y;
        if (ctx->cmp)
          {
            while (x > 0 && y > 0 && ctx->cmp(ctx->idx(a, aoff + (x - 1), ctx->context),
                                              ctx->idx(b, boff + (y - 1), ctx->context), ctx->context) == 0)
              {
                x--;
                y--;
              }
          }
        else
          {
            const unsigned char *a0 = (const unsigned char *)a + aoff;
            const unsigned char *b0 = (const unsigned char *)b + boff;
            while (x > 0 && y > 0 && a0[x - 1] == b0[y - 1])
              {
                x--;
                y--;
              }
          }

        setv(ctx, kr, 1, x);

        if (!odd && kr >= -d && kr <= d)
          {
            if (x <= FV(kr))
              {
                ms->x = x;
                ms->y = y;
                return 2 * d;
              }
          }
      }
  }

  /* errno = EFAULT; */

  return -1;
}


static void
setv (struct ctx *ctx, ssize_t k, ssize_t r, ssize_t val)
{
  ssize_t j;
  ssize_t *i;

  /* Pack -N to N ssize_to 0 to N * 2
   */
  j = k <= 0 ? -k * 4 + r : k * 4 + (r - 2);

  i = (ssize_t *)ar_ref ((void **)&ctx->buf, 0, j, sizeof (ssize_t));
  *i = val;
}


static ssize_t
v(struct ctx *ctx, ssize_t k, ssize_t r)
{
  ssize_t j;

  j = k <= 0 ? -k * 4 + r : k * 4 + (r - 2);

  return *(ssize_t *)ar_ref ((void **)&ctx->buf, 0, j, sizeof (ssize_t));
}




/* arch-tag: Tom Lord Mon Jul 11 13:22:50 2005 (diff.c)
 */
