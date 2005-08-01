/* tag: Tom Lord Tue Dec  4 14:41:36 2001 (coding-inlines.c)
 */
/* coding-inlines.c -
 *
 ****************************************************************
 * Copyright (C) 2000, 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/machine/endian.h"
#include "hackerlab/machine/types.h"
#include "hackerlab/bugs/panic.h"
#include "hackerlab/uni/invariant.h"
#include "hackerlab/uni/coding.h"



#ifndef UNI_INLINE_QUALIFIERS
#define UNI_INLINE_QUALIFIERS
#endif

#ifndef __GNUC__
#undef __attribute__
#define __attribute__(X)
#endif



/************************************************************************
 *(h2 "Lowest Level Unicode String Functions")
 *
 *
 * These functions provide the basics of reading and writing codepoints
 * in strings.
 */



/************************************************************************
 *(h3 "Encoding Scheme Size Information")
 *
 */


/*(c uni_cv_sizeof)
 * size_t uni_cv_sizeof (enum uni_encoding_scheme encoding);
 *
 * Return the size in bytes of code units used in a particular
 * encoding scheme (1 for uni_iso8859_1 and uni_utf8, 2 for uni_utf16,
 * etc.).
 */
UNI_INLINE_QUALIFIERS size_t __attribute__((unused))
uni_cv_sizeof (enum uni_encoding_scheme encoding)
{
  switch (encoding)
    {
#undef UNI_ENCODING
#define UNI_ENCODING(NAME, WIDTH, SCANNER_WIDTH, CSET_SIZE)	case uni_ ## NAME: return (size_t)WIDTH; break;

      UNI_ENCODING_SCHEMES;

    default:
      return uni_cv_sizeof (uni_iso8859_1);
    }
}


/*(c uni_scanner_sizeof)
 * size_t uni_scanner_sizeof (enum uni_encoding_scheme encoding);
 *
 * Return the size in bytes of the unit of length used by the
 * prefered scanner routine for a given encoding.
 * 
 * In other words, for native encoding forms, return 1;
 * For -be and -le forms, return what ujni_cv_sizeof would return.
 */
UNI_INLINE_QUALIFIERS size_t __attribute__((unused))
uni_scanner_sizeof (enum uni_encoding_scheme encoding)
{
  switch (encoding)
    {
#undef UNI_ENCODING
#define UNI_ENCODING(NAME, WIDTH, SCANNER_WIDTH, CSET_SIZE)	case uni_ ## NAME: return (size_t)SCANNER_WIDTH; break;

      UNI_ENCODING_SCHEMES;

    default:
      return uni_cv_sizeof (uni_iso8859_1);
    }
}


UNI_INLINE_QUALIFIERS ssize_t __attribute__((unused))
uni_cv_length_of_codepoint_in_encoding (enum uni_encoding_scheme enc,
                                        t_unicode c)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ((c < 255) ? (ssize_t)1 : (ssize_t)-1);
    case uni_utf8:
      return (  (c < 128)     ? (ssize_t)1
              : (c < (1<<11)) ? (ssize_t)2
              : (c < (1<<16)) ? (ssize_t)3
              : (c < (1<<21)) ? (ssize_t)4
              :                 (ssize_t)-2);
    case uni_utf16:
    case uni_utf16be:
    case uni_utf16le:
      return (  (c < (1<<16))  ? (ssize_t)1
              : (c < (1<<21))  ? (ssize_t)2
              :                  (ssize_t)-2);

    case uni_utf32:
    case uni_utf32be:
    case uni_utf32le:
      return (  (c < (1<<21))  ? (ssize_t)1
              :                  (ssize_t)-2);

    case uni_bogus32:
    case uni_bogus32be:
    case uni_bogus32le:
      return (ssize_t)1;
    }
}


/************************************************************************
 *(h3 "Codepoint Properties for Reading and Writing Strings")
 *
 *
 *
 */


