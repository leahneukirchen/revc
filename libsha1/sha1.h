/* sha1.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBSHA1__SHA1_H
#define INCLUDE__LIBSHA1__SHA1_H


#include "hackerlab/machine/types.h"



struct sha1_ctx
{
  t_uint32 A;
  t_uint32 B;
  t_uint32 C;
  t_uint32 D;
  t_uint32 E;

  t_uint32 total[2];
  t_uint32 buflen;

  char buffer[128];
};



/* automatically generated __STDC__ prototypes */
extern void sha1_buffer (t_uchar * const sha1_ret,
                         const char * const buffer,
                         size_t const len);
extern void sha1_init_ctx (struct sha1_ctx * ctx);
extern void sha1_process_bytes (struct sha1_ctx * const ctx,
                                const void * buffer,
                                size_t len);
extern void sha1_finish_ctx (t_uchar * const sha1_ret,
                             struct sha1_ctx * const ctx);
extern void sha1_to_hex (t_uchar * fourty_out, t_uchar * twenty_in);
#endif  /* INCLUDE__LIBSHA1__SHA1_H */


/* arch-tag: Tom Lord Tue Apr 19 23:15:56 2005 (sha1.h)
 */
