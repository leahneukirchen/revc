/* add-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/mains/adjust-inventory-main.h"
#include "revc/mains/add-main.h"

int
revc_add_main (int argc, char ** argv)
{
  return revc_adjust_inventory_main (argc, argv, 1, 0);
}



/* arch-tag: Tom Lord Thu Jun 23 14:52:45 2005 (add-main.c)
 */
