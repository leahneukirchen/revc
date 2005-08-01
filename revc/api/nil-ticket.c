/* nil-ticket.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/deps/debug.h"
#include "revc/api/constants.h"
#include "revc/api/nil-blob.h"
#include "revc/api/nil-ticket.h"

t_uchar *
revc_nil_ticket (const t_uchar * const errname)
{
  const t_uchar * root_blob;
  t_uchar * answer = 0;

  root_blob = revc_nil_blob (errname);
  if (!root_blob)
    return 0;

  answer = str_alloc_cat_many (0,
                               "revision: ", REVC_NIL_REVNAME, "\n",
                               "root-blob: ", root_blob, "\n",
                               "root-metadata: ", REVC_DEFAULT_DIR_METADATA, "\n",
                               "\n",
                               str_end);

  return answer;
}
                      



 /* arch-tag: Tom Lord Mon Jun 27 09:28:04 2005 (nil-root-ticket.c)
 */