/*(c uni_is_codepoint)
 * int uni_is_codepoint (t_unicode c);
 *
 * Return non-0 iff `c' is in the closed range [0..0x1fffff].
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_is_codepoint (t_unicode c)
{
  return (c <= (t_unicode)0x1fffffL);
}


#define UNI_BOGUS_MASK ((t_unicode)1 << 21)
/*(c uni_is_bogus)
 * int uni_is_bogus (t_unicode c);
 *
 * Return non-0 iff `c' is a bogus unicode character.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_is_bogus (t_unicode c)
{
  return (c >> 21) == (t_unicode)1;
}


/*(c uni_make_bogus)
 * t_unicode uni_is_bogus (t_unicode c);
 *
 * Return a new bogus unicode character containing `c'. Calling
 * `uni_is_bogus' on the returned value always yields a non-zero
 * value.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_make_bogus (t_unicode c)
{
  uni_invariant (uni_is_codepoint (c));
  return (c | UNI_BOGUS_MASK);
}


/*(c uni_extract_bogus)
 * t_unicode uni_extract_bogus (t_unicode c);
 *
 * Return the codepoint contained in `c'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_extract_bogus_character (t_unicode c)
{
  uni_invariant (uni_is_bogus (c));
  return (c & (~UNI_BOGUS_MASK));
}

#undef UNI_BOGUS_MASK


/*(c uni_is_high_surrogate)
 * int uni_is_high_surrogate (t_unicode c);
 *
 * Return 1 if `c' is a high surrogate, 0 otherwise.
 *
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_is_high_surrogate (t_unicode c)
{
  return (((c) >= 0xdb80) && ((c) <= 0xdbff));
}


/*(c uni_is_low_surrogate)
 * int uni_is_low_surrogate (t_unicode c);
 *
 * Return 1 if `c' is a low surrogate, 0 otherwise.
 *
 */
UNI_INLINE_QUALIFIERS int  __attribute__((unused))
uni_is_low_surrogate (t_unicode c)
{
  return (((c) >= 0xdc00) && ((c) <= 0xdfff));
}


/*(c uni_is_surrogate)
 * int uni_is_surrogate (t_unicode c);
 *
 * Return 1 if `c' is a surrogate codepoint, 0 otherwise.
 *
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_is_surrogate (t_unicode c)
{
  return (c >= 0xdb80) && (c <= 0xdfff);
}


/*(c uni_assemble_surrogates)
 * t_unicode uni_assemble_surrogates (t_unichar hi, t_unichar lo);
 *
 * Assemble a high and low surrogate codpoint pair into the
 * single codepoint they denote.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_assemble_surrogates (t_unichar hi, t_unichar lo)
{
  return ((hi - 0xd800) * 0x400) + (lo - 0xdc00) + 0x10000;
}



/************************************************************************
 *(h3 "Unaligned Codepoint Scanning")
 *
 *
 * These functions are used to read codepoints from strings
 * of various encodings.
 *
 * In the nominal case, all scan functions return the next codepoint in
 * the input stream. If the codepoint is cut-off (would require bytes
 * beyond the stream's length), UNI_SCAN_INPUT_TRUNCATED is returned
 * instead. Finally, if the encoding is invalid, the offending (up to
 * 4) bytes are wrapped in a `bogus' codepoint. Thus, the
 * returned value should always be ckecked using `uni_is_bogus'.
 *
 * \Note:/ The string data manipulated by these functions does _not_
 * have to be aligned appropriately for the code unit of the encoding
 * scheme. These functions read one byte at a time.
 *
 * \Note:/ In contrast to many unicode string functions in
 * `libhackerlab', these functions measure string lengths and
 * manipulate string indexes as _byte_ addresses.
 */


