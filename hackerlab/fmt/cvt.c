/* cvt.c - converting integers to and from strings
 *
 ****************************************************************
 * Copyright (C) 1998, 2000 Thomas Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include "hackerlab/os/errno.h"
#include "hackerlab/char/char-class.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fmt/cvt.h"


/************************************************************************
 *(h1 "Converting Integers to and from Strings"
 *    :include ("hackerlab/fmt/cvt.h"))
 *
 * The functions in this chapter convert between integer types and
 * strings.
 */
/*(menu)
 */

/************************************************************************
 *(h2 "Converting Integers to Strings")
 * 
 */

/*(c cvt_ulong_to_decimal)
 * size_t cvt_ulong_to_decimal (t_uchar * nbuf, unsigned long n);
 * 
 * Convert `n' to a 0-terminated decimal number.
 *
 * Return the number of digits in the output.
 */
size_t
cvt_ulong_to_decimal (t_uchar * nbuf, unsigned long n)
{
  size_t x;
  size_t y;
  size_t used;

  if (!n)
    {
      nbuf[0] = '0';
      nbuf[1] = 0;
      return 1;
    }

  x = 0;
  while (n)
    {
      nbuf[x] = '0' + n % 10;
      n /= 10;
      x += 1;
    }
  nbuf[x] = 0;
  y = 0;
  used = x--;
  while (x > y)
    {
      int c;
      c = nbuf[x];
      nbuf[x] = nbuf[y];
      nbuf[y] = c;
      --x;
      ++y;
    }
  return used;
}

/*(c cvt_ulong_to_decimal_n)
 * int cvt_ulong_to_decimal_n (t_uchar * nbuf,
 *                             size_t buflen, 
 *                             unsigned long n,
 *                             size_t * needed);
 *
 * Convert `n' to a 0-terminated decimal number written to `nbuf' with
 * has the size of `buflen'.  Stores the size which would be needed by
 * the conversion in `*needed' if it is not null (even on failure).
 *
 * Return 0 on success and -1 if the buffer is too small.
 */
int
cvt_ulong_to_decimal_n (t_uchar * nbuf, size_t buflen, unsigned long n, size_t * needed)
{
  size_t used;
  t_uchar tmp[sizeof (unsigned long)*2 + (sizeof (unsigned long) + 1)/2 + 1];

  used = cvt_ulong_to_decimal (tmp, n);

  if (needed)
    *needed = used;

  if (used >= buflen)
    return -1;

  (void) str_cpy (nbuf, tmp);
  return 0;
}


/*(c cvt_long_to_decimal)
 * void cvt_long_to_decimal (t_uchar * nbuf, long n);
 * 
 * Convert `n' to a 0-terminated decimal number.
 *
 * Return the number of digits in the output.
 */
size_t
cvt_long_to_decimal (t_uchar * nbuf, long n)
{
  if (n < 0)
    {
      nbuf[0] = '-';
      return 1 + cvt_ulong_to_decimal (nbuf + 1, -n);
    }
  else
    return cvt_ulong_to_decimal (nbuf, n);
}


/*(c cvt_long_to_decimal_n)
 * int cvt_long_to_decimal_n (t_uchar * nbuf,
 *                            size_t buflen,
 *                            unsigned long n,
 *                            size_t * needed);
 *
 * Convert `n' to a 0-terminated decimal number written to `nbuf' with
 * has the size of `buflen'.  Stores the size which would be needed by
 * the conversion in `*needed' if it is not null (even on failure).
 *
 * Return 0 on success and -1 if the buffer is too small.
 */
int
cvt_long_to_decimal_n (t_uchar * nbuf, size_t buflen, long n, size_t * needed)
{
  size_t used;
  t_uchar tmp[sizeof (unsigned long)*2 + (sizeof (unsigned long) + 1)/2 + 2];

  if (n < 0)
    {
      tmp[0] = '-';
      used =  1 + cvt_ulong_to_decimal (tmp + 1, -n);
    }
  else
    used = cvt_ulong_to_decimal (tmp, n);

  if (needed)
    *needed = used;

  if (used >= buflen)
    return -1;

  (void) str_cpy (nbuf, tmp);
  return 0;
}


