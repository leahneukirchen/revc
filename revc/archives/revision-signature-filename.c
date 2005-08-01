/* revision-signature-filename.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str-many.h"
#include "revc/archives/revision-signature-filename.h"

t_uchar *
revc__revision_signature_filename (const t_uchar * const errname,
                                   const t_uchar * const ticket_blob,
                                   const t_uchar * const commit_ticket_blob)
{
  return str_alloc_cat_many (0,
                             ".",
                             commit_ticket_blob,
                             "+",
                             ticket_blob,
                             str_end);
}



/* arch-tag: Tom Lord Thu Jul 14 11:27:45 2005 (revision-signature-filename.c)
 */
