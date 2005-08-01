/* sha1.c: 
 *
 ****************************************************************
 * Copyright (C) 2000, 2001, 2003, 2005 Free Software Foundation, Inc.
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

/* includes */

#include "hackerlab/machine/endian.h"
#include "hackerlab/mem/mem.h"
#include "libsha1/sha1.h"


/* typedefs */




/* __STDC__ prototypes for static functions */



static void sha1_read_ctx (t_uchar * const sha1_ret,
                           const struct sha1_ctx * const ctx);
static void sha1_process_block (struct sha1_ctx * const ctx,
                                const void * const buffer,
                                size_t const len);


/* declarations */


/* NOTSWAP is a macro that does an endian swap on architectures that
 * are big-endian, as SHA needs some data in a little-endian format
 */

#if MACHINE_IS_BIGENDIAN

# define NOTSWAP(n) (n)

# define SWAP(n) \
    (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))

#else

# define NOTSWAP(n) \
    (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))

# define SWAP(n) (n)

#endif


/* code */

/* Compute MD5 message digest for LEN bytes beginning at BUFFER.  The
 * result is always in little endian byte order, so that a byte-wise
 * output yields to the wanted ASCII representation of the message
 * digest.
 */
void
sha1_buffer (t_uchar * const sha1_ret,
            const char * const buffer,
            size_t const len)
{
  struct sha1_ctx ctx;

  /* Initialize the computation context.
   */
  sha1_init_ctx (&ctx);

  /* Process whole buffer but last len % 64 bytes.
   */
  sha1_process_bytes (&ctx, buffer, len);

  /* Put result in desired memory area.
   */
  sha1_finish_ctx (sha1_ret, &ctx);
}


void
sha1_init_ctx (struct sha1_ctx * ctx)
{
  ctx->A = 0x67452301;
  ctx->B = 0xefcdab89;
  ctx->C = 0x98badcfe;
  ctx->D = 0x10325476;
  ctx->E = 0xc3d2e1f0;

  ctx->total[0] = ctx->total[1] = 0;
  ctx->buflen = 0;
}


static void
sha1_read_ctx (t_uchar * const sha1_ret,
              const struct sha1_ctx * const ctx)
{
  t_uint32 word;

  word = NOTSWAP (ctx->A);
  sha1_ret[0] = 0xff & (word >> 24);
  sha1_ret[1] = 0xff & (word >> 16);
  sha1_ret[2] = 0xff & (word >> 8);
  sha1_ret[3] = 0xff & word;

  word = NOTSWAP (ctx->B);
  sha1_ret[4] = 0xff & (word >> 24);
  sha1_ret[5] = 0xff & (word >> 16);
  sha1_ret[6] = 0xff & (word >> 8);
  sha1_ret[7] = 0xff & word;

  word = NOTSWAP (ctx->C);
  sha1_ret[8] = 0xff & (word >> 24);
  sha1_ret[9] = 0xff & (word >> 16);
  sha1_ret[10] = 0xff & (word >> 8);
  sha1_ret[11] = 0xff & word;

  word = NOTSWAP (ctx->D);
  sha1_ret[12] = 0xff & (word >> 24);
  sha1_ret[13] = 0xff & (word >> 16);
  sha1_ret[14] = 0xff & (word >> 8);
  sha1_ret[15] = 0xff & word;

  word = NOTSWAP (ctx->E);
  sha1_ret[16] = 0xff & (word >> 24);
  sha1_ret[17] = 0xff & (word >> 16);
  sha1_ret[18] = 0xff & (word >> 8);
  sha1_ret[19] = 0xff & word;
}


void
sha1_process_bytes (struct sha1_ctx * const ctx,
                   const void * buffer,
                   size_t len)
{
  /* When we already have some bits in our internal buffer concatenate
   * both inputs first.
   */
  if (ctx->buflen != 0)
    {
      size_t left_over = ctx->buflen;
      size_t add = 128 - left_over > len ? len : 128 - left_over;

      mem_move (&ctx->buffer[left_over], buffer, add);
      ctx->buflen += add;

      if (ctx->buflen > 64)
	{
	  sha1_process_block (ctx, ctx->buffer, ctx->buflen & ~63);

	  ctx->buflen &= 63;

	  /* The regions in the following copy operation cannot overlap.
           */
	  mem_move (ctx->buffer,
                    &ctx->buffer[(left_over + add) & ~63],
                    ctx->buflen);
	}

      buffer = (const char *) buffer + add;
      len -= add;
    }

  /* Process available complete blocks.
   */
  if (len >= 64)
    {
      while (len > 64)
        {
          mem_move (ctx->buffer, buffer, 64);
          sha1_process_block (ctx, ctx->buffer, 64);
          buffer = (const char *) buffer + 64;
          len -= 64;
        }
    }

  /* Move remaining bytes in internal buffer.
   */
  if (len > 0)
    {
      size_t left_over = ctx->buflen;

      mem_move (&ctx->buffer[left_over], buffer, len);
      left_over += len;
      if (left_over >= 64)
	{
	  sha1_process_block (ctx, ctx->buffer, 64);
	  left_over -= 64;
	  mem_move (ctx->buffer, &ctx->buffer[64], left_over);
	}
      ctx->buflen = left_over;
    }
}


