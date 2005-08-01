/* pika-escaping-utils.h - decls for saving escaped/unescaped pika scheme strings
 *
 ****************************************************************
 * Copyright (C) 2004 Christian Thaeter
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#ifndef INCLUDE__CHAR__PIKA_ESCAPING_UTILS_H
#define INCLUDE__CHAR__PIKA_ESCAPING_UTILS_H


#include "hackerlab/mem/alloc-limits.h"


/* automatically generated __STDC__ prototypes */
extern t_uchar * pika_save_escape_iso8859_1_n (size_t * length_return,
                                               alloc_limits limits,
                                               int escape_classes,
                                               const t_uchar * str,
                                               size_t length);
extern t_uchar * pika_save_escape_iso8859_1 (size_t * length_return,
                                             alloc_limits limits,
                                             int escape_classes,
                                             const t_uchar * str);
extern t_uchar * pika_save_unescape_iso8859_1_n (size_t * length_return,
                                                 alloc_limits limits,
                                                 const t_uchar * str,
                                                 size_t length);
extern t_uchar * pika_save_unescape_iso8859_1 (size_t * length_return,
                                               alloc_limits limits,
                                               const t_uchar * str);
#endif  /* INCLUDE__CHAR__PIKA_ESCAPING_UTILS_H */
