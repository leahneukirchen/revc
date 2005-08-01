/* uni-string-inlines.c
 *
 ****************************************************************
 * Copyright (C) 2004 by Jose Antonio Ortega Ruiz
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/unicode/unicode.h"
#include "hackerlab/bugs/panic.h"



#ifndef UNI_INLINE_QUALIFIERS
#define UNI_INLINE_QUALIFIERS
#endif

#ifndef __GNUC__
#undef __attribute__
#define __attribute__(X)
#endif



/************************************************************************
 *(h2 "Uni-string constructors")
 *

The type `uni_string' is a union of pointers to data of different
sizes, depending on the actual encoding scheme used by the Unicode
string at hand.  Hackerlab supports all the standard Unicode 4.0
encodings, plus `iso8859_1' for (extended) ASCII data and `bogus32'
(with -be and -le varients) for Unicode data mixed with non-Unicode
codepoints. The following construction functions create a new
`uni_string' value from a pointer to data in a concrete encoding
form. For instance, if `utf8_data' points to 8-bit, UTF8-encoded code
units, we can transform it to uni_string with:

   uni_string s = uni_string_utf8 (utf8_data);

Getting back the original pointer is then just a matter of accessing
the requisite union member:

   t_uchar * data = s.uni_utf8;

In addition to the standard unicode schemes, a `uni_string' can be
viewed as a raw pointer to data (via its `raw' union variant).  The
`bogus' encodings (`bogus32', `bogus32be', and `bogus32le') represent
a UTF-32 unicode string that may contain invalid codepoints.

*/

/*(c uni_string_raw)
 * uni_string uni_string_raw (t_uchar * data);
 *
 * Return a uni_string value wrapping the given pointer
 * to raw data.
 */
UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
uni_string_raw (t_uchar * data)
{
  uni_string answer;

  answer.raw = data;
  return answer;
}


/*(c uni_string_iso8859_1)
 * uni_string uni_string_iso8859_1 (t_uchar * iso8859_1_data);
 *
 * Return a uni_string value wrapping the given pointer
 * to ASCII data.
 */
UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
uni_string_iso8859_1 (t_uchar * iso8859_1_data)
{
  uni_string answer;

  answer.iso8859_1 = iso8859_1_data;
  return answer;
}


/*(c uni_string_uft8)
 * uni_string uni_string_utf8 (t_uchar * utf8_data);
 *
 * Return a uni_string value wrapping the given pointer
 * to UTF8 code units.
 */
UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
uni_string_utf8 (t_uchar * utf8_data)
{
  uni_string answer;

  answer.utf8 = utf8_data;
  return answer;
}

/*(c uni_string_uft16)
 * uni_string uni_string_utf16 (t_uint16 * uint16_data);
 *
 * Return a uni_string value wrapping the given pointer
 * to UTF16 code units.
 */
UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
uni_string_utf16 (t_uint16 * uint16_data)
{
  uni_string answer;

  answer.utf16 = uint16_data;
  return answer;
}

/*(c uni_string_uft32)
 * uni_string uni_string_utf32 (t_uint32 * utf32_data);
 *
 * Return a uni_string value wrapping the given pointer
 * to UTF32 code units.
 */
UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
uni_string_utf32 (t_uint32 * uint32_data)
{
  uni_string answer;

  answer.utf32 = uint32_data;
  return answer;
}

/*(c uni_string_bogus32)
 * uni_string uni_string_bogus32 (t_uint32 * uint32_data);
 *
 * Return a uni_string value wrapping the given pointer
 * to bogus code units.
 */
UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
uni_string_bogus32 (t_uint32 * uint32_data)
{
  uni_string answer;

  answer.bogus32 = uint32_data;
  return answer;
}

/*(c uni_string_encoding)
 * uni_string uni_string_encoding (enum uni_encoding_scheme enc, t_uchar * data);
 *
 * Return a uni_string value wrapping the given pointer
 * to data, which points to code units in the given encoding.
 */
UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
uni_string_encoding (enum uni_encoding_scheme enc, t_uchar * data)
{
  switch (enc)
    {
    default:
      panic ("Unexpected  uni_encoding_scheme value");
      return UNI_STRING_NULL;
    case uni_iso8859_1:
      return uni_string_iso8859_1 (data);
    case uni_utf8:
      return uni_string_utf8 (data);
    case uni_utf16:
    case uni_utf16le:
    case uni_utf16be:
      return uni_string_utf16 ((t_uint16 *) data);
    case uni_utf32:
    case uni_utf32le:
    case uni_utf32be:
      return uni_string_utf32 ((t_uint32 *) data);
    case uni_bogus32:
    case uni_bogus32le:
    case uni_bogus32be:
      return uni_string_bogus32 ((t_uint32 *) data);
    }
}




/* tag: Jose Antonio Ortega Ruiz Sat Feb 21 2004 02:04:23 (uni-string-inlines.c)
*/
