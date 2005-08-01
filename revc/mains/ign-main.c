/* ign-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/mains/adjust-inventory-main.h"
#include "revc/mains/ign-main.h"

int
revc_ign_main (int argc, char ** argv)
{
  return revc_adjust_inventory_main (argc, argv, 0, 1);
}



/* arch-tag: Tom Lord Thu Jun 23 14:54:25 2005 (mains/ign-main.c)
 */
