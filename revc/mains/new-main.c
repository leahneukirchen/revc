/* new-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/mains/lint-loop-main.h"
#include "revc/mains/new-main.h"

int
revc_new_main (int argc, char ** argv)
{
  return revc_lint_loop_main (argc, argv, 1, 0);
}



/* arch-tag: Tom Lord Fri Jun 24 12:20:25 2005 (mains/new-main.c)
 */
