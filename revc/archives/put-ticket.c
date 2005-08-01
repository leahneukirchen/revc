/* put-ticket.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/put-from-str.h"
#include "revc/archives/put-ticket.h"

int
revc_put_ticket (const t_uchar * const errname,
                 struct revc_archives * archs,
                 int txn_n,
                 const t_uchar * const ticket)
{
  size_t len = str_length (ticket);
  t_uchar * ticket_path = file_name_in_vicinity (0, archs->_commits[txn_n].tmp_dir, "ticket");
  int status = revc_put_from_str (errname, ticket_path, ticket, len, 0);

  lim_free (0, ticket_path);
  return status;
}



/* arch-tag: Tom Lord Mon Jul  4 13:11:16 2005 (put-commit-ticket.c)
 */
