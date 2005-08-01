/* md5.c - MD5 Message-Digest Algorithm
 * Copyright (C) 1995, 1996, 1998, 1999,
 *               2000, 2001 Free Software Foundation, Inc.
 *
 * Modifications Copyright (C) 2003, Thomas Lord
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * According to the definition of MD5 in RFC 1321 from April 1992.
 * NOTE: This is *not* the same file as the one from glibc.
 * NOTE: This is *not* the same file as the one from gnupg.
 */

/* Written by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1995.  */
/* Heavily modified for GnuPG by <wk@gnupg.org> */
/* Modified for hackerlab by <lord@emf.net> aka <lord@gnu.org> */




#include "hackerlab/bugs/panic.h"
#include "hackerlab/machine/endian.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/hash/md5.h"


/************************************************************************
 *(h1 "MD5 Routines")
 * 
 * The md5 routines allow you to compute an MD5 message digest
 * According to the definition of MD5 in RFC 1321 from April 1992.
 * 
 */


struct md5_context
{
  /* chaining variables:
   */
  t_uint32 A;
  t_uint32 B;
  t_uint32 C;
  t_uint32 D;


  t_uint32  nblocks;
  t_uchar buf[64];
  int  count;
};



/* __STDC__ prototypes for static functions */
static void transform (md5_context_t ctx, t_uchar * data);



/*(c make_md5_context)
 * md5_context_t make_md5_context (alloc_limits limits);
 * 
 * Allocate and initialize an object which will keep track of the
 * state of an md5 digest computation.
 */
md5_context_t
make_md5_context (alloc_limits limits)
{
  md5_context_t ctx = 0;

  ctx = lim_malloc (limits, sizeof (*ctx));
  md5_context_reset (ctx);
  return ctx;
}


/*(c md5_context_reset)
 * void md5_context_reset (md5_context_t ctx);
 * 
 * Reinitialize an md5 state object.  This will 
 * undo the effects of any previous calls to
 * `md5_scan'.
 */
void
md5_context_reset (md5_context_t ctx)
{
  if (ctx)
    {
      ctx->A = 0x67452301;
      ctx->B = 0xefcdab89;
      ctx->C = 0x98badcfe;
      ctx->D = 0x10325476;

      ctx->nblocks = 0;
      ctx->count = 0;
    }
}


/*(c free_md5_context)
 * void free_md5_context (alloc_limits limits, md5_context_t ctx);
 * 
 * Free all resources associated with an md5 state object.
 */
void
free_md5_context (alloc_limits limits, md5_context_t ctx)
{
  lim_free (limits, ctx);
}



/*(c md5_scan)
 * void md5_scan (md5_context_t hd, t_uchar * inbuf, size_t inlen);
 * 
 * Scan the next `inlen' bytes of `inbuf', treating them as subsequent
 * bytes in a message for which we are computing an md5 digest.
 * 
 * This function may be called repeatedly on sequential ``bursts'' 
 * of a total message.
 */
void
md5_scan (md5_context_t hd, t_uchar * inbuf, size_t inlen)
{
  if (hd->count == 64)
    {
      /* process and flush the buffer
       */
      transform (hd, hd->buf);
      hd->count = 0;
      hd->nblocks++;
    }

  if (!inlen)
    return;

  if (hd->count)
    {
      /* fill a partial buffer, 
       * process and flush it.
       */
      while (inlen && (hd->count < 64))
        {
          hd->buf[hd->count] = *inbuf;

          hd->count++;
          inbuf++;
          inlen--;
        }
      md5_scan (hd, 0, 0);
    }

  /* process inbuf in 64 byte chunks
   *
   * (doesn't bother copying chunks to the buffer in hd)
   */
  while (inlen >= 64)
    {
      transform (hd, inbuf);

      hd->count = 0;
      hd->nblocks++;
      inlen -= 64;
      inbuf += 64;
    }

  /* buffer up any fragment of a remaining chunk
   */
  while (inlen && (hd->count < 64))
    {
      hd->buf[hd->count] = *inbuf;

      hd->count++;
      inbuf++;
      inlen--;
    }
}