/*(c uni_iso8859_1_iscan)
 * t_unicode uni_iso8859_1_iscan (t_uchar * str,
 *                                size_t * pos,
 *                                size_t len)
 *
 * Return the (8-bit) codepoint at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_iso8859_1_iscan (t_uchar * str,
                     size_t * pos,
                     size_t len)
{
  uni_invariant (str && pos && *pos < len);
  return str [(*pos)++];
}



/*(c uni_utf8_iscan)
 * t_unicode uni_utf8_iscan (t_uchar * str,
 *                           size_t * pos,
 *                           size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the offending byte is encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf8_iscan (t_uchar * str,
                size_t * pos,
                size_t len)
{
  t_unicode c0;
  t_unicode c1;
  t_unicode c2;
  t_unicode c3;
  t_unicode c;

  uni_invariant (str && pos && (*pos < len));

  c0 = str [(*pos)++];
  switch (c0 >> 3)
    {
    case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
    case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
      return c0;

    case 24: case 25: case 26: case 27:

      if ((*pos) >= len)
	{
	  --(*pos);
	  return UNI_SCAN_INPUT_TRUNCATED;
	}

      c1 = str [(*pos)++];

      if ((c1 >> 6) != 2)
	{
          --(*pos);
	  return uni_make_bogus (c0);
	}

      c = (((c0 & 0x1f) << 6) | (c1 & 0x3f));

      if (c < 128)
	{
          --(*pos);
	  return uni_make_bogus (c0);
	}

      return c;

    case 28: case 29:
      if (((*pos) + 1) >= len)
	{
	  --(*pos);
	  return UNI_SCAN_INPUT_TRUNCATED;
	}

      c1 = str [(*pos)++];
      c2 = str [(*pos)++];

      if (((c1 >> 6) != 2) || ((c2 >> 6) != 2))
	{
          (*pos) -= 2;
	  return uni_make_bogus (c0);
	}

      c = ((c0 & 0xf) << 12) | ((c1 & 0x3f) << 6) | (c2 & 0x3f);

      if (uni_is_surrogate (c) || (c < (1 << 11)))
	{
          (*pos) -= 2;
	  return uni_make_bogus (c0);
	}

      return c;

    case 30:

      if (((*pos) + 2) >= len)
	{
	  --(*pos);
	  return UNI_SCAN_INPUT_TRUNCATED;
	}

      c1 = str [(*pos)++];
      c2 = str [(*pos)++];
      c3 = str [(*pos)++];

      if (((c1 >> 6) != 2) || ((c2 >> 6) != 2) || ((c3 >> 6) != 2))
	{
	  (*pos) -= 3;
	  return uni_make_bogus (c0);
	}

      c = (((c0 & 7) << 18) | ((c1 & 0x3f) << 12) | ((c2 & 0x3f) << 6) | (c3 & 0x3f));

      if ((c < (1 << 16)) || (c > 0x1fffffL))
	{
	  (*pos) -= 3;
	  return uni_make_bogus (c0);
	}

      return c;

    default:			/* 16 and 31 */
      return uni_make_bogus (c0);
    }
}



