/* tag: Tom Lord Tue Dec  4 14:41:43 2001 (unicode.h)
 */
/* unicode.h -
 *
 ****************************************************************
 * Copyright (C) 2000, 2004 Tom Lord, Jose A Ortega Ruiz
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__UNICODE__UNICODE_H
#define INCLUDE__UNICODE__UNICODE_H


#include "hackerlab/uni/coding.h"
#include "hackerlab/unidata/unidata.h"


/************************************************************************
 *(h2 "Low-level Unicode String Types")
 * 
 * 
 * 
 */




/*(c uni_string :category type)
 * typedef struct uni__undefined_struct * uni_string;
 *
 * The type `uni_string' is a union of pointers to values of different
 * sizes. It is used to represent the address of a Unicode string or
 * an address within a Unicode string.
 *
 * Any two `uni_string' values may be compared for equality using the
 * `==' operator.
 *
 * `uni_string' pointers within a single string may be compared
 * using any relational operator (`<', `>', etc.) by referring to
 * the `.raw' field.
 *
 * `uni_string' values are created from UTF-8 pointers (`t_uchar
 * *'), from UTF-16 pointers (`t_uint16 *') or from UTF-32 pointers
 * (`t_uint32 *') by means of provided cast functions (see below).
 *
 * By convention, all functions that operate on Unicode strings
 * accept two parameters for each string: an encoding form, and
 * a string pointer as in this function declaration:
 *
 *	void uni_fn (enum uni_encoding_scheme encoding,
 *		     uni_string s);
 *
 * By convention, the length of a Unicode string is always measured
 * in code units, no matter what the size of those code units.
 * Integer string indexes are also measured in code units.
 */
typedef union
{
  t_uchar * raw;
  t_uchar * iso8859_1;
  t_uchar * utf8;
  t_uint16 * utf16;
  t_uint32 * utf32;
  t_uint32 * bogus32;
  t_uchar * utf16be;
  t_uchar * utf32be;
  t_uchar * bogus32be;
  t_uchar * utf16le;
  t_uchar * utf32le;
  t_uchar * bogus32le;
} uni_string;


/*(c UNI_STRING_NULL :category macro)
 * #define UNI_STRING_NULL  ...
 *
 * An rvalue expression yielding a null-pointer cast to a
 * `uni_string'.
 */
#define UNI_STRING_NULL (uni_string_raw ((t_uchar*)0))


/*(c UNI_STRING_TO_raw :category macro)
 * #define UNI_STRING_TO_raw(s) ...
 *
 * Return the address of `uni_string' data cast to `t_uchar *'.
 */
/*(c UNI_STRING_TO_iso8859_1 :category macro)
 * #define UNI_STRING_TO_iso8859_1(s) ...
 *
 * Return the address of `uni_string' data cast to `t_uchar *'.
 */
/*(c UNI_STRING_TO_utf8 :category macro)
 * #define UNI_STRING_TO_utf8(s) ...
 *
 * Return the address of `uni_string' data cast to `t_uchar *'.
 */
/*(c UNI_STRING_TO_utf16 :category macro)
 * #define UNI_STRING_TO_utf16(s) ...
 *
 * Return the address of `uni_string' data cast to `t_uint16 *'.
 */
/*(c UNI_STRING_TO_utf32 :category macro)
 * #define UNI_STRING_TO_utf32(s) ...
 *
 * Return the address of `uni_string' data cast to `t_uint32 *'.
 */
/*(c UNI_STRING_TO_bogus32 :category macro)
 * #define UNI_STRING_TO_bogus32(s) ...
 *
 * Return the address of `uni_string' data cast to `t_uint32 *'.
 */

#define UNI_STRING_TO_raw(s) (s.raw)
#define UNI_STRING_TO_iso8859_1(s) (s.iso8859_1)
#define UNI_STRING_TO_utf8(s) (s.utf8)
#define UNI_STRING_TO_utf16(s) (s.utf16)
#define UNI_STRING_TO_utf32(s) (s.utf32)
#define UNI_STRING_TO_bogus32(s) (s.bogus32)

#if defined(__GNUC__) && defined(UNI_CODING_INLINES)

#undef UNI_INLINE_QUALIFIERS
#define UNI_INLINE_QUALIFIERS static inline
#include "hackerlab/unicode/uni-string-inlines.c"

#else

#undef UNI_INLINE_QUALIFIERS
#include "hackerlab/unicode/uni-string-inlines.h"

#endif


/* automatically generated __STDC__ prototypes */
#endif  /* INCLUDE__UNICODE__UNICODE_H */
