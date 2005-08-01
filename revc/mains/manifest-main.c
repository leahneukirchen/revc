/* manifest-main.c
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/mains/invlist-main.h"
#include "revc/mains/manifest-main.h"

int
revc_manifest_main (int argc, char ** argv)
{
  return revc_invlist_main (argc, argv, 0);
}  




/* arch-tag: Tom Lord Thu Jun 23 16:35:55 2005 (mains/manifest-main.c)
 */
