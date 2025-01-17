/* tag: Tom Lord Tue Dec  4 14:41:34 2001 (=ffs.c)
 */
/* =ffs.c - scaffolding for ffs
 *
 ****************************************************************
 * Copyright (C) 2000 Thomas Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



/* This is handy:
   gcc -g -o =ffs =ffs.c ; ./=ffs ; rm =ffs
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int
main (int argc, char ** argv)
{
  int x;
  for (x = 0; x < 256; ++x)
    {
      int pos;
      for (pos = 0; pos < 8; ++pos)
	{
	  if (x & ((bitset_subset)1 << pos))
	    {
	      printf ("%d, ", pos);
	      break;
	    }
	}
      if (pos == 8)
	printf ("-1, ");
      if ((x % 16) == 15)
	putchar ('\n');
    }
  return 0;
}
