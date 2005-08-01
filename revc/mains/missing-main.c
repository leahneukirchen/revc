/* missing-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/mains/lint-loop-main.h"
#include "revc/mains/missing-main.h"

int
revc_missing_main (int argc, char ** argv)
{
  return revc_lint_loop_main (argc, argv, 0, 1);
}



/* arch-tag: Tom Lord Fri Jun 24 12:21:14 2005 (mains/missing-main.c)
 */
