/* coding.h - coding system conversions
 *
 ****************************************************************
 * Copyright (C) 2000, 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__UNI__CODING_H
#define INCLUDE__UNI__CODING_H



#include "hackerlab/machine/types.h"



#define UNI_ENCODING_SCHEMES                                       \
	UNI_ENCODING (iso8859_1, 1, 1, 256)  /* must be first */   \
	UNI_ENCODING (utf8, 1, 1, (1 << 21))                       \
	UNI_ENCODING (utf16, 2, 2, (1 << 21))                      \
	UNI_ENCODING (utf16be, 2, 1, (1 << 21))                    \
	UNI_ENCODING (utf16le, 2, 1, (1 << 21))                    \
        UNI_ENCODING (utf32, 4, 4, (1 << 21))                      \
        UNI_ENCODING (utf32le, 4, 1, (1 << 21))                    \
        UNI_ENCODING (utf32be, 4, 1, (1 << 21))                    \
        UNI_ENCODING (bogus32, 4, 4, (1 << 23))                    \
        UNI_ENCODING (bogus32le, 4, 1, (1 << 23))                  \
        UNI_ENCODING (bogus32be, 4, 1, (1 << 23))

enum uni_encoding_scheme
{
#undef UNI_ENCODING
#define UNI_ENCODING(NAME, WIDTH, SCANNER_WIDTH, CSET_SIZE)	uni_ ## NAME,
  UNI_ENCODING_SCHEMES
};

#define UNI_ENCODING_PAIR(a,b)		(((a)<<4) | (b))




#define UNI_SCAN_INPUT_TRUNCATED ((t_unicode)1 << 23)



typedef t_unicode (*uni_iscan_fn)(t_uchar * str, size_t * pos, size_t len);
typedef int (*uni_iput_fn)(t_uchar * str, size_t * pos, size_t len, t_unicode c);


#if defined(__GNUC__) && (defined(UNI_CODING_INLINES) || defined(UNI_INLINES))

#undef UNI_INLINE_QUALIFIERS
#define UNI_INLINE_QUALIFIERS static inline
#include "hackerlab/uni/coding-inlines.c"

#else

#undef UNI_INLINE_QUALIFIERS
#include "hackerlab/uni/coding-inlines.h"

#endif



/* automatically generated __STDC__ prototypes */
extern uni_iscan_fn uni_encoding_iscan_fn (enum uni_encoding_scheme encoding);
extern uni_iput_fn uni_encoding_iput_fn (enum uni_encoding_scheme encoding);
#endif  /* INCLUDE__UNI__CODING_H */