/*(c uni_utf16be_iscan)
 * t_unicode uni_utf16be_iscan (t_uchar * str,
 *                              size_t * pos,
 *                              size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 2 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf16be_iscan (t_uchar * str, size_t * pos, size_t len)
{
  t_unicode hi;
  t_unicode lo;
  t_unicode first;
  t_unicode second;

  uni_invariant (str && pos && (((*pos) + 1) < len));

  hi = str [(*pos)++];
  lo = str [(*pos)++];
  first = (hi << 8) | lo;

  if (uni_is_low_surrogate (first))
    {
      return uni_make_bogus (first);
    }

  if (!uni_is_high_surrogate (first))
    return first;

  if (((*pos) + 1) >= len)
    {
      (*pos) -= 2;
      return UNI_SCAN_INPUT_TRUNCATED;
    }

  hi = str [(*pos)++];
  lo = str [(*pos)++];
  second = (hi << 8) | lo;

  if (!uni_is_low_surrogate (second))
    {
      (*pos) -= 2;
      return uni_make_bogus (first);
    }

  return uni_assemble_surrogates (first, second);
}


/*(c uni_utf16le_iscan)
 * t_unicode uni_utf16le_iscan (t_uchar * str,
 *                              size_t * pos,
 *                              size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 2 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf16le_iscan (t_uchar * str, size_t * pos, size_t len)
{
  t_unicode hi;
  t_unicode lo;
  t_unicode first;
  t_unicode second;

  uni_invariant (str && pos && (((*pos) + 1) < len));

  lo = str [(*pos)++];
  hi = str [(*pos)++];
  first = (hi << 8) | lo;

  if (uni_is_low_surrogate (first))
    {
      return uni_make_bogus (first);
    }

  if (!uni_is_high_surrogate (first))
    return first;

  if (((*pos) + 1) >= len)
    {
      (*pos) -= 2;
      return UNI_SCAN_INPUT_TRUNCATED;
    }

  lo = str [(*pos)++];
  hi = str [(*pos)++];
  second = (hi << 8) | lo;

  if (!uni_is_low_surrogate (second))
    {
      (*pos) -= 2;
      return uni_make_bogus (first);
    }

  return uni_assemble_surrogates (first, second);
}


/*(c uni_utf16_iscan)
 * t_unicode uni_utf16_iscan (t_uchar * str,
 *                            size_t * pos,
 *                            size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 2 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf16_iscan (t_uchar * str, size_t * pos, size_t len)
{
#if MACHINE_IS_BIGENDIAN
  return uni_utf16be_iscan (str, pos, len);
#else
  return uni_utf16le_iscan (str, pos, len);
#endif
}

/*(c uni_utf32le_iscan)
 * t_unicode uni_utf32le_iscan (t_uchar * str,
 *                              size_t * pos,
 *                              size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 4 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf32le_iscan (t_uchar * str,
                   size_t * pos,
                   size_t len)
{
  t_unicode result;
  size_t p;

  uni_invariant (str && pos && (*pos < len));

  p = *pos;

  if (p + 4 > len) return UNI_SCAN_INPUT_TRUNCATED;

  result = str[p] + (str[p + 1] << 8) + (str[p + 2] << 16) + (str[p + 3] << 24);

  (*pos) += 4;

  return result;
}


/*(c uni_utf32be_iscan)
 * t_unicode uni_utf32be_iscan (t_uchar * str,
 *                              size_t * pos,
 *                              size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 4 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf32be_iscan (t_uchar * str,
                   size_t * pos,
                   size_t len)
{
  t_unicode result;
  size_t p;

  uni_invariant (str && pos && (*pos < len));

  p = *pos;

  if (p + 4 > len) return UNI_SCAN_INPUT_TRUNCATED;

  result = str[p + 3] + (str[p + 2] << 8) + (str[p + 1] << 16) + (str[p] << 24);

  (*pos) += 4;

  return result;
}

/*(c uni_utf32_iscan)
 * t_unicode uni_utf32_iscan (t_uchar * str,
 *                            size_t * pos,
 *                            size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 4 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf32_iscan (t_uchar * str, size_t * pos, size_t len)
{
#if MACHINE_IS_BIGENDIAN
  return uni_utf32be_iscan (str, pos, len);
#else
  return uni_utf32le_iscan (str, pos, len);
#endif
}


/*(c uni_bogus32le_iscan)
 * t_unicode uni_bogus32le_iscan (t_uchar * str,
 *                              size_t * pos,
 *                              size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 4 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_bogus32le_iscan (t_uchar * str,
                   size_t * pos,
                   size_t len)
{
  t_unicode result;
  size_t p;

  uni_invariant (str && pos && (*pos < len));

  p = *pos;

  if (p + 4 > len) return UNI_SCAN_INPUT_TRUNCATED;

  result = str[p] + (str[p + 1] << 8) + (str[p + 2] << 16) + (str[p + 3] << 24);

  (*pos) += 4;
  return result;
}

/*(c uni_bogus32be_iscan)
 * t_unicode uni_bogus32be_iscan (t_uchar * str,
 *                              size_t * pos,
 *                              size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 4 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_bogus32be_iscan (t_uchar * str,
                   size_t * pos,
                   size_t len)
{
  t_unicode result;
  size_t p;

  uni_invariant (str && pos && (*pos < len));

  p = *pos;

  if (p + 4 > len) return UNI_SCAN_INPUT_TRUNCATED;

  result = str[p + 3] + (str[p + 2] << 8) + (str[p + 1] << 16) + (str[p] << 24);

  (*pos) += 4;
  return result;
}


/*(c uni_bogus32_iscan)
 * t_unicode uni_bogus32_iscan (t_uchar * str,
 *                              size_t * pos,
 *                              size_t len)
 *
 * Return the (possibly bogus) codepoint which begins at offset `*pos'
 * in `str'.  As a side-effect, increment `*pos' past the end of the
 * codepoint.  `len' indicates the length (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond `len') then
 * `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 4 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.  
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_bogus32_iscan (t_uchar * str, size_t * pos, size_t len)
{
#if MACHINE_IS_BIGENDIAN
  return uni_bogus32be_iscan (str, pos, len);
#else
  return uni_bogus32le_iscan (str, pos, len);
#endif
}


/************************************************************************
 *(h3 "Aligned Codepoint Scanning")
 *
 *
 * These functions are used to read codepoints from strings
 * of various encodings.
 *
 * \Note:/ The string data manipulated by these functions have to be
 * aligned appropriately for the code unit of the encoding scheme.
 *
 * \Note:/ These functions measure string lengths and
 * manipulate string indexes as _code_value_ indexes.
 */


