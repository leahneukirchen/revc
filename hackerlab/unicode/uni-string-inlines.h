/* uni-string-inlines.h
 *
 ****************************************************************
 * Copyright (C) 2004 by Jose Antonio Ortega Ruiz
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__UNICODE__UNI_STRING_INLINES_H
#define INCLUDE__UNICODE__UNI_STRING_INLINES_H

#undef UNI_INLINE_QUALIFIERS
#define UNI_INLINE_QUALIFIERS


#ifndef __GNUC__
#undef __attribute__
#define __attribute__(X)
#endif



extern UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
     uni_string_raw (t_uchar * data);
extern UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
     uni_string_iso8859_1 (t_uchar * iso8859_1_data);
extern UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
     uni_string_utf8 (t_uchar * utf8_data);
extern UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
     uni_string_utf16 (t_uint16 * uint16_data);
extern UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
     uni_string_utf32 (t_uint32 * uint32_data);
extern UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
     uni_string_bogus32 (t_uint32 * bogus32_data);
extern UNI_INLINE_QUALIFIERS uni_string __attribute__((unused))
     uni_string_encoding (enum uni_encoding_scheme enc, t_uchar * data);


#endif /* INCLUDE__UNICODE__UNI_STRING_INLINES_H */


/* tag: Jose Antonio Ortega Ruiz Sat Feb 21 2004 01:22:48 (uni-string-inlines.h)
*/
