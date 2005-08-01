/* revname-from-ancestry.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/deps/error.h"
#include "revc/api/check-list.h"
#include "revc/api/parse-ancestry-line.h"
#include "revc/api/revname-from-ancestry.h"

t_uchar *
revc_revname_from_ancestry (const t_uchar * const errname,
                            t_uchar * const ancestry,
                            size_t const ancestry_size)
{
  t_uchar * pos = ancestry;
  size_t remain = ancestry_size;
  t_uchar * revision = 0;

  if (revc_check_list (errname, pos, remain))
    return 0;

  if (!remain)
    {
      revc_error (errname, "empty ancestry\n");
      return 0;
    }

  if (revc_parse_ancestry_line (errname, 0, 0, &revision, pos))
    return 0;

  return str_save (0, revision);
}



/* arch-tag: Tom Lord Mon Jun 27 15:55:14 2005 (revname-from-ancestry.c)
 */