/*(c uni_iso8859_1_scan)
 * t_unicode uni_iso8859_1_scan (t_uchar * str,
 *                               size_t * pos,
 *                               size_t len)
 *
 * Return the (8-bit) codepoint at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_iso8859_1_scan (t_uchar * str, size_t * pos, size_t len)
{
  return uni_iso8859_1_iscan (str, pos, len);
}


/*(c uni_utf8_scan)
 * t_unicode uni_utf8_scan (t_uchar * str,
 *                          size_t * pos,
 *                          size_t len)
 *
 * Return the codepoint which begins at offset `*pos'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len' indicates the length
 * (in bytes) of the string.
 *
 * If the codepoint is cut-off (would require bytes beyond
 * `len') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the offending byte is encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf8_scan (t_uchar * str, size_t * pos, size_t len)
{
  return uni_utf8_iscan (str, pos, len);
}



/*(c uni_utf16_scan)
 * t_unicode uni_utf16_scan (t_uint16 * str,
 *                           size_t * pos16,
 *                           size_t len16)
 *
 * Return the codepoint which begins at offset `*pos16'
 * in `str'.   As a side-effect, increment `*pos' past
 * the end of the codepoint.  `len16' indicates the length
 * of the string (measured in `t_uint16' values).
 *
 * If the codepoint is cut-off (would require data beyond
 * `len16') then `*pos' is not modified and UNI_SCAN_INPUT_TRUNCATED is returned.
 * (This is intended to facilitate applications reading characters
 * from a buffered I/O stream.)
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 2 offending bytes are encoded in a bogus character and returned.
 * Thus, the value returned by this function should be ckecked using
 * `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf16_scan (t_uint16 * str, size_t * pos16, size_t len16)
{
  t_unicode first;
  t_unicode second;

  uni_invariant (str && pos16 && ((*pos16) < len16));

  first = str[(*pos16)++];

  if (uni_is_low_surrogate (first))
    {
      return uni_make_bogus (first);
    }

  if (!uni_is_high_surrogate (first))
    return first;

  if ((*pos16) >= len16)
    {
      (*pos16) -= 1;
      return UNI_SCAN_INPUT_TRUNCATED;
    }

  second = str [(*pos16)++];

  if (!uni_is_low_surrogate (second))
    {
      (*pos16) -= 1;
      return uni_make_bogus (first);
    }

  return uni_assemble_surrogates (first, second);
}

/*(c uni_utf32_scan)
 * t_unicode uni_utf32_scan (t_uint32 * str,
 *                           size_t * pos32,
 *                           size_t len32)
 *
 * Return the codepoint which begins at offset `*pos32' in `str'.  As
 * a side-effect, increment `*pos' past the end of the codepoint.
 * `len32' indicates the length of the string (measured in `t_uint32'
 * values).
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 4 offending bytes are encoded in a bogus character and
 * returned.  Thus, the value returned by this function should be
 * ckecked using `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_utf32_scan (t_uint32 * str, size_t * pos32, size_t len32)
{
  t_unicode result;

  uni_invariant (str && pos32 && ((*pos32) < len32));

  result = str[(*pos32)++];
  return result;
}


/*(c uni_bogus32_scan)
 * t_unicode uni_bogus32_scan (t_uint32 * str,
 *                           size_t * pos32,
 *                           size_t len32)
 *
 * Return the codepoint which begins at offset `*pos32' in `str'.  As
 * a side-effect, increment `*pos' past the end of the codepoint.
 * `len32' indicates the length of the string (measured in `t_uint32'
 * values).
 *
 * If the string beginning at offset `*pos' is not validly encoded,
 * the 4 offending bytes are encoded in a bogus character and
 * returned.  Thus, the value returned by this function should be
 * ckecked using `uni_is_bogus'.
 */
UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused))
uni_bogus32_scan (t_uint32 * str, size_t * pos32, size_t len32)
{
  t_unicode result;

  uni_invariant (str && pos32 && ((*pos32) < len32));

  result = str[(*pos32)++];
  return result;
}


/************************************************************************
 *(h3 "Unaligned Codepoint Writing")
 *
 *
 * These functions are used to write codepoints to strings
 * of various encodings.
 *
 * \Note:/ The string data manipulated by these functions
 * does _not_ have to be aligned appropriately for the code
 * unit of the encoding scheme.  These functions write one byte
 * at a time.
 *
 * \Note:/ In contrast to many unicode string functions in
 * `libhackerlab', these functions measure string lengths and
 * manipulate string indexes as _byte_ addresses.
 */


