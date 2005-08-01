/* is-valid-changeset-name.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/char-class.h"
#include "revc/deps/error.h"
#include "revc/api/constants.h"
#include "revc/api/blob-addr-skip.h"
#include "revc/api/is-valid-changeset-name.h"

int
revc_is_valid_changeset_name (const t_uchar * const errname,
                              const t_uchar * const candidate_name,
                              int report_errors)
{
  const t_uchar * pos = candidate_name;
  size_t len = 0;

  if (!candidate_name)
    {
    error:
      revc_error (errname, "invalid changeset name (%s)\n", candidate_name);
      return 0;
    }

  while ((len < REVC_MAX_REVNAME_SIZE) && *pos)
    {
      if (*pos == '/')
        break;
      if (*pos == '\n')
        goto error;
      ++pos;
      ++len;
    }

  if (*pos && (*pos != '/'))
    {
      revc_error (errname, "changeset name too long (%.*s...)\n", len, candidate_name);
      return 0;
    }

  if (*pos)
    ++pos;

  if (*pos && (*pos != '+'))
    {
      if (revc_blob_addr_skip (errname, &pos))
        goto error;
    }
  if (*pos && (*pos == '+'))
    {
      ++pos;
      if (revc_blob_addr_skip (errname, &pos))
        goto error;
    }
  if (*pos)
    goto error;

  return 1;
}



/* arch-tag: Tom Lord Sun Jun 26 17:30:43 2005 (is-valid-revision-name.c)
 */
