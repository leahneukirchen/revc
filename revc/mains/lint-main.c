/* lint-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/mains/lint-loop-main.h"
#include "revc/mains/lint-main.h"

int
revc_lint_main (int argc, char ** argv)
{
  return revc_lint_loop_main (argc, argv, 1, 1);
}



/* arch-tag: Tom Lord Fri Jun 24 10:55:46 2005 (mains/lint-main.c)
 */