/*(c cvt_ulong_to_octal)
 * size_t cvt_ulong_to_octal (t_uchar * nbuf, unsigned long n);
 * 
 * Convert `n' to a 0-terminated octal number.
 */
size_t
cvt_ulong_to_octal (t_uchar * nbuf, unsigned long n)
{
  size_t x;
  size_t y;
  size_t used;

  if (!n)
    {
      nbuf[0] = '0';
      nbuf[1] = 0;
      return 1;
    }

  x = 0;
  while (n)
    {
      nbuf[x] = '0' + n % 8;
      n /= 8;
      x += 1;
    }
  nbuf[x] = 0;
  y = 0;
  used = x--;
  while (x > y)
    {
      int c;
      c = nbuf[x];
      nbuf[x] = nbuf[y];
      nbuf[y] = c;
      --x;
      ++y;
    }
  return used;
}


/*(c cvt_ulong_to_octal_n)
 * int cvt_ulong_to_octal_n (t_uchar * nbuf,
 *                           size_t buflen,
 *                           unsigned long n,
 *                           size_t * needed);
 *
 * Convert `n' to a 0-terminated octal number written to `nbuf' with
 * has the size of `buflen'.  Stores the size which would be needed by
 * the conversion in `*needed' if it is not null (even on failure).
 *
 * Return 0 on success and -1 if the buffer is too small
 */
int
cvt_ulong_to_octal_n (t_uchar * nbuf, size_t buflen, unsigned long n, size_t * needed)
{
  size_t used;
  t_uchar tmp[(sizeof (unsigned long)*8 + 2)/3 + 1];

  used = cvt_ulong_to_octal (tmp, n);

  if (needed)
    *needed = used;

  if (used >= buflen)
    return -1;

  (void) str_cpy (nbuf, tmp);
  return 0;
}


/*(c cvt_long_to_octal)
 * void cvt_long_to_octal (t_uchar * nbuf, long n);
 * 
 * Convert `n' to a 0-terminated octal number.
 *
 * Return the number of digits in the output.
 */
size_t
cvt_long_to_octal (t_uchar * nbuf, long n)
{
  if (n < 0)
    {
      nbuf[0] = '-';
      return 1 + cvt_ulong_to_octal (nbuf + 1, -n);
    }
  else
    return cvt_ulong_to_octal (nbuf, n);
}


/*(c cvt_long_to_octal_n)
 * int cvt_long_to_octal_n (t_uchar * nbuf,
 *                          size_t buflen,
 *                          unsigned long n,
 *                          size_t * needed);
 *
 * Convert `n' to a 0-terminated octal number written to `nbuf' with
 * has the size of `buflen'.  Stores the size which would be needed by
 * the conversion in `*needed' if it is not null (even on failure).
 *
 * Return 0 on success and -1 if the buffer is too small.
 */
int
cvt_long_to_octal_n (t_uchar * nbuf, size_t buflen, long n, size_t * needed)
{
  size_t used;
  t_uchar tmp[(sizeof (unsigned long)*8 + 2)/3 + 2];

  if (n < 0)
    {
      tmp[0] = '-';
      used = 1 + cvt_ulong_to_octal (tmp + 1, -n);
    }
  else
    used = cvt_ulong_to_octal (tmp, n);

  if (needed)
    *needed = used;

  if (used >= buflen)
    return -1;

  (void) str_cpy (nbuf, tmp);
  return 0;
}


/*(c cvt_ulong_to_HEX)
 * size_t cvt_ulong_to_HEX (t_uchar * nbuf, unsigned long n);
 *
 * Convert `n' to a 0-terminated hexadecimal number using upper-case
 * hex digits "A..F".
 * 
 * Return the number of digits in the output.
 */
size_t
cvt_ulong_to_HEX (t_uchar * nbuf, unsigned long n)
{
  static const t_uchar * HEX = "0123456789ABCDEF";
  size_t x;
  size_t y;
  size_t needed;

  if (!n)
    {
      nbuf[0] = '0';
      nbuf[1] = 0;
      return 1;
    }

  x = 0;
  while (n)
    {
      nbuf[x] = HEX[n % 16];
      n /= 16;
      x += 1;
    }
  nbuf[x] = 0;
  y = 0;
  needed = x--;
  while (x > y)
    {
      int c;
      c = nbuf[x];
      nbuf[x] = nbuf[y];
      nbuf[y] = c;
      --x;
      ++y;
    }
  return needed;
}


