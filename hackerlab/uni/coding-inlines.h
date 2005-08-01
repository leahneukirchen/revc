/* coding-inlines.h - declarations for inline encoding/decoding functions
 *
 ****************************************************************
 * Copyright (C) 2000, 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__UNI__CODING_INLINES_H
#define INCLUDE__UNI__CODING_INLINES_H


#undef UNI_INLINE_QUALIFIERS
#define UNI_INLINE_QUALIFIERS


#ifndef __GNUC__
#undef __attribute__
#define __attribute__(X)
#endif


/* automatically generated __STDC__ prototypes */
extern UNI_INLINE_QUALIFIERS size_t __attribute__((unused)) uni_cv_sizeof (enum uni_encoding_scheme encoding);
extern UNI_INLINE_QUALIFIERS size_t __attribute__((unused)) uni_scanner_sizeof (enum uni_encoding_scheme encoding);
extern UNI_INLINE_QUALIFIERS ssize_t __attribute__((unused)) uni_cv_length_of_codepoint_in_encoding (enum uni_encoding_scheme enc,
                                                                                                     t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_is_codepoint (t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_is_bogus (t_unicode c);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_make_bogus (t_unicode c);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_extract_bogus_character (t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_is_high_surrogate (t_unicode c);
extern UNI_INLINE_QUALIFIERS int  __attribute__((unused)) uni_is_low_surrogate (t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_is_surrogate (t_unicode c);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_assemble_surrogates (t_unichar hi, t_unichar lo);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_iso8859_1_iscan (t_uchar * str,
                                                                                    size_t * pos,
                                                                                    size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf8_iscan (t_uchar * str,
                                                                               size_t * pos,
                                                                               size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf16be_iscan (t_uchar * str, size_t * pos, size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf16le_iscan (t_uchar * str, size_t * pos, size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf16_iscan (t_uchar * str, size_t * pos, size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf32le_iscan (t_uchar * str,
                                                                                  size_t * pos,
                                                                                  size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf32be_iscan (t_uchar * str,
                                                                                  size_t * pos,
                                                                                  size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf32_iscan (t_uchar * str, size_t * pos, size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_bogus32le_iscan (t_uchar * str,
                                                                                    size_t * pos,
                                                                                    size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_bogus32be_iscan (t_uchar * str,
                                                                                    size_t * pos,
                                                                                    size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_bogus32_iscan (t_uchar * str, size_t * pos, size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_iso8859_1_scan (t_uchar * str, size_t * pos, size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf8_scan (t_uchar * str, size_t * pos, size_t len);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf16_scan (t_uint16 * str, size_t * pos16, size_t len16);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_utf32_scan (t_uint32 * str, size_t * pos32, size_t len32);
extern UNI_INLINE_QUALIFIERS t_unicode __attribute__((unused)) uni_bogus32_scan (t_uint32 * str, size_t * pos32, size_t len32);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_iso8859_1_iput (t_uchar * str,
                                                                             size_t * pos,
                                                                             size_t len,
                                                                             t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf8_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf16be_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf16le_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf16_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf32le_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf32be_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf32_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_bogus32le_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_bogus32be_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_bogus32_iput (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_iso8859_1_put (t_uchar * str,
                                                                            size_t * pos,
                                                                            size_t len,
                                                                            t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf8_put (t_uchar * str, size_t * pos, size_t len, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf16_put (t_uint16 * str, size_t * pos16, size_t len16, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_utf32_put (t_uint32 * str, size_t * pos32, size_t len32, t_unicode c);
extern UNI_INLINE_QUALIFIERS int __attribute__((unused)) uni_bogus32_put (t_uint32 * str, size_t * pos32, size_t len32, t_unicode c);
#endif  /* INCLUDE__UNI__CODING_INLINES_H */
