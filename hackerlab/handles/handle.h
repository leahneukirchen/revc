/* handle.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__HANDLES__HANDLE_H
#define INCLUDE__HANDLES__HANDLE_H


#include "hackerlab/machine/types.h"



/* automatically generated __STDC__ prototypes */
extern ssize_t alloc_handle (void);
extern int free_handle (ssize_t h);
extern ssize_t handle_ref (ssize_t h);
extern int handle_set (ssize_t h, ssize_t s, ssize_t r);
extern ssize_t alloc_handle_set (void);
extern int free_handle_set (ssize_t s);
#endif  /* INCLUDE__HANDLES__HANDLE_H */


/* tag: Tom Lord Sat Nov 20 15:52:22 2004 (handle.h)
 */