/*(c cvt_ulong_to_HEX_n)
 * int cvt_ulong_to_HEX_n (t_uchar * nbuf,
 *                         size_t buflen,
 *                         unsigned long n,
 *                         size_t * needed);
 *
 * Convert `n' to a 0-terminated octal number hexadecimal number using
 * upper-case written to `nbuf' with has the size of `buflen'.  hex
 * digits "A..F".  Stores the size which would be needed by the
 * conversion in `*needed' if it is not null (even on failure).
 *
 * Return 0 on success and -1 if the buffer is too small.
 */
int
cvt_ulong_to_HEX_n (t_uchar * nbuf, size_t buflen, unsigned long n, size_t * needed)
{
  size_t used;
  t_uchar tmp[sizeof (unsigned long)*2 + 1];

  used = cvt_ulong_to_HEX (tmp, n);

  if (needed)
    *needed = used;

  if (used >= buflen)
    return -1;

  (void) str_cpy (nbuf, tmp);
  return 0;
}


/*(c cvt_long_to_HEX)
 * size_t cvt_long_to_HEX (t_uchar * nbuf, long n);
 * 
 * Convert `n' to a 0-terminated hexadecimal number using upper-case
 * hex digits "A..F".
 *
 * Return the number of digits in the output.
 */
size_t
cvt_long_to_HEX (t_uchar * nbuf, long n)
{
  if (n < 0)
    {
      nbuf[0] = '-';
      return 1 + cvt_ulong_to_HEX (nbuf + 1, -n);
    }
  else
    return cvt_ulong_to_HEX (nbuf, n);
}

/*(c cvt_long_to_HEX_n)
 * int cvt_long_to_HEX_n (t_uchar * nbuf,
 *                        size_t buflen,
 *                        unsigned long n,
 *                        size_t * needed);
 *
 * Convert `n' to a 0-terminated octal number hexadecimal number using
 * upper-case written to `nbuf' with has the size of `buflen'.  hex
 * digits "A..F".  Stores the size which would be needed by the
 * conversion in `*needed' if it is not null (even on failure).
 *
 * Return 0 on success and -1 if the buffer is too small.
 */
int
cvt_long_to_HEX_n (t_uchar * nbuf, size_t buflen, long n, size_t * needed)
{
  size_t used;
  t_uchar tmp[sizeof (unsigned long)*2 + 2];

  if (n < 0)
    {
      tmp[0] = '-';
      used = 1 + cvt_ulong_to_HEX (tmp + 1, -n);
    }
  else
    used = cvt_ulong_to_HEX (tmp, n);

  if (needed)
    *needed = used;

  if (used >= buflen)
    return -1;

  (void) str_cpy (nbuf, tmp);
  return 0;
}


/*(c cvt_ulong_to_hex)
 * void cvt_ulong_to_hex (t_uchar * nbuf, unsigned long n);
 *
 * Convert `n' to a 0-terminated hexadecimal number using lower-case
 * hex digits "a..f".
 *
 * Return the number of digits in the output.
 */
size_t
cvt_ulong_to_hex (t_uchar * nbuf, unsigned long n)
{
  static const t_uchar * hex = "0123456789abcdef";
  size_t x;
  size_t y;
  size_t needed;

  if (!n)
    {
      nbuf[0] = '0';
      nbuf[1] = 0;
      return 1;
    }

  x = 0;
  while (n)
    {
      nbuf[x] = hex[n % 16];
      n /= 16;
      x += 1;
    }
  nbuf[x] = 0;
  y = 0;
  needed = x--;
  while (x > y)
    {
      int c;
      c = nbuf[x];
      nbuf[x] = nbuf[y];
      nbuf[y] = c;
      --x;
      ++y;
    }
  return needed;
}


/*(c cvt_ulong_to_hex_n)
 * int cvt_ulong_to_hex_n (t_uchar * nbuf,
 *                         size_t buflen,
 *                         unsigned long n,
 *                         size_t * needed);
 *
 * Convert `n' to a 0-terminated octal number hexadecimal number using
 * lower-case written to `nbuf' with has the size of `buflen'.  hex
 * digits "a..f".  Stores the size which would be needed by the
 * conversion in `*needed' if it is not null (even on failure).
 *
 * Return 0 on success and -1 if the buffer is too small.
 */