/*(c md5_final)
 * void md5_final (t_uchar * result, md5_context_t state);
 * 
 * Declare that a complete message has been scanned using
 * `state' and `md5_scan()'.
 * 
 * Return the 16-byte md5 digest in `result', which must point to
 * storage for at least 16 bytes.
 * 
 * As a side-effect, `state' is reinitialized and may be used
 * again with `md5_scan ()' to process a new message.
 */
void
md5_final (t_uchar * result, md5_context_t hd)
{
  t_uint32 t;
  t_uint32 msb;
  t_uint32 lsb;

  /* flush teh buffer if it's full.
   */
  md5_scan (hd, 0, 0);

  t = hd->nblocks;

  /* multiply by 64 to make a byte count
   * for the blocks processed so far.
   */
  lsb = t << 6;
  msb = t >> 26;

  /* add the count of still-buffered 
   * bytes (there are < 64).
   * 
   * [Fake 64 bit integer math.... this could be reasonably fixed in
   * hackerlab but I don't want to perturb the inherited code that
   * much right now.]
   */

  t = lsb;
  lsb += hd->count;
  if (lsb < t )
    msb++;

  /* multiply by 8 to make a bit count
   */
  t = lsb;
  lsb <<= 3;
  msb <<= 3;
  msb |= t >> 29;

  /* Need to append some padding and the bit count to the message and
   * add it to the md5sum.  The total resulting message has to come
   * out to a multiple of 64 bytes.
   *
   * It'll take 8 bytes to hold the length and we need a minimum
   * of one padding byte.
   */

  if (hd->count < 56)
    {
      /* enough room for the mandatory padding byte
       */
      hd->buf[hd->count] = 0x80;
      hd->count++;

      /* add optional padding bytes if needed.
       */
      while (hd->count < 56)
        {
          hd->buf[hd->count] = 0;
          hd->count++;
        }
    }
  else
    {
      /* need one extra block
       * 
       * Mandatory padding byte:
       */
      hd->buf[hd->count] = 0x80;
      hd->count++;

      /* fill out this too-small black with optional
       * padding bytes.
       */
      while (hd->count < 64)
        {
          hd->buf[hd->count] = 0;
          hd->count++;
        }
  
      /* scan the last block and pad out the next:
       */

      md5_scan (hd, 0, 0);
      
      mem_set0 (hd->buf, (size_t)56);
      hd->count = 56;
    }

  invariant (hd->count == 56);

  /* append the 64-bit bit count
   */
  hd->buf[56] = (t_uchar)(lsb & 0xff);
  hd->buf[57] = (t_uchar)((lsb >>  8) & 0xff);
  hd->buf[58] = (t_uchar)((lsb >> 16) & 0xff);
  hd->buf[59] = (t_uchar)((lsb >> 24) & 0xff);
  hd->buf[60] = (t_uchar)(msb & 0xff);
  hd->buf[61] = (t_uchar)((msb >>  8) & 0xff);
  hd->buf[62] = (t_uchar)((msb >> 16) & 0xff);
  hd->buf[63] = (t_uchar)((msb >> 24) & 0xff);

  transform (hd, hd->buf);

#define X(a) \
  do { \
       *result++ = (t_uchar)(hd-> a & 0xff); \
       *result++ = (t_uchar)((hd-> a >> 8) & 0xff); \
       *result++ = (t_uchar)((hd-> a >> 16) & 0xff); \
       *result++ = (t_uchar)((hd-> a >> 24) & 0xff); \
     } while(0)

  X(A);
  X(B);
  X(C);
  X(D);
#undef X

  md5_context_reset (hd);
}





/*(c md5_alloc_ascii)
 * t_uchar * md5_alloc_ascii (alloc_limits limits, t_uchar * result);
 * 
 * Return a newly allocated 33-byte 0-terminated ascii string
 * containing a hexadecimal version of the 16-byte binary md5 sum
 * pointed to by `result'.
 */
t_uchar *
md5_alloc_ascii (alloc_limits limits, t_uchar * result)
{
  t_uchar * answer = 0;

  answer = lim_malloc (limits, 33);
  if (!answer)
    return 0;

  answer[32] = 0;

  md5_ascii (answer, result);

  return answer;
}