/*(c uni_iso8859_1_iput)
 * int uni_iso8859_1_iput (t_uchar * str,
 *                         size_t * pos,
 *                         size_t len,
 *                         t_unicode c)
 *
 * Write the codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 *
 * \Note:/ This function may only be called for values of `c' less
 * than 256.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_iso8859_1_iput (t_uchar * str,
                    size_t * pos,
                    size_t len,
                    t_unicode c)
{
  uni_invariant (str && pos && (c < 256));

  if ((*pos) >= len)
    {
      return -1;
    }
  str[(*pos)++] = (t_uchar)c;
  return 1;
}


/*(c uni_utf8_iput)
 * int uni_utf8_iput (t_uchar * str,
 *                    size_t * pos,
 *                    size_t len,
 *                    t_unicode c)
 *
 * Write the codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf8_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
  uni_invariant (str && pos);
  uni_invariant (uni_is_codepoint (c));
  uni_invariant (!uni_is_surrogate (c));

  if (c < 0x80)
    {
      if (*pos >= len)
	return -1;
      str[(*pos)++] = c;
      return 1;
    }
  else if (c < 0x800)
    {
      if ((*pos + 1) >= len)
	return -2;

      str[(*pos)++] = (0xc0 | (c >> 6));
      str[(*pos)++] = (0x80 | (c & 0x3f));
      return 2;
    }
  else if (c <= 0xffff)
    {
      if ((*pos + 2) >= len)
	return -3;

      str[(*pos)++] = (0xe0 | (c >> 12));
      str[(*pos)++] = (0x80 | ((c >> 6) & 0x3f));
      str[(*pos)++] = (0x80 | (c & 0x3f));
      return 3;
    }
  else if (c < 0x110000)
    {
      t_unicode hi_bits;

      if ((*pos + 3) >= len)
	return -4;

      hi_bits = (c >> 16);
      c &= 0xffff;

      str[(*pos)++] = (0xf0 | (hi_bits >> 2));
      str[(*pos)++] = (0x80 | ((hi_bits & 3) << 4) | ((c >> 12) & 0xf));
      str[(*pos)++] = (0x80 | ((c >> 6) & 0x3f));
      str[(*pos)++] = (0x80 | (c & 0x3f));
      return 4;
    }
  else
    while (1)
      panic ("out of range character in uni_utf8_iput");
}


/*(c uni_utf16be_iput)
 * int uni_utf16be_iput (t_uchar * str,
 *                       size_t * pos,
 *                       size_t len,
 *                       t_unicode c)
 *
 * Write the codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf16be_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
  uni_invariant (str && pos);
  uni_invariant (uni_is_codepoint (c));
  uni_invariant (!uni_is_surrogate (c));

  if (c <= 0xffff)
    {
      if ((*pos + 1) >= len)
	return -2;

      str[(*pos)++] = ((c >> 8) & 0xff);
      str[(*pos)++] = (c & 0xff);
      return 2;
    }
  else
    {
      t_unicode hi;
      t_unicode lo;

      if ((*pos + 3) >= len)
	return -4;

      hi = (c - 0x10000) / 0x400 + 0xd800;
      lo = (c - 0x10000) + 0xdc00;

      str[(*pos)++] = ((hi >> 8) & 0xff);
      str[(*pos)++] = (hi & 0xff);

      str[(*pos)++] = ((lo >> 8) & 0xff);
      str[(*pos)++] = (lo & 0xff);
      return 4;
    }
}


/*(c uni_utf16le_iput)
 * int uni_utf16le_iput (t_uchar * str,
 *                       size_t * pos,
 *                       size_t len,
 *                       t_unicode c)
 *
 * Write the codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf16le_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
  uni_invariant (str && pos);
  uni_invariant (uni_is_codepoint (c));
  uni_invariant (!uni_is_surrogate (c));

  if (c <= 0xffff)
    {
      if ((*pos + 1) >= len)
	return -2;

      str[(*pos)++] = (c & 0xff);
      str[(*pos)++] = ((c >> 8) & 0xff);
      return 2;
    }
  else
    {
      t_unicode hi;
      t_unicode lo;

      if ((*pos + 3) >= len)
	return -4;

      hi = (c - 0x10000) / 0x400 + 0xd800;
      lo = (c - 0x10000) + 0xdc00;

      str[(*pos)++] = (hi & 0xff);
      str[(*pos)++] = ((hi >> 8) & 0xff);

      str[(*pos)++] = (lo & 0xff);
      str[(*pos)++] = ((lo >> 8) & 0xff);
      return 4;
    }
}


/*(c uni_utf16_iput)
 * int uni_utf16_iput (t_uchar * str,
 *                     size_t * pos,
 *                     size_t len,
 *                     t_unicode c)
 *
 * Write the codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf16_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
#if MACHINE_IS_BIGENDIAN
  return uni_utf16be_iput (str, pos, len, c);
#else
  return uni_utf16le_iput (str, pos, len, c);
#endif
}

/*(c uni_utf32le_iput)
 * int uni_utf32le_iput (t_uchar * str,
 *                       size_t * pos,
 *                       size_t len,
 *                       t_unicode c)
 *
 * Write the codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf32le_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
  uni_invariant (str && pos);
  uni_invariant (uni_is_codepoint (c));
  uni_invariant (!uni_is_surrogate (c));

  if ((*pos) + 4 > len) return -4;

  str[(*pos)++] = c & 0xff;
  str[(*pos)++] = (c >> 8) & 0xff;
  str[(*pos)++] = (c >> 16) & 0xff;
  str[(*pos)++] = 0;

  return 4;
}

/*(c uni_utf32be_iput)
 * int uni_utf32be_iput (t_uchar * str,
 *                       size_t * pos,
 *                       size_t len,
 *                       t_unicode c)
 *
 * Write the codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf32be_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
  uni_invariant (str && pos);
  uni_invariant (uni_is_codepoint (c));
  uni_invariant (!uni_is_surrogate (c));

  if ((*pos) + 4 > len) return -4;

  str[(*pos)++] = 0;
  str[(*pos)++] = (c >> 16) & 0xff;
  str[(*pos)++] = (c >> 8) & 0xff;
  str[(*pos)++] = c & 0xff;

  return 4;
}


/*(c uni_utf32_iput)
 * int uni_utf32_iput (t_uchar * str,
 *                     size_t * pos,
 *                     size_t len,
 *                     t_unicode c)
 *
 * Write the codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf32_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
#if MACHINE_IS_BIGENDIAN
  return uni_utf32be_iput (str, pos, len, c);
#else
  return uni_utf32le_iput (str, pos, len, c);
#endif
}


/*(c uni_bogus32le_iput)
 * int uni_bogus32le_iput (t_uchar * str,
 *                         size_t * pos,
 *                         size_t len,
 *                         t_unicode c)
 *
 * Write the (possibly bogus) codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_bogus32le_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
  uni_invariant (str && pos);

  if ((*pos) + 4 > len) return -4;

  str[(*pos)++] = c & 0xff;
  str[(*pos)++] = (c >> 8) & 0xff;
  str[(*pos)++] = (c >> 16) & 0xff;
  str[(*pos)++] = 0;

  return 4;
}


/*(c uni_bogus32be_iput)
 * int uni_bogus32be_iput (t_uchar * str,
 *                         size_t * pos,
 *                         size_t len,
 *                         t_unicode c)
 *
 * Write the (possibly bogus) codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_bogus32be_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
  uni_invariant (str && pos);

  if ((*pos) + 4 > len) return -4;

  str[(*pos)++] = 0;
  str[(*pos)++] = (c >> 16) & 0xff;
  str[(*pos)++] = (c >> 8) & 0xff;
  str[(*pos)++] = c & 0xff;

  return 4;
}


/*(c uni_bogus32_iput)
 * int uni_bogus32_iput (t_uchar * str,
 *                       size_t * pos,
 *                       size_t len,
 *                       t_unicode c)
 *
 * Write the (possibly bogus) codepoint `c' to `str' at offset `*pos'.
 *
 * As a side effect, increment `*pos' past the newly written
 * codpoint.
 *
 * Normally, return the number of bytes written (the same amount
 * by which `*pos' is incremented).
 *
 * If writing the codepoint would exceed the length, `len', of the
 * string, then instead return the inverse (negative) number of bytes
 * required to store the codepoint.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_bogus32_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
#if MACHINE_IS_BIGENDIAN
  return uni_bogus32be_iput (str, pos, len, c);
#else
  return uni_bogus32le_iput (str, pos, len, c);
#endif
}


/************************************************************************
 *(h3 "Aligned Codepoint Writing")
 *
 *
 * These functions are used to write codepoints to strings
 * of various encodings.
 *
 * \Note:/ The string data manipulated by these functions have to be
 * aligned appropriately for the code unit of the encoding scheme.
 *
 * \Note:/ These functions measure string lengths and
 * manipulate string indexes as _code_value_ indexes.
 */


