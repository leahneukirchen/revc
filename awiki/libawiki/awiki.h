/* awiki.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__AWIKI_H
#define INCLUDE__LIBAWIKI__AWIKI_H


#include "hackerlab/bugs/panic.h"
#include "awiki/libawiki/node-pts.h"
#include "awiki/libawiki/node-type.h"
#include "awiki/libawiki/parse.h"
#include "awiki/libawiki/html.h"



#define Regexp_decl(STEM, RE, CFLAGS) \
static regex_t * \
awiki__compiled_re_ ## STEM (void) \
{ \
  static int initialized = 0; \
  static regex_t preg; \
  \
  if (!initialized) \
    { \
      int stat; \
      stat = regcomp (&preg, (RE), CFLAGS); \
      invariant (!stat); \
      initialized = 1; \
    } \
  \
  return &preg; \
}


#define Regexp(STEM)  (awiki__compiled_re_ ## STEM ())


/* automatically generated __STDC__ prototypes */
extern int init_awiki (void);
#endif  /* INCLUDE__LIBAWIKI__AWIKI_H */


/* arch-tag: Tom Lord Sun Nov 21 11:25:45 2004 (libawiki/awiki.h)
 */
