/* get.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/fmt/cvt.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/error.h"
#include "revc/deps/debug.h"
#include "revc/deps/ensure-dir-exists.h"
#include "revc/deps/put-from-str.h"
#include "revc/api/zput-from-str.h"
#include "revc/archives/commit-ticket.h"
#include "revc/archives/get-blob.h"
#include "revc/api/constants.h"
#include "revc/api/next-revname.h"
#include "revc/api/check-list.h"
#include "revc/api/list-skip.h"
#include "revc/api/ancestry-skip.h"
#include "revc/api/parse-ancestry-line.h"
#include "revc/api/build-node.h"
#include "revc/api/get.h"

#define BYTES_FOR_DECIMAL_ULONG  (4 * sizeof (t_ulong))


int
revc_get (const t_uchar * const errname,
          struct revc_archives * archs,
          const t_uchar * const revision,
          const t_uchar * const output_dir)
{
  int answer = -69;
  t_uchar fq_rev[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar ticket_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar rev[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar root_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar root_metadata[REVC_MAX_METADATA_SIZE + 1];
  t_uchar * revc_path = 0;
  t_uchar * manifest_path = 0;
  t_uchar * ignore_path = 0;
  t_uchar * prereqs_path = 0;
  t_uchar * ancestry_path = 0;
  t_uchar ignore_blob[REVC_MAX_BLOB_ADDR_SIZE];
  t_uchar prereqs_blob[REVC_MAX_BLOB_ADDR_SIZE];
  t_uchar ancestry_blob[REVC_MAX_BLOB_ADDR_SIZE];
  t_uchar * prereqs = 0;
  size_t prereqs_size = 0;

  t_uchar * manifest = 0;
  size_t manifest_size = 0;

  t_uchar * ignore = 0;
  size_t ignore_size = 0;
  t_uchar * ancestry = 0;
  size_t ancestry_size = 0;

  t_uchar * prereqs_tail = 0;
  size_t prereqs_tail_size = 0;
  t_uchar * tree_prereqs = 0;
  size_t tree_prereqs_size = 0;

  t_uchar * ancestry_tail = 0;
  size_t ancestry_tail_size = 0;

  t_uchar next_revname[REVC_MAX_REVNAME_SIZE + 1];

  size_t this_ancno;
  size_t this_mergeno;

  t_uchar * tree_ancestry;
  size_t tree_ancestry_size;

  if (revc_commit_ticket (errname,
                          fq_rev,
                          ticket_blob,
                          rev,
                          root_blob,
                          root_metadata,
                          ancestry_blob,
                          ignore_blob,
                          prereqs_blob,
                          0, 0, 0, 0,
                          &prereqs,
                          &prereqs_size,
                          archs,
                          revision))
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, revc_path);
      lim_free (0, manifest_path);
      lim_free (0, ignore_path);
      lim_free (0, prereqs_path);
      lim_free (0, ancestry_path);
      lim_free (0, manifest);
      lim_free (0, ignore);
      lim_free (0, prereqs);
      lim_free (0, ancestry);
      lim_free (0, tree_prereqs);
      return answer;
    }

  revc_debug (errname, "getting revision %s\n", fq_rev);

  if (0 >= revc_get_blob (errname, &ancestry, &ancestry_size,
                          archs, revision, prereqs, prereqs_size,
                          ancestry_blob,
                          0))
    goto bail;

  if (0 >= revc_get_blob (errname, &ignore, &ignore_size,
                          archs, revision, prereqs, prereqs_size,
                          ignore_blob,
                          0))
    goto bail;


  if (revc_build_node (errname,
                       &manifest, &manifest_size,
                       archs, fq_rev, prereqs, prereqs_size,
                       ".",
                       output_dir,
                       root_blob, root_metadata))
    goto bail;

  revc_path = file_name_in_vicinity (0, output_dir, ".revc");
  manifest_path = file_name_in_vicinity (0, output_dir, ".revc/manifest");
  ignore_path = file_name_in_vicinity (0, output_dir, ".revc/ignore");
  prereqs_path = file_name_in_vicinity (0, output_dir, ".revc/prereqs");
  ancestry_path = file_name_in_vicinity (0, output_dir, ".revc/ancestry");

  if (revc_ensure_dir_exists (errname, revc_path))
    goto bail;

  if (revc_put_from_str (errname, manifest_path, manifest, manifest_size, 0))
    goto bail;

  if (revc_put_from_str (errname, ignore_path, ignore, ignore_size, 0))
    goto bail;

  if (revc_check_list (errname, prereqs, prereqs_size) || !prereqs_size)
    {
      revc_error (errname, "corrupt prereqs\n");
      goto bail;
    }

  prereqs_tail = str_chr_index (prereqs, 0);
  prereqs_tail_size = prereqs_size - (prereqs_tail - prereqs);
  
  tree_prereqs_size = str_length (fq_rev) + prereqs_tail_size;
  tree_prereqs = lim_malloc (0, tree_prereqs_size);
  str_cpy (tree_prereqs, fq_rev);
  mem_move (tree_prereqs + str_length (tree_prereqs), prereqs_tail, prereqs_tail_size);

  if (revc_zput_from_str (errname, 0, prereqs_path, tree_prereqs, tree_prereqs_size))
    goto bail;

  if (!ancestry_size || revc_check_list (errname, ancestry, ancestry_size))
    {
      revc_error (errname, "corrupt ancestry\n");
      goto bail;
    }

  if (revc_parse_ancestry_line (errname, &this_ancno, &this_mergeno, 0, ancestry))
    goto bail;

  ancestry_tail = ancestry;
  ancestry_tail_size = ancestry_size;
  revc_ancestry_skip (&ancestry_tail, &ancestry_tail_size);

  if (revc_next_revname (errname, next_revname, rev))
    goto bail;

  tree_ancestry = lim_malloc (0, 4 * REVC_MAX_REVNAME_SIZE + ancestry_tail_size);
  tree_ancestry_size = 0;

  tree_ancestry_size += cvt_ulong_to_decimal (&tree_ancestry[tree_ancestry_size], (t_ulong)this_ancno + 1);
  tree_ancestry[tree_ancestry_size++] = '.';
  tree_ancestry[tree_ancestry_size++] = '0';
  tree_ancestry[tree_ancestry_size++] = '.';
  str_cpy (&tree_ancestry[tree_ancestry_size], next_revname);
  tree_ancestry_size += str_length (next_revname);
  tree_ancestry[tree_ancestry_size++] = 0;
  tree_ancestry_size += cvt_ulong_to_decimal (&tree_ancestry[tree_ancestry_size], (t_ulong)this_ancno);
  tree_ancestry[tree_ancestry_size++] = '.';
  tree_ancestry_size += cvt_ulong_to_decimal (&tree_ancestry[tree_ancestry_size], (t_ulong)this_mergeno);
  tree_ancestry[tree_ancestry_size++] = '.';
  str_cpy (&tree_ancestry[tree_ancestry_size], rev);
  tree_ancestry_size += str_length (rev);
  str_cpy (&tree_ancestry[tree_ancestry_size], "/+");
  tree_ancestry_size += str_length ("/+");
  str_cpy (&tree_ancestry[tree_ancestry_size], ticket_blob);
  tree_ancestry_size += str_length (ticket_blob);
  tree_ancestry[tree_ancestry_size++] = 0;
  mem_move (&tree_ancestry[tree_ancestry_size], ancestry_tail, ancestry_tail_size);
  tree_ancestry_size += ancestry_tail_size;

  if (revc_put_from_str (errname, ancestry_path, tree_ancestry, tree_ancestry_size, 0))
    goto bail;


  answer = 0;
  goto leave;
}



/* arch-tag: Tom Lord Sun Jun 26 17:09:53 2005 (get.c)
 */