void
sha1_finish_ctx (t_uchar * const sha1_ret,
                struct sha1_ctx * const ctx)
{
  /* This array contains the bytes used to pad the buffer to the next
   * 64-byte boundary.  (RFC 1321, 3.1: Step 1)
   */
  static const unsigned char fillbuf[64] = { 0x80, 0 /* , 0, 0, ...  */ };

  /* Take yet unprocessed bytes into account.
   */
  t_uint32 bytes = ctx->buflen;
  size_t pad;

  /* Now count remaining bytes.
   */
  ctx->total[0] += bytes;
  if (ctx->total[0] < bytes)
    ++ctx->total[1];

  pad = bytes >= 56 ? 64 + 56 - bytes : 56 - bytes;
  mem_move (&ctx->buffer[bytes], fillbuf, pad);

  /* Put the 64-bit file length in *bits* at the end of the buffer.
   */
  *(t_uint32 *) &ctx->buffer[bytes + pad + 4] = NOTSWAP (ctx->total[0] << 3);
  *(t_uint32 *) &ctx->buffer[bytes + pad] = NOTSWAP ((ctx->total[1] << 3) |
						    (ctx->total[0] >> 29));

  sha1_process_block (ctx, ctx->buffer, bytes + pad + 8);

  sha1_read_ctx (sha1_ret, ctx);
}


/* SHA1 round constants
 */
#define K1 0x5a827999L
#define K2 0x6ed9eba1L
#define K3 0x8f1bbcdcL
#define K4 0xca62c1d6L

/* Round functions.  Note that F2 is the same as F4.
 */
#define F1(B,C,D) ( D ^ ( B & ( C ^ D ) ) )
#define F2(B,C,D) (B ^ C ^ D)
#define F3(B,C,D) ( ( B & C ) | ( D & ( B | C ) ) )
#define F4(B,C,D) (B ^ C ^ D)

/* Process LEN bytes of BUFFER, accumulating context into CTX.
 * It is assumed that LEN % 64 == 0.
 * Most of this code comes from GnuPG's cipher/sha1.c.
 */

static void
sha1_process_block (struct sha1_ctx * const ctx,
                   const void * const buffer,
                   size_t const len)
{
  const t_uint32 *words = buffer;
  size_t nwords = len / sizeof (t_uint32);
  const t_uint32 * endp = words + nwords;
  t_uint32 x[16];
  t_uint32 a = ctx->A;
  t_uint32 b = ctx->B;
  t_uint32 c = ctx->C;
  t_uint32 d = ctx->D;
  t_uint32 e = ctx->E;

  /* First increment the byte count.  RFC 1321 specifies the possible
     length of the file up to 2^64 bits.  Here we only compute the
     number of bytes.  Do a double word increment.  */
  ctx->total[0] += len;
  if (ctx->total[0] < len)
    ++ctx->total[1];

#define rol(x,n) ( ((x) << (n)) | ((x) >> (32-(n))) )

#define M(I) ( tm =   x[I&0x0f] ^ x[(I-14)&0x0f] \
		    ^ x[(I-8)&0x0f] ^ x[(I-3)&0x0f] \
	       , (x[I&0x0f] = rol(tm, 1)) )