int
cvt_ulong_to_hex_n (t_uchar * nbuf, size_t buflen, unsigned long n, size_t * needed)
{
  size_t used;
  t_uchar tmp[sizeof (unsigned long)*2 + 1];

  used = cvt_ulong_to_hex (tmp, n);

  if (needed)
    *needed = used;

  if (used >= buflen)
    return -1;

  (void) str_cpy (nbuf, tmp);
  return 0;
}


/*(c cvt_long_to_hex)
 * size_t cvt_long_to_hex (t_uchar * nbuf, long n);
 *
 * Convert `n' to a 0-terminated hexadecimal number using upper-case
 * hex digits "A..F".
 *
 * Return the number of digits in the output.
 */
size_t
cvt_long_to_hex (t_uchar * nbuf, long n)
{
  if (n < 0)
    {
      nbuf[0] = '-';
      return 1 + cvt_ulong_to_hex (nbuf + 1, -n);
    }
  else
    return cvt_ulong_to_hex (nbuf, n);
}


/*(c cvt_long_to_hex_n)
 * int cvt_long_to_hex_n (t_uchar * nbuf,
 *                        size_t buflen,
 *                        unsigned long n,
 *                        size_t * needed);
 *
 * Convert `n' to a 0-terminated octal number hexadecimal number using
 * lower-case written to `nbuf' with has the size of `buflen'.  hex
 * digits "a..f".  Stores the size which would be needed by the
 * conversion in `*needed' if it is not null (even on failure).
 *
 * Return 0 on success and -1 if the buffer is too small.
 */
int
cvt_long_to_hex_n (t_uchar * nbuf, size_t buflen, long n, size_t * needed)
{
  size_t used;
  t_uchar tmp[sizeof (unsigned long)*2 + 2];

  if (n < 0)
    {
      tmp[0] = '-';
      used = 1 + cvt_ulong_to_hex (tmp + 1, -n);
    }
  else
    used = cvt_ulong_to_hex (tmp, n);

  if (needed)
    *needed = used;

  if (used >= buflen)
    return -1;

  (void) str_cpy (nbuf, tmp);
  return 0;
}



/************************************************************************
 *(h2 "Converting Decimal Strings to Integers")
 * 
 */


/*(c cvt_decimal_to_ulong)
 * int cvt_decimal_to_ulong (int * errn,
 *			     unsigned long * answerp,
 *			     const t_uchar * text,
 *			     size_t len);
 * 
 * Convert the decimal number `text' to an unsigned long integer.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' is not a valid number.
 */
