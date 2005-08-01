/* init.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "revc/archives/archives.h"
#include "revc/archives/init-archives.h"
#include "revc/archives/close-archives.h"
#include "revc/deps/rmrf.h"
#include "revc/deps/rename.h"
#include "revc/api/nil-fq-revision.h"
#include "revc/api/get.h"
#include "revc/api/init.h"


int
revc_init (const t_uchar * const errname)
{
  const t_uchar * const fqnil = revc_nil_fq_revision (errname);
  struct revc_archives archs = REVC_ARCHIVES_NIL_INIT;

  if (!fqnil)
    return -1;

  if (revc_init_archives (errname, &archs))
    return -1;

  revc_rmrf (errname, ",,.revc");

  if (revc_get (errname, &archs, fqnil, ",,.revc"))
    {
    bail:
      (void)revc_close_archives (errname, &archs);
      return -1;
    }

  if (revc_rename (errname, ",,.revc/.revc", ".revc"))
    goto bail;

  revc_rmrf (errname, ",,.revc");

  if (revc_close_archives (errname, &archs))
    return -1;

  return 0;
}






/* arch-tag: Tom Lord Thu Jun 23 11:25:38 2005 (init.c)
 */