#define R(A,B,C,D,E,F,K,M)  do { E += rol( A, 5 )     \
				      + F( B, C, D )  \
				      + K	      \
				      + M;	      \
				 B = rol( B, 30 );    \
			       } while(0)

  while (words < endp)
    {
      t_uint32 tm;
      int t;
      /* FIXME: see sha1.c for a better implementation.  */
      for (t = 0; t < 16; t++)
	{
	  x[t] = NOTSWAP (*words);
	  words++;
	}

      R( a, b, c, d, e, F1, K1, x[ 0] );
      R( e, a, b, c, d, F1, K1, x[ 1] );
      R( d, e, a, b, c, F1, K1, x[ 2] );
      R( c, d, e, a, b, F1, K1, x[ 3] );
      R( b, c, d, e, a, F1, K1, x[ 4] );
      R( a, b, c, d, e, F1, K1, x[ 5] );
      R( e, a, b, c, d, F1, K1, x[ 6] );
      R( d, e, a, b, c, F1, K1, x[ 7] );
      R( c, d, e, a, b, F1, K1, x[ 8] );
      R( b, c, d, e, a, F1, K1, x[ 9] );
      R( a, b, c, d, e, F1, K1, x[10] );
      R( e, a, b, c, d, F1, K1, x[11] );
      R( d, e, a, b, c, F1, K1, x[12] );
      R( c, d, e, a, b, F1, K1, x[13] );
      R( b, c, d, e, a, F1, K1, x[14] );
      R( a, b, c, d, e, F1, K1, x[15] );
      R( e, a, b, c, d, F1, K1, M(16) );
      R( d, e, a, b, c, F1, K1, M(17) );
      R( c, d, e, a, b, F1, K1, M(18) );
      R( b, c, d, e, a, F1, K1, M(19) );
      R( a, b, c, d, e, F2, K2, M(20) );
      R( e, a, b, c, d, F2, K2, M(21) );
      R( d, e, a, b, c, F2, K2, M(22) );
      R( c, d, e, a, b, F2, K2, M(23) );
      R( b, c, d, e, a, F2, K2, M(24) );
      R( a, b, c, d, e, F2, K2, M(25) );
      R( e, a, b, c, d, F2, K2, M(26) );
      R( d, e, a, b, c, F2, K2, M(27) );
      R( c, d, e, a, b, F2, K2, M(28) );
      R( b, c, d, e, a, F2, K2, M(29) );
      R( a, b, c, d, e, F2, K2, M(30) );
      R( e, a, b, c, d, F2, K2, M(31) );
      R( d, e, a, b, c, F2, K2, M(32) );
      R( c, d, e, a, b, F2, K2, M(33) );
      R( b, c, d, e, a, F2, K2, M(34) );
      R( a, b, c, d, e, F2, K2, M(35) );
      R( e, a, b, c, d, F2, K2, M(36) );
      R( d, e, a, b, c, F2, K2, M(37) );
      R( c, d, e, a, b, F2, K2, M(38) );
      R( b, c, d, e, a, F2, K2, M(39) );
      R( a, b, c, d, e, F3, K3, M(40) );
      R( e, a, b, c, d, F3, K3, M(41) );
      R( d, e, a, b, c, F3, K3, M(42) );
      R( c, d, e, a, b, F3, K3, M(43) );
      R( b, c, d, e, a, F3, K3, M(44) );
      R( a, b, c, d, e, F3, K3, M(45) );
      R( e, a, b, c, d, F3, K3, M(46) );
      R( d, e, a, b, c, F3, K3, M(47) );
      R( c, d, e, a, b, F3, K3, M(48) );
      R( b, c, d, e, a, F3, K3, M(49) );
      R( a, b, c, d, e, F3, K3, M(50) );
      R( e, a, b, c, d, F3, K3, M(51) );
      R( d, e, a, b, c, F3, K3, M(52) );
      R( c, d, e, a, b, F3, K3, M(53) );
      R( b, c, d, e, a, F3, K3, M(54) );
      R( a, b, c, d, e, F3, K3, M(55) );
      R( e, a, b, c, d, F3, K3, M(56) );
      R( d, e, a, b, c, F3, K3, M(57) );
      R( c, d, e, a, b, F3, K3, M(58) );
      R( b, c, d, e, a, F3, K3, M(59) );
      R( a, b, c, d, e, F4, K4, M(60) );
      R( e, a, b, c, d, F4, K4, M(61) );
      R( d, e, a, b, c, F4, K4, M(62) );
      R( c, d, e, a, b, F4, K4, M(63) );
      R( b, c, d, e, a, F4, K4, M(64) );
      R( a, b, c, d, e, F4, K4, M(65) );
      R( e, a, b, c, d, F4, K4, M(66) );
      R( d, e, a, b, c, F4, K4, M(67) );
      R( c, d, e, a, b, F4, K4, M(68) );
      R( b, c, d, e, a, F4, K4, M(69) );
      R( a, b, c, d, e, F4, K4, M(70) );
      R( e, a, b, c, d, F4, K4, M(71) );
      R( d, e, a, b, c, F4, K4, M(72) );
      R( c, d, e, a, b, F4, K4, M(73) );
      R( b, c, d, e, a, F4, K4, M(74) );
      R( a, b, c, d, e, F4, K4, M(75) );
      R( e, a, b, c, d, F4, K4, M(76) );
      R( d, e, a, b, c, F4, K4, M(77) );
      R( c, d, e, a, b, F4, K4, M(78) );
      R( b, c, d, e, a, F4, K4, M(79) );

      a = ctx->A += a;
      b = ctx->B += b;
      c = ctx->C += c;
      d = ctx->D += d;
      e = ctx->E += e;
    }
}




void
sha1_to_hex (t_uchar * fourty_out, t_uchar * twenty_in)
{
  int in;
  int out;

  for ((in = 0), (out = 0);
       in < 20;
       (in += 4), (out += 8))
    {
      int b0 = twenty_in[in];
      int b1 = twenty_in[in + 1];
      int b2 = twenty_in[in + 2];
      int b3 = twenty_in[in + 3];

#define hex_digit(n) (((n) >= 10) ? ('a' + ((n) - 10)) : ('0' + (n)))

      fourty_out[out] = hex_digit ((b3 >> 4) & 0xf);
      fourty_out[out + 1] = hex_digit (b3 & 0xf);
      fourty_out[out + 2] = hex_digit ((b2 >> 4) & 0xf);
      fourty_out[out + 3] = hex_digit (b2 & 0xf);
      fourty_out[out + 4] = hex_digit ((b1 >> 4) & 0xf);
      fourty_out[out + 5] = hex_digit (b1 & 0xf);
      fourty_out[out + 6] = hex_digit ((b0 >> 4) & 0xf);
      fourty_out[out + 7] = hex_digit (b0 & 0xf);
    }
}





/* arch-tag: Tom Lord Tue Apr 19 23:15:47 2005 (sha1.c)
 */