/*(c uni_iso8859_1_put)
 * int uni_iso8859_1_put (t_uchar * str,
 *                        size_t * pos,
 *                        size_t len,
 *                        t_unicode c)
 *
 * Store the codepoint `c' in `str' beginning at code value
 * offset `*pos'.
 *
 * Normally, advance `*pos' past the end of the newly stored
 * codepoint and return the number of code values written.
 *
 * If storing `c' would require writing beyond the `len' code values
 * beginning at `str', then instead return the inverse (negative) number
 * of code values needed to store `c'.
 *
 * \Note:/ It is an error to call this function with values of `c'
 * greater than 256.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_iso8859_1_put (t_uchar * str,
                   size_t * pos,
                   size_t len,
                   t_unicode c)
{
  return uni_iso8859_1_iput (str, pos, len, c);
}


/*(c uni_utf8_put)
 * int uni_utf8_put (t_uchar * str,
 *                   size_t * pos,
 *                   size_t len,
 *                   t_unicode c)
 *
 * Store the codepoint `c' in `str' beginning at code value
 * offset `*pos'.
 *
 * Normally, advance `*pos' past the end of the newly stored
 * codepoint and return the number of code values written.
 *
 * If storing `c' would require writing beyond the `len' code values
 * beginning at `str', then instead return the inverse (negative) number
 * of code values needed to store `c'.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf8_put (t_uchar * str, size_t * pos, size_t len, t_unicode c)
{
  return uni_utf8_iput (str, pos, len, c);
}


/*(c uni_utf16_put)
 * int uni_utf16_put (t_uint16 * str,
 *                    size_t * pos,
 *                    size_t len,
 *                    t_unicode c)
 *
 * Store the codepoint `c' in `str' beginning at code value
 * offset `*pos'.
 *
 * Normally, advance `*pos' past the end of the newly stored
 * codepoint and return the number of code values written.
 *
 * If storing `c' would require writing beyond the `len' code values
 * beginning at `str', then instead return the inverse (negative) number
 * of code values needed to store `c'.
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf16_put (t_uint16 * str, size_t * pos16, size_t len16, t_unicode c)
{
  uni_invariant (str && pos16);
  uni_invariant (uni_is_codepoint (c));
  uni_invariant (!uni_is_surrogate (c));

  if (c <= 0xffff)
    {
      if ((*pos16) >= len16)
	return -1;

      str[(*pos16)++] = (t_uint16)(c & 0xffff);
      return 1;
    }
  else
    {
      t_unicode hi;
      t_unicode lo;

      if ((*pos16 + 1) >= len16)
	return -2;

      hi = (c - 0x10000) / 0x400 + 0xd800;
      lo = (c - 0x10000) + 0xdc00;

      str[(*pos16)++] = (t_uint16)(hi & 0xffff);
      str[(*pos16)++] = (t_uint16)(lo & 0xffff);

      return 2;
    }
}

/*(c uni_utf32_put)
 * int uni_utf32_put (t_uint32 * str,
 *                    size_t * pos,
 *                    size_t len,
 *                    t_unicode c)
 *
 * Store the codepoint `c' in `str' beginning at code value
 * offset `*pos'.
 *
 * Advance `*pos' past the end of the newly stored
 * codepoint and return the number of code values written (i.e. 1).
 *
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_utf32_put (t_uint32 * str, size_t * pos32, size_t len32, t_unicode c)
{
  uni_invariant (str && pos32 && ((*pos32) < len32));
  uni_invariant (uni_is_codepoint (c));

  str[(*pos32)++] = c;
  return 1;
}

/*(c uni_bogus32_put)
 * int uni_bogus32_put (t_uint32 * str,
 *                      size_t * pos,
 *                      size_t len,
 *                      t_unicode c)
 *
 * Store the (possibly bogus) codepoint `c' in `str' beginning at code
 * value offset `*pos'.
 *
 * Advance `*pos' past the end of the newly stored
 * codepoint and return the number of code values written (i.e. 1).
 *
 */
UNI_INLINE_QUALIFIERS int __attribute__((unused))
uni_bogus32_put (t_uint32 * str, size_t * pos32, size_t len32, t_unicode c)
{
  uni_invariant (str && pos32 && ((*pos32) < len32));

  str[(*pos32)++] = c;
  return 1;
}

