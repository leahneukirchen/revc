/* del-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/mains/adjust-inventory-main.h"
#include "revc/mains/del-main.h"

int
revc_del_main (int argc, char ** argv)
{
  return revc_adjust_inventory_main (argc, argv, 0, 0);
}



/* arch-tag: Tom Lord Thu Jun 23 14:54:04 2005 (mains/del-main.c)
 */