/*(c md5_ascii)
 * void md5_ascii (t_uchar * answer, t_uchar * result);
 * 
 * Format a 32-byte ascii string containing a hexadecimal version of
 * the 16-byte binary md5 sum pointed to by `result'.
 * 
 * This function does not add a final 0-byte to the string.
 */
void
md5_ascii (t_uchar * answer, t_uchar * result)
{
  int x;

  for (x = 0; x < 16; ++x)
    {
      int hi = (0xf & (result[x] >> 4));
      int lo = (0xf & result[x]);

      answer[2 * x] = ((hi >= 10) ? ('a' + (hi - 10)) : ('0' + hi));
      answer[2 * x + 1] = ((lo >= 10) ? ('a' + (lo - 10)) : ('0' + lo));
    }
}




/* These are the four functions used in the four steps of the MD5
 * algorithm and defined in the RFC 1321.  The first function is a
 * little bit optimized (as found in Colin Plumbs public domain
 * implementation).
 */

/* #define FF(b, c, d) ((b & c) | (~b & d)) */
#define FF(b, c, d) (d ^ (b & (c ^ d)))
#define FG(b, c, d) FF (d, b, c)
#define FH(b, c, d) (b ^ c ^ d)
#define FI(b, c, d) (c ^ (b | ~d))



/****************
 * transform n*64 bytes
 */
