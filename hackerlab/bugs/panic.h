/* panic.h - decls for fatal errors
 *
 ****************************************************************
 * Copyright (C) 1998, 2000 Thomas Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#ifndef INCLUDE__BUGS__PANIC_H
#define INCLUDE__BUGS__PANIC_H


#include "hackerlab/machine/types.h"
#include "hackerlab/bugs/panic-exit.h"


#define invariant_4(CONDITION, MSG, FILE, LINE) \
	do { if (!(CONDITION)) invariant_test (0, (MSG), (FILE), (LINE)); } while (0)

#define invariant_2(CONDITION, MSG) \
	invariant_4 ((CONDITION), (MSG), __FILE__, __LINE__)

#define invariant__x(CONDITION) \
	invariant_4 ((CONDITION), #CONDITION, __FILE__, __LINE__)

#define invariant(CONDITION) \
	invariant__x(CONDITION)

#define invariant_at_file_linex(CONDITION, FILE, LINE) \
	do { if (!(CONDITION)) invariant_test (0, #CONDITION, (FILE), (LINE)); } while (0)

#define invariant_at_file_line(CONDITION, FILE, LINE) \
	invariant_at_file_linex((CONDITION), (FILE), (LINE))


/* Use invariant_TODO when you want to write an invariant check
 * because you *think* the code is right but for external reasons
 * you aren't certain.  This allows an invariant failure to be
 * checked but also allows you to `grep' for code that needs
 * more careful review.
 */

#define invariant_TODO(X) invariant(X)




/* automatically generated __STDC__ prototypes */
extern void panic (const char * str);
extern void panic_msg (const char * str);
extern void invariant_test (int condition, const char * str, char * file, int line);
#endif  /* INCLUDE__BUGS__PANIC_H */
