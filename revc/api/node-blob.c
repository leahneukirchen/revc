/* node-blob.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/error.h"
#include "revc/deps/file-stats.h"
#include "revc/api/file-blob.h"
#include "revc/api/node-blob.h"

int
revc_node_blob (const t_uchar * const errname,
                t_uchar * const blob_addr_ret,
                const t_uchar * const path_to_root,
                const t_uchar * const path_from_root,
                const t_uchar * const node)
{
  t_uchar * path_to_node = file_name_in_vicinity (0, path_to_root, node);
  int node_status;
  t_uchar node_type;
  size_t node_size;
  int answer = -69;

  node_status = revc_file_stats (errname, &node_type, &node_size, 0, path_to_node);
  if (node_status < 0)
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, path_to_node);
      return answer;
    }

  if (node_type == 'f')
    {
      if (revc_file_blob (errname, blob_addr_ret, path_to_node, node_size))
        goto bail;
      answer = 1;
      goto leave;
    }
  else if (node_type == 'd')
    {
      revc_error (errname, "node is a directory (%s)\n", node);
      goto bail;
    }
  else if (node_type == 0)
    {
      answer = 0;
      goto leave;
    }
  else
    {
      panic ("internal error");
      goto bail;
    }
}



/* arch-tag: Tom Lord Thu Jun 30 11:25:59 2005 (node-blob.c)
 */