static void
transform (md5_context_t ctx, t_uchar * data)
{
  t_uint32 correct_words[16];
  t_uint32 A = ctx->A;
  t_uint32 B = ctx->B;
  t_uint32 C = ctx->C;
  t_uint32 D = ctx->D;
  t_uint32 * cwp = correct_words;

#if MACHINE_IS_BIGENDIAN
  {
    int i;
    t_uchar * p2;
    t_uchar * p1;

    i = 0;
    p1 = data;
    p2 = (t_uchar*)correct_words;

    while (i < 16)
      {
        p2[3] = *p1++;
        p2[2] = *p1++;
        p2[1] = *p1++;
        p2[0] = *p1++;

        ++i;
        p2 += 4;
      }
  }
#else
  mem_move ((t_uchar *)correct_words, (t_uchar *)data, (size_t)64);
#endif


  /* Rotate a 32 bit integer by n bytes
   */
#define rol(x,n) ( ((x) << (n)) | ((x) >> (32-(n))) )

#define OP(a, b, c, d, s, T)                                        \
  do                                                                \
    {                                                               \
      a += FF (b, c, d) + (*cwp++) + T;                             \
      a = rol(a, s);                                                \
      a += b;                                                       \
    }                                                               \
  while (0)

  /* Before we start, one word about the strange constants.
   * They are defined in RFC 1321 as
   *
   * T[i] = (int) (4294967296.0 * fabs (sin (i))), i=1..64
   */

  /* Round 1.  */
  OP (A, B, C, D,  7, 0xd76aa478);
  OP (D, A, B, C, 12, 0xe8c7b756);
  OP (C, D, A, B, 17, 0x242070db);
  OP (B, C, D, A, 22, 0xc1bdceee);
  OP (A, B, C, D,  7, 0xf57c0faf);
  OP (D, A, B, C, 12, 0x4787c62a);
  OP (C, D, A, B, 17, 0xa8304613);
  OP (B, C, D, A, 22, 0xfd469501);
  OP (A, B, C, D,  7, 0x698098d8);
  OP (D, A, B, C, 12, 0x8b44f7af);
  OP (C, D, A, B, 17, 0xffff5bb1);
  OP (B, C, D, A, 22, 0x895cd7be);
  OP (A, B, C, D,  7, 0x6b901122);
  OP (D, A, B, C, 12, 0xfd987193);
  OP (C, D, A, B, 17, 0xa679438e);
  OP (B, C, D, A, 22, 0x49b40821);

#undef OP
#define OP(f, a, b, c, d, k, s, T)  \
  do                                                          \
    {                                                         \
      a += f (b, c, d) + correct_words[k] + T;                \
      a = rol(a, s);                                          \
      a += b;                                                 \
    }                                                         \
  while (0)

  /* Round 2.  */
  OP (FG, A, B, C, D,  1,  5, 0xf61e2562);
  OP (FG, D, A, B, C,  6,  9, 0xc040b340);
  OP (FG, C, D, A, B, 11, 14, 0x265e5a51);
  OP (FG, B, C, D, A,  0, 20, 0xe9b6c7aa);
  OP (FG, A, B, C, D,  5,  5, 0xd62f105d);
  OP (FG, D, A, B, C, 10,  9, 0x02441453);
  OP (FG, C, D, A, B, 15, 14, 0xd8a1e681);
  OP (FG, B, C, D, A,  4, 20, 0xe7d3fbc8);
  OP (FG, A, B, C, D,  9,  5, 0x21e1cde6);
  OP (FG, D, A, B, C, 14,  9, 0xc33707d6);
  OP (FG, C, D, A, B,  3, 14, 0xf4d50d87);
  OP (FG, B, C, D, A,  8, 20, 0x455a14ed);
  OP (FG, A, B, C, D, 13,  5, 0xa9e3e905);
  OP (FG, D, A, B, C,  2,  9, 0xfcefa3f8);
  OP (FG, C, D, A, B,  7, 14, 0x676f02d9);
  OP (FG, B, C, D, A, 12, 20, 0x8d2a4c8a);

  /* Round 3.  */
  OP (FH, A, B, C, D,  5,  4, 0xfffa3942);
  OP (FH, D, A, B, C,  8, 11, 0x8771f681);
  OP (FH, C, D, A, B, 11, 16, 0x6d9d6122);
  OP (FH, B, C, D, A, 14, 23, 0xfde5380c);
  OP (FH, A, B, C, D,  1,  4, 0xa4beea44);
  OP (FH, D, A, B, C,  4, 11, 0x4bdecfa9);
  OP (FH, C, D, A, B,  7, 16, 0xf6bb4b60);
  OP (FH, B, C, D, A, 10, 23, 0xbebfbc70);
  OP (FH, A, B, C, D, 13,  4, 0x289b7ec6);
  OP (FH, D, A, B, C,  0, 11, 0xeaa127fa);
  OP (FH, C, D, A, B,  3, 16, 0xd4ef3085);
  OP (FH, B, C, D, A,  6, 23, 0x04881d05);
  OP (FH, A, B, C, D,  9,  4, 0xd9d4d039);
  OP (FH, D, A, B, C, 12, 11, 0xe6db99e5);
  OP (FH, C, D, A, B, 15, 16, 0x1fa27cf8);
  OP (FH, B, C, D, A,  2, 23, 0xc4ac5665);

  /* Round 4.  */
  OP (FI, A, B, C, D,  0,  6, 0xf4292244);
  OP (FI, D, A, B, C,  7, 10, 0x432aff97);
  OP (FI, C, D, A, B, 14, 15, 0xab9423a7);
  OP (FI, B, C, D, A,  5, 21, 0xfc93a039);
  OP (FI, A, B, C, D, 12,  6, 0x655b59c3);
  OP (FI, D, A, B, C,  3, 10, 0x8f0ccc92);
  OP (FI, C, D, A, B, 10, 15, 0xffeff47d);
  OP (FI, B, C, D, A,  1, 21, 0x85845dd1);
  OP (FI, A, B, C, D,  8,  6, 0x6fa87e4f);
  OP (FI, D, A, B, C, 15, 10, 0xfe2ce6e0);
  OP (FI, C, D, A, B,  6, 15, 0xa3014314);
  OP (FI, B, C, D, A, 13, 21, 0x4e0811a1);
  OP (FI, A, B, C, D,  4,  6, 0xf7537e82);
  OP (FI, D, A, B, C, 11, 10, 0xbd3af235);
  OP (FI, C, D, A, B,  2, 15, 0x2ad7d2bb);
  OP (FI, B, C, D, A,  9, 21, 0xeb86d391);

  /* Put checksum in context given as argument.
   */
  ctx->A += A;
  ctx->B += B;
  ctx->C += C;
  ctx->D += D;
}



/* tag: Tom Lord Fri Oct 24 08:45:11 2003 (md5.c)
 */
