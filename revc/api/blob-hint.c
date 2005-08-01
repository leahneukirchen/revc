/* blob-hint.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/char/char-class.h"
#include "hackerlab/char/str.h"
#include "revc/api/blob-hint.h"

void
revc_blob_hint (t_uint * const a_byte_ret,
                t_uint * const a_mask_ret,
                t_uint * const b_byte_ret,
                t_uint * const b_mask_ret,
                const t_uchar * const blob)
{
  t_uchar key[7];
  t_uchar bytes[3];
  t_uint a_bits;
  t_uint b_bits;
  
  mem_set0 (key, sizeof (key));
  str_cpy_n (key, blob, sizeof (key) - 1);

  bytes[0] = (char_digit_value(key[0]) << 4) | char_digit_value(key[1]);
  bytes[1] = (char_digit_value(key[2]) << 4) | char_digit_value(key[3]);
  bytes[2] = (char_digit_value(key[4]) << 4) | char_digit_value(key[5]);

  a_bits = (bytes[0] << 2) | (bytes[1] >> 6);
  b_bits = ((bytes[1] & 0x3f) << 4) | (bytes[2] >> 4);

  if (a_byte_ret)
    *a_byte_ret = (a_bits >> 3);
  if (a_mask_ret)
    switch (a_bits & 7)
      {
      case 0: *a_mask_ret = 1; break;
      case 1: *a_mask_ret = 2; break;
      case 2: *a_mask_ret = 4; break;
      case 3: *a_mask_ret = 8; break;
      case 4: *a_mask_ret = 16; break;
      case 5: *a_mask_ret = 32; break;
      case 6: *a_mask_ret = 64; break;
      case 7: *a_mask_ret = 128; break;
      }

  if (b_byte_ret)
    *b_byte_ret = (128 + (b_bits >> 3));
  if (b_mask_ret)
    switch (b_bits & 7)
      {
      case 0: *b_mask_ret = 1; break;
      case 1: *b_mask_ret = 2; break;
      case 2: *b_mask_ret = 4; break;
      case 3: *b_mask_ret = 8; break;
      case 4: *b_mask_ret = 16; break;
      case 5: *b_mask_ret = 32; break;
      case 6: *b_mask_ret = 64; break;
      case 7: *b_mask_ret = 128; break;
      }
}



/* arch-tag: Tom Lord Sun Jul  3 07:35:56 2005 (blob-hint.c)
 */
