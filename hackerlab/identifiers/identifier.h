/* identifier.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__IDENTIFIER_H
#define INCLUDE__LIBAWIKI__IDENTIFIER_H


#include "hackerlab/machine/types.h"
#include "hackerlab/bugs/panic.h"

#define Identifier_decl(C_STEM, STRING) \
  static const t_uchar * \
  Identifier_ ## C_STEM (void) \
  { \
    static const t_uchar * it = 0; \
    if (!it) it = intern_identifier (STRING); \
    return it; \
  } extern int __identifier_unused_rubbish_ ## C_STEM

#define Identifier_safe_decl(C_STEM, STRING) \
  static const t_uchar * \
  Identifier_ ## C_STEM (void) \
  { \
    static const t_uchar * it = 0; \
    if (!it) it = intern_identifier (STRING); \
    invariant (!!it); \
    return it; \
  } extern int __identifier_unused_rubbish_ ## C_STEM


#define Identifier(STEM) (Identifier_ ## STEM ())



/* automatically generated __STDC__ prototypes */
extern const t_uchar * intern_identifier (const t_uchar * const name);
#endif  /* INCLUDE__LIBAWIKI__IDENTIFIER_H */


/* tag: Tom Lord Wed Oct 27 23:05:11 2004 (identifier.h)
 */
