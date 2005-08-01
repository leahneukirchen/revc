/* unign-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/mains/adjust-inventory-main.h"
#include "revc/mains/unign-main.h"

int
revc_unign_main (int argc, char ** argv)
{
  return revc_adjust_inventory_main (argc, argv, 0, 0);
}



/* arch-tag: Tom Lord Thu Jun 23 14:55:17 2005 (mains/unign-main.c)
 */
