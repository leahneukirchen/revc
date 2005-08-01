/* create-empty-file.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/vu/safe.h"
#include "revc/deps/create-empty-file.h"

int
revc_create_empty_file (const t_uchar * const errname,
                        const t_uchar * const file)
{
  safe_close (safe_open (file, O_CREAT | O_WRONLY, 0666));
  return 0;
}





/* arch-tag: Tom Lord Thu Jun 23 12:55:22 2005 (create-empty-file.c)
 */
