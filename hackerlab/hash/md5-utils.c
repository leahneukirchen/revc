/* md5-utils.c: 
 *
 ****************************************************************
 * Copyright (C) 2003 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/hash/md5-utils.h"


/************************************************************************
 *(h1 "MD5 Convenience Functions")
 * 
 * 
 * 
 */


/*(c md5_ascii_for_str)
 * t_uchar * md5_ascii_for_str (alloc_limits limits,
 *                              t_uchar * string);
 * 
 * Return a 0-terminated (33 bytes, total) string containing
 * a hexadecimal expression of the md5 hash of `string'.
 * 
 * This procedure internally allocates and frees memory using
 * `limits'.
 */
t_uchar *
md5_ascii_for_str (alloc_limits limits,
                   t_uchar * string)
{
  return md5_ascii_for_str_n (limits, string, str_length (string));
}


/*(c md5_ascii_for_str_n)
 * t_uchar * md5_ascii_for_str_n (alloc_limits limits,
 *                                t_uchar * string,
 *                                size_t length);
 * 
 * Return a 0-terminated (33 bytes, total) string containing a
 * hexadecimal expression of the md5 hash of the `length'-byte
 * `string'.
 * 
 * This procedure internally allocates and frees memory using
 * `limits'.
 */
t_uchar *
md5_ascii_for_str_n (alloc_limits limits,
                     t_uchar * string,
                     size_t length)
{
  t_uchar binary[16];

  md5_for_str_n (binary, limits, string, length);
  return md5_alloc_ascii (limits, binary);
}


/*(c md5_for_str)
 * void md5_for_str (t_uchar * result,
 *                   alloc_limits limits,
 *                   t_uchar * string);
 * 
 * Fill the 16-byte array `result' with the (binary) md5 
 * hash of `string'.
 * 
 * This procedure internally allocates and frees memory using
 * `limits'.
 */
void
md5_for_str (t_uchar * result,
             alloc_limits limits,
             t_uchar * string)
{
  md5_for_str_n (result, limits, string, str_length (string));
}


/*(c md5_for_str_n)
 * void md5_for_str_n (t_uchar * result,
 *                     alloc_limits limits,
 *                     t_uchar * string,
 *                     size_t length);
 * 
 * Fill the 16-byte array `result' with the (binary) md5 
 * hash of the `length'-byte `string'.
 * 
 * This procedure internally allocates and frees memory using
 * `limits'.
 */
void
md5_for_str_n (t_uchar * result,
               alloc_limits limits,
               t_uchar * string,
               size_t length)
{
  md5_context_t context;

  context = make_md5_context (limits);
  md5_scan (context, string, length);
  md5_final (result, context);
}



/* tag: Tom Lord Sun Dec 28 11:37:01 2003 (md5-utils.c)
 */
