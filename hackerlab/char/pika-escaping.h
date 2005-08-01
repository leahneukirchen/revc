/* pika-escaping.h - decls for escaping/unescaping pika scheme strings
 *
 ****************************************************************
 * Copyright (C) 2004 Christian Thaeter
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#ifndef INCLUDE__CHAR__PIKA_ESCAPING_H
#define INCLUDE__CHAR__PIKA_ESCAPING_H

#include "hackerlab/char/char-class.h"
#include "hackerlab/char/str.h"



/* automatically generated __STDC__ prototypes */
extern size_t pika_escape_iso8859_1_n (t_uchar * result,
                                       size_t result_length,
                                       int escape_classes,
                                       const t_uchar * str,
                                       size_t length);
extern int pika_unescape_iso8859_1_n (size_t * result_needed,
                                      size_t * str_used,
                                      t_uchar * result,
                                      size_t result_length,
                                      const t_uchar * str,
                                      size_t length);
#endif  /* INCLUDE__CHAR__PIKA_ESCAPING_H */