int
cvt_decimal_to_ulong (int * errn,
                      unsigned long * answerp,
                      const t_uchar * text,
                      size_t len)
{
  size_t consumed;
  unsigned long answer;

  if (cvt_scan_decimal_to_ulong (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}


/*(c cvt_scan_decimal_to_ulong)
 * int cvt_scan_decimal_to_ulong (int * errn,
 *				  unsigned long * answerp,
 *				  size_t * consumed,
 *			          const t_uchar * text,
 *			          size_t len);
 * 
 * Convert a decimal begining at `text' to an unsigned long integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_decimal_to_ulong (int * errn,
                           unsigned long * answerp,
                           size_t * consumed,
			   const t_uchar * text,
                           size_t len)
{
  unsigned long answer;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '+'))
    {
      ++text;
      --len;
    }

  if (!char_is_digit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_digit (*text))
    {
      int digit;
      unsigned long next;
      digit = *text - '0';
      next = answer * 10 + digit;
      if (answer && (next <= answer))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/*(c cvt_decimal_to_uint)
 * int cvt_decimal_to_uint (int * errn,
 *			    unsigned int * answerp,
 *			    const t_uchar * text,
 *			    size_t len);
 * 
 * Convert the decimal number `text' to an unsigned integer.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned int'.
 *	EINVAL	`text' is not a valid number.
 */
int
cvt_decimal_to_uint (int * errn,
                     unsigned int * answerp,
                     const t_uchar * text,
                     size_t len)
{
  size_t consumed;
  unsigned int answer;

  if( cvt_scan_decimal_to_uint (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}

/*(c cvt_scan_decimal_to_uint)
 * int cvt_scan_decimal_to_uint (int * errn,
 *				 unsigned int * answerp,
 *				 size_t * consumed,
 *			         const t_uchar * text,
 *			         size_t len);
 * 
 * Convert a decimal begining at `text' to an unsigned integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_decimal_to_uint (int * errn,
                          unsigned int * answerp,
			  size_t * consumed,
                          const t_uchar * text,
                          size_t len)
{
  unsigned int answer;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '+'))
    {
      ++text;
      --len;
    }

  if (!char_is_digit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_digit (*text))
    {
      int digit;
      unsigned int next;
      digit = *text - '0';
      next = answer * 10 + digit;
      if (answer && (next <= answer))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/*(c cvt_decimal_to_long)
 * int cvt_decimal_to_long (int * errn,
 *			    long * answerp,
 *			    const t_uchar * text,
 *			    size_t len);
 * 
 * Convert the decimal number `text' to a long integer.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE		the decimal number will not fit in a `long'.
 *	EINVAL		`text' is not a valid number.
 */
int
cvt_decimal_to_long (int * errn,
                     long * answerp,
                     const t_uchar * text,
                     size_t len)
{
  long answer;
  size_t consumed;

  if( cvt_scan_decimal_to_long (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}


/*(c cvt_scan_decimal_to_long)
 * int cvt_scan_decimal_to_long (int * errn,
 *				 long * answerp,
 *				 size_t * consumed,
 *			         const t_uchar * text,
 *			         size_t len);
 * 
 * Convert a decimal begining at `text' to a long integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_decimal_to_long (int * errn,
                          long * answerp,
			  size_t * consumed,
                          const t_uchar * text,
                          size_t len)
{
  long answer;
  int sign;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '-'))
    {
      sign = -1;
      ++text;
      --len;
    }
  else
    {
      sign = 1;
      if (len && (*text == '+'))
	{
	  ++text;
	  --len;
	}
    }

  if (!char_is_digit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_digit (*text))
    {
      int digit;
      long next;
      digit = *text - '0';
      next = answer * 10 + sign * digit;
      if (answer
          && ((sign > 0)
              ? (next <= answer)
              : (answer <= next)))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
      }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/*(c cvt_decimal_to_int)
 * int cvt_decimal_to_int (int * errn,
 *			   int * answerp,
 *			   const t_uchar * text,
 *			   size_t len);
 * 
 * Convert the decimal number `text' to an integer.
 *
 * Return 0 upon success, -1 on error.
 *
 * On error, `*errn' may be:
 *
 *	ERANGE		the decimal number will not fit in an `int'.
 *	EINVAL		`text' is not a valid number.
 */
int
cvt_decimal_to_int (int * errn,
                    int * answerp,
                    const t_uchar * text,
                    size_t len)
{
  size_t consumed;
  int answer;

  if( cvt_scan_decimal_to_int (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}

/*(c cvt_scan_decimal_to_int)
 * int cvt_scan_decimal_to_int (int * errn,
 *				int * answerp,
 *				size_t * consumed,
 *				const t_uchar * text,
 *				size_t len);
 * 
 * Convert a decimal begining at `text' to an integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_decimal_to_int (int * errn,
                         int * answerp,
			 size_t * consumed,
                         const t_uchar * text,
                         size_t len)
{
  int answer;
  int sign;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '-'))
    {
      sign = -1;
      ++text;
      --len;
    }
  else
    {
      sign = 1;
      if (len && (*text == '+'))
	{
	  ++text;
	  --len;
	}
    }

  if (!char_is_digit (*text))
    {
    einval:
      if (errn)
        if (errn)
          *errn = EINVAL;
      return -1;
    }

  while (len && char_is_digit (*text))
    {
      int digit;
      int next;
      digit = *text - '0';
      next = answer * 10 + sign * digit;
      if (answer &&
          ((sign > 0)
           ? (next <= answer)
           : (answer <= next)))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/************************************************************************
 *(h2 "Converting Hexadecimal Strings to Integers")
 * 
 */


/*(c cvt_hex_to_ulong)
 * int cvt_hex_to_ulong (int * errn,
 *			 unsigned long * answerp,
 *			 const t_uchar * text,
 *			 size_t len);
 * 
 * Convert the hexadecimal number `text' to an unsigned long integer.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the hexadecimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' is not a valid hexadecimal number.
 */
int
cvt_hex_to_ulong (int * errn,
                  unsigned long * answerp,
                  const t_uchar * text,
                  size_t len)
{
  size_t consumed;
  unsigned long answer;

  if( cvt_scan_hex_to_ulong (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}

/*(c cvt_scan_hex_to_ulong)
 * int cvt_scan_hex_to_ulong (int * errn,
 *			      unsigned long * answerp,
 *			      size_t * consumed,
 *			      const t_uchar * text,
 *			      size_t len);
 * 
 * Convert a hexadecimal begining at `text' to an unsigned long integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_hex_to_ulong (int * errn,
                       unsigned long * answerp,
		       size_t * consumed,
                       const t_uchar * text,
                       size_t len)
{
  unsigned long answer;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '+'))
    {
      ++text;
      --len;
    }

  if (!char_is_xdigit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_xdigit (*text))
    {
      int digit;
      unsigned long next;
      digit = char_digit_value (*text);
      next = answer * 16 + digit;
      if (answer && (next <= answer))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/*(c cvt_hex_to_uint)
 * int cvt_hex_to_uint (int * errn,
 *			unsigned int * answerp,
 *			const t_uchar * text,
 *			size_t len);
 * 
 * Convert the hexadecimal number `text' to an unsigned integer.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the hexadecimal number will not fit in an `unsigned int'.
 *	EINVAL	`text' is not a valid hexadecimal number.
 */
int
cvt_hex_to_uint (int * errn,
                 unsigned int * answerp,
                 const t_uchar * text,
                 size_t len)
{
  size_t consumed;
  unsigned int answer;

  if( cvt_scan_hex_to_uint (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}

/*(c cvt_scan_hex_to_uint)
 * int cvt_scan_hex_to_uint (int * errn,
 *			     unsigned int * answerp,
 *			     size_t * consumed,
 *			     const t_uchar * text,
 *			     size_t len);
 * 
 * Convert a hexadecimal begining at `text' to an unsigned integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_hex_to_uint (int * errn,
                      unsigned int * answerp,
                      size_t * consumed,
                      const t_uchar * text,
                      size_t len)
{
  unsigned int answer;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '+'))
    {
      ++text;
      --len;
    }

  if (!char_is_xdigit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_xdigit (*text))
    {
      int digit;
      unsigned int next;
      digit = char_digit_value (*text);
      next = answer * 16 + digit;
      if (answer && (next <= answer))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/*(c cvt_hex_to_long)
 * int cvt_hex_to_long (int * errn,
 *			long * answerp,
 *			const t_uchar * text,
 *			size_t len);
 * 
 * Convert the hexadecimal number `text' to a long integer.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE		the hexadecimal number will not fit in a `long'.
 *	EINVAL		`text' is not a valid hexadecimal number.
 */
int
cvt_hex_to_long (int * errn,
                 long * answerp,
                 const t_uchar * text,
                 size_t len)
{
  long answer;
  size_t consumed;

  if( cvt_scan_hex_to_long (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}

/*(c cvt_scan_hex_to_long)
 * int cvt_scan_hex_to_long (int * errn,
 *			     long * answerp,
 *			     size_t * consumed,
 *			     const t_uchar * text,
 *			     size_t len);
 * 
 * Convert a hexadecimal begining at `text' to a long integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_hex_to_long (int * errn,
                      long * answerp,
		      size_t * consumed,
                      const t_uchar * text,
                      size_t len)
{
  long answer;
  int sign;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '-'))
    {
      sign = -1;
      ++text;
      --len;
    }
  else
    {
      sign = 1;
      if (len && (*text == '+'))
	{
	  ++text;
	  --len;
	}
    }

  if (!char_is_xdigit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_xdigit (*text))
    {
      int digit;
      long next;
      digit = char_digit_value (*text);
      next = answer * 16 + sign * digit;
      if (answer &&
          ((sign > 0)
           ? (next <= answer)
           : (answer <= next)))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/*(c cvt_hex_to_int)
 * int cvt_hex_to_int (int * errn,
 *		       int * answerp,
 *		       const t_uchar * text,
 *		       size_t len);
 * 
 * Convert the hexadecimal number `text' to an integer.
 *
 * Return 0 upon success, -1 on error.
 *
 * On error, `*errn' may be:
 *
 *	ERANGE		the hexadecimal number will not fit in an `int'.
 *	EINVAL		`text' is not a valid hexadecimal number.
 */
int
cvt_hex_to_int (int * errn,
                int * answerp,
                const t_uchar * text,
                size_t len)
{
  size_t consumed;
  int answer;

  if( cvt_scan_hex_to_int (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}

/*(c cvt_scan_hex_to_int)
 * int cvt_scan_hex_to_int (int * errn,
 *			    int * answerp,
 *			    size_t * consumed,
 *			    const t_uchar * text,
 *			    size_t len);
 * 
 * Convert a hexadecimal begining at `text' to an integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_hex_to_int (int * errn,
                     int * answerp,
		     size_t * consumed,
                     const t_uchar * text,
                     size_t len)
{
  int answer;
  int sign;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '-'))
    {
      sign = -1;
      ++text;
      --len;
    }
  else
    {
      sign = 1;
      if (len && (*text == '+'))
	{
	  ++text;
	  --len;
	}
    }

  if (!char_is_xdigit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_xdigit (*text))
    {
      int digit;
      int next;
      digit = char_digit_value (*text);
      next = answer * 16 + sign * digit;
      if (answer &&
          ((sign > 0)
           ? (next <= answer)
           : (answer <= next)))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/************************************************************************
 *(h2 "Converting Octal Strings to Integers")
 * 
 */

/*(c cvt_octal_to_ulong)
 * int cvt_octal_to_ulong (int * errn,
 *			   unsigned long * answerp,
 *			   const t_uchar * text,
 *			   size_t len);
 * 
 * Convert the octal number `text' to an unsigned long integer.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the octal number will not fit in an `unsigned long'.
 *	EINVAL	`text' is not a valid octal number.
 */
int
cvt_octal_to_ulong (int * errn,
		    unsigned long * answerp,
		    const t_uchar * text,
		    size_t len)
{
  size_t consumed;
  unsigned long answer;

  if( cvt_scan_octal_to_ulong (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}

/*(c cvt_scan_octal_to_ulong)
 * int cvt_scan_octal_to_ulong (int * errn,
 *				unsigned long * answerp,
 *				size_t * consumed,
 *				const t_uchar * text,
 *				size_t len);
 * 
 * Convert a octal begining at `text' to an unsigned long integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_octal_to_ulong (int * errn,
			 unsigned long * answerp,
			 size_t * consumed,
			 const t_uchar * text,
			 size_t len)
{
  unsigned long answer;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '+'))
    {
      ++text;
      --len;
    }

  if (!char_is_odigit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_odigit (*text))
    {
      int digit;
      unsigned long next;
      digit = *text - '0';
      next = answer * 8 + digit;
      if (answer && (next <= answer))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/*(c cvt_octal_to_uint)
 * int cvt_octal_to_uint (int * errn,
 *			  unsigned int * answerp,
 *			  const t_uchar * text,
 *			  size_t len);
 * 
 * Convert the octal number `text' to an unsigned integer.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the octal number will not fit in an `unsigned int'.
 *	EINVAL	`text' is not a valid octal number.
 */
int
cvt_octal_to_uint (int * errn,
                   unsigned int * answerp,
                   const t_uchar * text,
                   size_t len)
{
  size_t consumed;
  unsigned int answer;

  if( cvt_scan_octal_to_uint (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}


/*(c cvt_scan_octal_to_uint)
 * int cvt_scan_octal_to_uint (int * errn,
 *			       unsigned int * answerp,
 *			       size_t * consumed,
 *			       const t_uchar * text,
 *			       size_t len);
 * 
 * Convert a octal begining at `text' to an unsigned integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_octal_to_uint (int * errn,
                        unsigned int * answerp,
			size_t * consumed,
                        const t_uchar * text,
                        size_t len)
{
  unsigned int answer;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '+'))
    {
      ++text;
      --len;
    }

  if (!char_is_odigit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_odigit (*text))
    {
      int digit;
      unsigned int next;
      digit = *text - '0';
      next = answer * 8 + digit;
      if (answer && (next <= answer))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/*(c cvt_octal_to_long)
 * int cvt_octal_to_long (int * errn,
 *			  long * answerp,
 *			  const t_uchar * text,
 *			  size_t len);
 * 
 * Convert the octal number `text' to a long integer.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE		the octal number will not fit in a `long'.
 *	EINVAL		`text' is not a valid octal number.
 */
int
cvt_octal_to_long (int * errn,
                   long * answerp,
                   const t_uchar * text,
                   size_t len)
{
  long answer;
  size_t consumed;

  if( cvt_scan_octal_to_long (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}


/*(c cvt_scan_octal_to_long)
 * int cvt_scan_octal_to_long (int * errn,
 *			       long * answerp,
 *			       size_t * consumed,
 *			       const t_uchar * text,
 *			       size_t len);
 * 
 * Convert a octal begining at `text' to a long integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_octal_to_long (int * errn,
                        long * answerp,
			size_t * consumed,
                        const t_uchar * text,
                        size_t len)
{
  long answer;
  int sign;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '-'))
    {
      sign = -1;
      ++text;
      --len;
    }
  else
    {
      sign = 1;
      if (len && (*text == '+'))
	{
	  ++text;
	  --len;
	}
    }

  if (!char_is_odigit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_odigit (*text))
    {
      int digit;
      long next;
      digit = *text - '0';
      next = answer * 8 + sign * digit;
      if (answer &&
          ((sign > 0)
           ? (next <= answer)
           : (answer <= next)))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}


/*(c cvt_octal_to_int)
 * int cvt_octal_to_int (int * errn,
 *			   int * answerp,
 *			   const t_uchar * text,
 *			   size_t len);
 * 
 * Convert the octal number `text' to an integer.
 *
 * Return 0 upon success, -1 on error.
 *
 * On error, `*errn' may be:
 *
 *	ERANGE		the octal number will not fit in an `int'.
 *	EINVAL		`text' is not a valid octal number.
 */
int
cvt_octal_to_int (int * errn,
                  int * answerp,
                  const t_uchar * text,
                  size_t len)
{
  size_t consumed;
  int answer;

  if( cvt_scan_octal_to_int (errn, &answer, &consumed, text, len) == -1)
    return -1;

  if (consumed != len)
    {
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  *answerp = answer;
  return 0;
}


/*(c cvt_scan_octal_to_int)
 * int cvt_scan_octal_to_int (int * errn,
 *			      int * answerp,
 *			      size_t * consumed,
 *			      const t_uchar * text,
 *			      size_t len);
 * 
 * Convert a octal begining at `text' to an integer.
 * Pass the actual characters parsed back in `*consumed' if `consumed' is not null.
 *
 * Return 0 upon success, -1 on error.
 * 
 * On error, `*errn' may be:
 *
 *	ERANGE	the decimal number will not fit in an `unsigned long'.
 *	EINVAL	`text' can not be parsed as a valid number.
 */
int
cvt_scan_octal_to_int (int * errn,
                       int * answerp,
		       size_t * consumed,
                       const t_uchar * text,
                       size_t len)
{
  int answer;
  int sign;
  size_t initial_len;

  initial_len = len;
  answer = 0;

  if (len == 0)
    goto einval;

  if (len && (*text == '-'))
    {
      sign = -1;
      ++text;
      --len;
    }
  else
    {
      sign = 1;
      if (len && (*text == '+'))
	{
	  ++text;
	  --len;
	}
    }

  if (!char_is_odigit (*text))
    {
    einval:
      if (errn)
        *errn = EINVAL;
      return -1;
    }

  while (len && char_is_odigit (*text))
    {
      int digit;
      int next;
      digit = *text - '0';
      next = answer * 8 + sign * digit;
      if (answer &&
          ((sign > 0)
           ? (next <= answer)
           : (answer <= next)))
        {
          if (errn)
            *errn = ERANGE;
          return -1;
        }
      answer = next;
      ++text;
      --len;
    }
  *answerp = answer;
  if (consumed)
    *consumed = initial_len - len;
  return 0;
}
