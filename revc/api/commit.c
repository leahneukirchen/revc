/* commit.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fs/file-names.h"
#include "hackerlab/fmt/cvt.h"
#include "revc/deps/error.h"
#include "revc/deps/debug.h"
#include "revc/deps/warn.h"
#include "revc/deps/rmrf.h"
#include "revc/deps/put-from-str.h"
#include "revc/deps/rename.h"
#include "revc/deps/file-stats.h"
#include "revc/archives/commit-ticket.h"
#include "revc/archives/get-blob.h"
#include "revc/archives/begin-commit-txn.h"
#include "revc/archives/put-ticket.h"
#include "revc/archives/commit-file-size.h"
#include "revc/archives/commit-n-blobs.h"
#include "revc/archives/commit-zip-size.h"
#include "revc/archives/commit-blob-contents-blob.h"
#include "revc/archives/put-commit-ticket.h"
#include "revc/archives/put-commit-prereqs.h"
#include "revc/archives/commit-needs-str-blob.h"
#include "revc/archives/commit-txn-has-hint.h"
#include "revc/archives/end-commit-txn.h"
#include "revc/api/constants.h"
#include "revc/api/zput-from-str.h"
#include "revc/api/make-commit-ticket.h"
#include "revc/api/make-ticket.h"
#include "revc/api/copy-blob-to-dir-entry.h"
#include "revc/api/copy-metadata-to-dir-entry.h"
#include "revc/api/copy-fname-to-dir-entry.h"
#include "revc/api/file-blob-commiting.h"
#include "revc/api/string-blob.h"
#include "revc/api/traversal-cmp.h"
#include "revc/api/next-revname.h"
#include "revc/api/check-list.h"
#include "revc/api/list-find.h"
#include "revc/api/list-subtract-subtrees.h"
#include "revc/api/add-selected-subtrees.h"
#include "revc/api/list-find-subdir.h"
#include "revc/api/begin-revtrav.h"
#include "revc/api/revtrav-next.h"
#include "revc/api/revtrav-path.h"
#include "revc/api/revtrav-blob.h"
#include "revc/api/revtrav-metadata.h"
#include "revc/api/end-revtrav.h"
#include "revc/api/list-skip.h"
#include "revc/api/parse-ancestry-line.h"
#include "revc/api/commit.h"


/* __STDC__ prototypes for static functions */



static int commit_next_node (const t_uchar * const errname,
                             t_uchar ** const root_path_ret,
                             t_uchar * const root_blob_ret,
                             t_uchar * const root_metadata_ret,
                             const t_uchar * const path_to_root,
                             struct revc_archives * const archs,
                             const t_uchar * const revision,
                             int txn_n,
                             t_uchar * const prereqs,
                             size_t prereqs_size,
                             struct revc_revtrav_state * orig_revtrav,
                             t_uchar ** selected_pos,
                             size_t * selected_remain,
                             t_uchar ** manifest_pos,
                             size_t * manifest_remain,
                             t_uchar ** ignore_pos,
                             size_t * ignore_remain);
static int commit_from_tree (const t_uchar * const errname,
                             t_uchar ** const root_path_ret,
                             t_uchar * const root_blob_ret,
                             t_uchar * const root_metadata_ret,
                             const t_uchar * const path_to_root,
                             struct revc_archives * archs,
                             const t_uchar * const revision,
                             int const txn_n,
                             t_uchar * const prereqs,
                             size_t const prereqs_size,
                             t_uchar * const manifest,
                             size_t const manifest_size);



int
revc_commit (const t_uchar * const errname,
             struct revc_archives * const archs,
             const t_uchar * const revname,
             const t_uchar * const path_to_root,
             const t_uchar * const path_from_root,
             t_uchar * const manifest,
             size_t manifest_size,
             t_uchar * const ignore,
             size_t ignore_size,
             t_uchar * const ancestry,
             size_t const ancestry_size,
             t_uchar * const prereqs,
             size_t const prereqs_size,
             t_uchar * const selected_subtrees,
             size_t const selected_subtrees_size)
{
  int answer = -69;

  t_uchar * primary_prereq = 0;

  t_uchar orig_fq_revision[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar orig_root_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar orig_root_metadata[REVC_MAX_METADATA_SIZE + 1];
  t_uchar orig_ignore_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];

  int txn_n = -69;

  t_uchar * root_path = 0;
  t_uchar root_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar root_metadata[REVC_MAX_BLOB_ADDR_SIZE + 1];

  struct revc_revtrav_state origtrav_state;
  t_uchar * selected_pos = 0;
  size_t selected_remain = 0;
  t_uchar * manifest_pos = 0;
  size_t manifest_remain = 0;
  t_uchar * ignore_pos = 0;
  size_t ignore_remain = 0;

  int first_node_status = -69;

  t_uchar * orig_ignore = 0;
  size_t orig_ignore_size = 0;
  t_uchar * this_ignore = 0;
  size_t this_ignore_size = 0;
  t_uchar this_ignore_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];

  t_uchar commit_ancestry_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar * commit_ancestry = 0;
  size_t commit_ancestry_size = 0;
  t_uchar * ancestry_head = 0;
  size_t ancestry_head_size = 0;
  t_uchar * ancestry_tail = 0;
  size_t ancestry_tail_size = 0;

  t_uchar * commit_prereqs = 0;
  size_t commit_prereqs_size = 0;
  t_uchar commit_prereqs_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];

  t_uchar * ticket = 0;
  t_uchar wq_revname[REVC_MAX_REVNAME_SIZE + 1];
  size_t wq_revname_len;
  t_uchar ticket_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];

  size_t n_blobs = 0;
  size_t total_file_size = 0;
  size_t total_zip_size = 0;
  t_uchar blob_contents_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];

  t_uchar * commit_ticket = 0;
  t_uchar fq_revname[REVC_MAX_REVNAME_SIZE + 1];
  size_t fq_revname_len;

  t_uchar commit_ticket_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];

  t_uchar * commit_prereqs_tail = 0;
  size_t commit_prereqs_tail_size = 0;
  t_uchar * tree_prereqs = 0;
  size_t tree_prereqs_size = 0;
  
  size_t commit_ancestry_head_size = 0;
  t_uchar * commit_ancestry_tail = 0;
  size_t commit_ancestry_tail_size = 0;
  size_t this_anc = 0;
  t_uchar this_anc_prefix[2 * 4 * sizeof (t_ulong) + sizeof ("..")];
  t_uchar next_revname[REVC_MAX_REVNAME_SIZE];
  
  t_uchar * tree_ancestry = 0;
  size_t tree_ancestry_size = 0;

  t_uchar * prereqs_tmp_path = file_name_in_vicinity (0, path_to_root, ".revc/,,prereqs");
  t_uchar * prereqs_path = file_name_in_vicinity (0, path_to_root, ".revc/prereqs");
  t_uchar * ancestry_tmp_path = file_name_in_vicinity (0, path_to_root, ".revc/,,ancestry");
  t_uchar * ancestry_path = file_name_in_vicinity (0, path_to_root, ".revc/ancestry");
  
  if (revc_check_list (errname, ancestry, ancestry_size))
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, root_path);
      lim_free (0, this_ignore);
      lim_free (0, orig_ignore);
      lim_free (0, ticket);
      lim_free (0, commit_ancestry);
      lim_free (0, commit_prereqs);
      lim_free (0, tree_prereqs);
      lim_free (0, tree_ancestry);
      lim_free (0, prereqs_tmp_path);
      lim_free (0, prereqs_path);
      lim_free (0, ancestry_tmp_path);
      lim_free (0, ancestry_path);
      return answer;
    }

  if (!ancestry_size)
    {
      revc_error (errname, "empy ancestry?\n");
      goto bail;
    }

  if (revc_check_list (errname, prereqs, prereqs_size))
    goto bail;

  if (!prereqs_size)
    {
      revc_error (errname, "empy prereqs list?\n");
      goto bail;
    }

  primary_prereq = prereqs;

  if (revc_commit_ticket (errname,
                          orig_fq_revision,
                          0,
                          0,
                          orig_root_blob,
                          orig_root_metadata,
                          0,
                          orig_ignore_blob,
                          0,
                          0, 0, 0, 0,
                          0, 0,
                          archs,
                          primary_prereq))
    goto bail;

  
  txn_n = revc_begin_commit_txn (errname, archs, revname, prereqs, prereqs_size);
  if (txn_n < 0)
    goto bail;


  if (revc_begin_revtrav (errname, &origtrav_state, orig_root_blob, orig_root_metadata))
    goto bail;

  selected_pos = selected_subtrees;
  selected_remain = selected_subtrees_size;

  manifest_pos = manifest;
  manifest_remain = manifest_size;

  ignore_pos = ignore;
  ignore_remain = ignore_size;

  first_node_status = commit_next_node (errname, &root_path, root_blob, root_metadata,
                                        path_to_root,
                                        archs, revname, txn_n, prereqs, prereqs_size,
                                        &origtrav_state,
                                        &selected_pos,
                                        &selected_remain,
                                        &manifest_pos,
                                        &manifest_remain,
                                        &ignore_pos,
                                        &ignore_remain);

  if (first_node_status < 0)
    goto bail;

  if (!first_node_status || str_cmp (".", root_path))
    {
      revc_error (errname, "committed tree would lack a root?!? (%s)\n", root_path);
      goto bail;
    }

  invariant (!selected_remain);
  invariant (!revc_revtrav_path (errname, 0, &origtrav_state));

  ancestry_head = str_chr_index_n (ancestry, ancestry_size, '.');
  if (!ancestry_head)
    {
    corrupt_ancestry:
      revc_error (errname, "corrupt ancestry\n");
      goto bail;
    }
  ancestry_head = str_chr_index_n (ancestry_head + 1, ancestry_size - ((ancestry_head - 1) - ancestry), '.');
  if (!ancestry_head)
    goto corrupt_ancestry;
  ancestry_head += 1;
  ancestry_head_size = ancestry_head - ancestry;

  ancestry_tail = str_chr_index_n (ancestry, ancestry_size, 0);
  if (!ancestry_tail)
    goto corrupt_ancestry;
  ancestry_tail_size = ancestry_size - (ancestry_tail - ancestry);

  commit_ancestry_size = ancestry_head_size + str_length (revname) + ancestry_tail_size;
  commit_ancestry = lim_malloc (0, commit_ancestry_size);
  mem_move (commit_ancestry, ancestry, ancestry_head_size);
  mem_move (commit_ancestry + ancestry_head_size, revname, str_length (revname));
  mem_move (commit_ancestry + ancestry_head_size + str_length (revname), ancestry_tail, ancestry_tail_size);
  
  if (revc_string_blob (errname, commit_ancestry_blob, commit_ancestry, commit_ancestry_size))
    goto bail;

  if (revc_commit_needs_str_blob (errname,
                                  archs, txn_n,
                                  commit_ancestry_blob, commit_ancestry, commit_ancestry_size))
    goto bail;
  

  if (revc_make_ticket (errname, wq_revname, &ticket, revname, root_blob, root_metadata, commit_ancestry_blob))
    goto bail;

  wq_revname_len = str_length (wq_revname);

  if (revc_string_blob (errname, ticket_blob, ticket, str_length (ticket)))
    goto bail;

  if (revc_put_ticket (errname, archs, txn_n, ticket))
    goto bail;


  if (revc_get_blob (errname, &orig_ignore, &orig_ignore_size,
                     archs, orig_fq_revision, prereqs, prereqs_size,
                     orig_ignore_blob,
                     0))

  {
    t_uchar * tmp = 0;
    size_t tmp_size = 0;
    int status;

    if (revc_list_subtract_subtrees (errname, &tmp, &tmp_size,
                                     orig_ignore, orig_ignore_size,
                                     selected_subtrees, selected_subtrees_size))
      goto bail;


    status = revc_add_selected_subtrees (errname, &this_ignore, &this_ignore_size,
                                         tmp, tmp_size,
                                         ignore, ignore_size,
                                         selected_subtrees, selected_subtrees_size);

    lim_free (0, tmp);

    if (0 > status)
      goto bail;

    if (revc_string_blob (errname, this_ignore_blob, this_ignore, this_ignore_size))
      goto bail;

    if (revc_commit_needs_str_blob (errname,
                                    archs, txn_n,
                                    this_ignore_blob, this_ignore, this_ignore_size))
      goto bail;
  }

  commit_prereqs_size = wq_revname_len + 1 + REVC_BLOB_HINT_SET_SIZE + prereqs_size;
  commit_prereqs = lim_malloc (0, commit_prereqs_size);
  mem_move (commit_prereqs, wq_revname, wq_revname_len + 1);
  mem_move (commit_prereqs + wq_revname_len + 1, revc_commit_txn_has_hint (errname, archs, txn_n), REVC_BLOB_HINT_SET_SIZE);
  mem_move (commit_prereqs + wq_revname_len + 1 + REVC_BLOB_HINT_SET_SIZE, prereqs, prereqs_size);

  if (revc_string_blob (errname, commit_prereqs_blob, commit_prereqs, commit_prereqs_size))
    goto bail;

  if (revc_put_commit_prereqs (errname,
                               archs, txn_n,
                               commit_prereqs_blob, commit_prereqs, commit_prereqs_size))
    goto bail;


  if (revc_commit_file_size (errname, &total_file_size, archs, txn_n))
    goto bail;

  if (revc_commit_zip_size (errname, &total_zip_size, archs, txn_n))
    goto bail;

  if (revc_commit_n_blobs (errname, &n_blobs, archs, txn_n))
    goto bail;

  if (revc_commit_blob_contents_blob (errname, blob_contents_blob, archs, txn_n))
    goto bail;

  if (revc_make_commit_ticket (errname, fq_revname, &commit_ticket,
                               revname,
                               ticket_blob,
                               root_blob, root_metadata,
                               commit_ancestry_blob,
                               this_ignore_blob,
                               commit_prereqs_blob,
                               n_blobs,
                               total_file_size,
                               total_zip_size,
                               blob_contents_blob))
    goto bail;

  fq_revname_len = str_length (fq_revname);

  if (revc_put_commit_ticket (errname, archs, txn_n, commit_ticket))
    goto bail;

  commit_prereqs_tail = str_chr_index (commit_prereqs, 0) + 1;
  commit_prereqs_tail_size = (commit_prereqs_size - (commit_prereqs_tail - commit_prereqs));

  tree_prereqs_size = fq_revname_len + 1 + commit_prereqs_tail_size;
  tree_prereqs = lim_malloc (0, tree_prereqs_size);

  mem_move (tree_prereqs, fq_revname, fq_revname_len + 1);
  mem_move (tree_prereqs + fq_revname_len + 1, commit_prereqs_tail, commit_prereqs_tail_size);

  commit_ancestry_tail = str_chr_index (commit_ancestry, 0) + 1;
  commit_ancestry_tail_size = (commit_ancestry_size - (commit_ancestry_tail - commit_ancestry));
  commit_ancestry_head_size = ((1 + str_chr_index (1 + str_chr_index (commit_ancestry, '.'), '.')) - commit_ancestry);
  
  if (revc_parse_ancestry_line (errname, &this_anc, 0, 0, commit_ancestry))
    goto bail;

  cvt_ulong_to_decimal (this_anc_prefix, (t_ulong)(this_anc + 1));
  str_cat (this_anc_prefix, ".0.");

  if (revc_next_revname (errname, next_revname, revname))
    goto bail;
  

  tree_ancestry_size = (str_length (this_anc_prefix)
                        + str_length (next_revname)
                        + 1
                        + commit_ancestry_head_size
                        + wq_revname_len
                        + 1
                        + commit_ancestry_tail_size);
  tree_ancestry = lim_malloc (0, tree_ancestry_size);

  mem_move (tree_ancestry,
            this_anc_prefix,
            str_length (this_anc_prefix));
  mem_move (tree_ancestry + str_length (this_anc_prefix),
            next_revname,
            str_length (next_revname) + 1);
  mem_move (tree_ancestry + str_length (this_anc_prefix) + str_length (next_revname) + 1,
            commit_ancestry,
            commit_ancestry_head_size);
  mem_move (tree_ancestry + str_length (this_anc_prefix) + str_length (next_revname) + 1 + commit_ancestry_head_size,
            wq_revname,
            wq_revname_len + 1);
  mem_move ((tree_ancestry + str_length (this_anc_prefix) + str_length (next_revname) + 1 + commit_ancestry_head_size
             + wq_revname_len + 1),
            commit_ancestry_tail,
            commit_ancestry_tail_size);

  revc_rmrf (errname, prereqs_tmp_path);
  revc_rmrf (errname, ancestry_tmp_path);

  if (revc_string_blob (errname, commit_ticket_blob, commit_ticket, str_length (commit_ticket)))
    goto bail;

  if (revc_end_commit_txn (errname, archs, txn_n, revname, ticket_blob, commit_ticket_blob))
    goto bail;

  /* dangle: should be more txnal -- two-phase between tree and archive. */

  if (revc_zput_from_str (errname, 0, prereqs_tmp_path, tree_prereqs, tree_prereqs_size))
    goto bail;

  if (revc_put_from_str (errname, ancestry_tmp_path, tree_ancestry, tree_ancestry_size, 0))
    goto bail;

  if (revc_rename (errname, prereqs_tmp_path, prereqs_path))
    goto bail;

  if (revc_rename (errname, ancestry_tmp_path, ancestry_path))
    goto bail;

  revc_debug (errname, "commit\n  fq-rev: %s\n  root-blob: %s\n  root-metadata: %s\n", fq_revname, root_blob, root_metadata);

  revc_end_revtrav (errname, &origtrav_state, orig_root_blob, orig_root_metadata);


  answer = 0;

  goto leave;

}


static int
commit_next_node (const t_uchar * const errname,
                  t_uchar ** const root_path_ret,
                  t_uchar * const root_blob_ret,
                  t_uchar * const root_metadata_ret,
                  const t_uchar * const path_to_root,
                  struct revc_archives * const archs,
                  const t_uchar * const revision,
                  int txn_n,
                  t_uchar * const prereqs,
                  size_t prereqs_size,
                  struct revc_revtrav_state * orig_revtrav,
                  t_uchar ** selected_pos,
                  size_t * selected_remain,
                  t_uchar ** manifest_pos,
                  size_t * manifest_remain,
                  t_uchar ** ignore_pos,
                  size_t * ignore_remain)
{
  int has_next = -69;
  t_uchar * orig_next = 0;
  t_uchar * selected_next = 0;
  int cmp_selected_orig = -69;
  int orig_is_path_prefix = -69;
  t_uchar * dir = 0;
  size_t dir_size = 0;
  int answer = -69;

  enum 
    {
      take_from_tree,
      insert_from_orig,
      insert_from_tree,
      mixed_dir,
    } kind;


  has_next = revc_revtrav_path (errname, &orig_next, orig_revtrav);
  if (0 > has_next)
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, dir);
      return answer;
    committed_node_leave:
      answer = 1;
      goto leave;
    no_node_to_commit_leave:
      answer = 0;
      goto leave;
    }

  selected_next = (!*selected_remain ? 0 : *selected_pos);
  cmp_selected_orig = revc_traversal_cmp (selected_next, orig_next);
  orig_is_path_prefix = (selected_next && file_name_is_path_prefix (orig_next, selected_next));

  if (!*selected_remain && !has_next)
    goto no_node_to_commit_leave;
  else if (!*selected_remain)
    kind = insert_from_orig;
  else if (!has_next)
    kind = insert_from_tree;
  else if (!cmp_selected_orig)
    kind = take_from_tree;
  else if (orig_is_path_prefix)
    kind = mixed_dir;
  else if (cmp_selected_orig > 0)
    kind = insert_from_orig;
  else /* (!orig_is_path_prefix && (cmp_selected_orig < 0)) */
    kind = insert_from_tree;


  switch (kind)
    {
    default:
      {
        revc_error (errname, "internal error");
        panic ("abort");
        goto bail;
      }
    case insert_from_orig:
      {
        if (root_path_ret)
          *root_path_ret = str_save (0, orig_next);
        if (revc_revtrav_blob (errname, root_blob_ret, orig_revtrav))
          goto bail;
        if (revc_revtrav_metadata (errname, root_metadata_ret, orig_revtrav))
          goto bail;
        if (0 > revc_revtrav_next (errname, orig_revtrav, archs, revision, prereqs, prereqs_size, 1))
          goto bail;
        goto committed_node_leave;
      }

    case insert_from_tree:
    case take_from_tree:
      {
        t_uchar * node_manifest_pos;
        int manifest_status;
        t_uchar * subtree_pos;
        size_t subtree_size;
        t_uchar * subtree_end;
        size_t subtree_tail_size;
        size_t node_and_subtree_size;


        manifest_status = revc_list_find (errname, &node_manifest_pos,
                                          *manifest_pos, *manifest_remain,
                                          *selected_pos,
                                          0,
                                          revc_traversal_cmp);
        if (manifest_status < 0)
          goto bail;

        if (!manifest_status)
          {
            t_uchar * ignore_loc;
            int ignore_status;

            /* The user asked to commit a file from the local tree which 
             * is not in the manifest.  If the file is not in the tree
             * itself or is listed in the ignore list, we presume that 
             * the user meant "record that this file does not exist 
             * (e.g., has been deleted)" but if the file *does* exist 
             * in the tree *and* is not in the ignore list, then 
             * it's unclear whether the user forgot an `add' or 
             * truly meant a delete.
             */

            ignore_status = revc_list_find (errname, &ignore_loc,
                                            *ignore_pos, *ignore_remain,
                                            *selected_pos,
                                            1,
                                            revc_traversal_cmp);
            if (ignore_status < 0)
              goto bail;
            *ignore_remain -= (ignore_loc - *ignore_pos);
            *ignore_pos = ignore_loc;

            if (!ignore_status) /* file not in ignore list.  is it in the tree? */
              {
                t_uchar type;
                int status;
                t_uchar * path = file_name_in_vicinity (0, path_to_root, *selected_pos);

                status = revc_file_stats (errname, &type, 0, 0, path);
                lim_free (0, path);
                path = 0;

                if (status < 0)
                  goto bail;
                if (type)
                  {
                    revc_error (errname,
                                "attempt to explicitly commit unignored file not in manifest\n  node: %s\n",
                                *selected_pos);
                    goto bail;
                  }
              }
                
            goto no_node_to_commit_leave;
          }

        if (revc_list_find_subdir (errname,
                                   &subtree_pos, &subtree_size,
                                   *selected_pos,
                                   *manifest_pos,
                                   *manifest_remain))
          goto bail;

        subtree_end = subtree_pos + subtree_size;
        subtree_tail_size = *manifest_remain - (subtree_end - *manifest_pos);
        
        *manifest_pos = subtree_end;
        *manifest_remain = subtree_tail_size;

        node_and_subtree_size = (subtree_end - node_manifest_pos);

        revc_list_skip (selected_pos, selected_remain);

        while (*selected_remain && file_name_is_path_prefix (*manifest_pos, *selected_pos))
          {
            revc_warn (errname, "ignoring redundant subtree argument (%s)\n", *selected_pos);
            revc_list_skip (selected_pos, selected_remain);
          }

        if (commit_from_tree (errname, root_path_ret, root_blob_ret, root_metadata_ret,
                              path_to_root,
                              archs, revision, txn_n, prereqs, prereqs_size,
                              node_manifest_pos, node_and_subtree_size))
          goto bail;

        /* that advanced `selected_pos' and `manifest_pos' -- now skip over the
         * subtree in the ORIG traversal:
         */
        if (kind == take_from_tree)
          {
            if (0 > revc_revtrav_next (errname, orig_revtrav, archs, revision, prereqs, prereqs_size, 1))
              goto bail;
          }
        goto committed_node_leave;
      }

    case mixed_dir:
      {
        t_uchar * subtree_in_selected;
        size_t subtree_in_selected_size;
        t_uchar * subtree_in_selected_end;
        size_t subtree_in_selected_tail_size;

        t_uchar * subtree_in_manifest;
        size_t subtree_in_manifest_size;
        t_uchar * subtree_in_manifest_end;
        size_t subtree_in_manifest_tail_size;

        t_uchar * subtree_in_ignore;
        size_t subtree_in_ignore_size;
        t_uchar * subtree_in_ignore_end;
        size_t subtree_in_ignore_tail_size;

        if (revc_list_find_subdir (errname,
                                   &subtree_in_selected, &subtree_in_selected_size,
                                   orig_next,
                                   *selected_pos,
                                   *selected_remain))
          goto bail;
        subtree_in_selected_end = subtree_in_selected + subtree_in_selected_size;
        subtree_in_selected_tail_size = *selected_remain - (subtree_in_selected_end - *selected_pos);

        if (revc_list_find_subdir (errname,
                                   &subtree_in_manifest, &subtree_in_manifest_size,
                                   orig_next,
                                   *manifest_pos,
                                   *manifest_remain))
          goto bail;
        subtree_in_manifest_end = subtree_in_manifest + subtree_in_manifest_size;
        subtree_in_manifest_tail_size = *manifest_remain - (subtree_in_manifest_end - *manifest_pos);

        if (revc_list_find_subdir (errname,
                                   &subtree_in_ignore, &subtree_in_ignore_size,
                                   orig_next,
                                   *ignore_pos,
                                   *ignore_remain))
          goto bail;
        subtree_in_ignore_end = subtree_in_ignore + subtree_in_ignore_size;
        subtree_in_ignore_tail_size = *ignore_remain - (subtree_in_ignore_end - *ignore_pos);

        if (revc_revtrav_metadata (errname,
                                   root_metadata_ret,
                                   orig_revtrav))
          goto bail;

        if (revc_revtrav_next (errname,
                               orig_revtrav,
                               archs, revision, prereqs, prereqs_size,
                               0))
          goto bail;

        while (1)
          {
            int next_node_stat = -69;
            t_uchar * path = 0;
            t_uchar * fname;
            t_uchar blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
            t_uchar meta[REVC_MAX_METADATA_SIZE + 1];

            next_node_stat = commit_next_node (errname, &path, blob, meta,
                                               path_to_root,
                                               archs, revision, txn_n, prereqs, prereqs_size,
                                               orig_revtrav,
                                               &subtree_in_selected, &subtree_in_selected_size,
                                               &subtree_in_manifest, &subtree_in_manifest_size,
                                               &subtree_in_ignore, &subtree_in_ignore_size);
            if (next_node_stat < 0)
              goto bail;

            if (!next_node_stat)
              break;
              
            fname = str_chr_rindex (path, '/') + 1;
            if (!fname)
              fname = path;

            dir = (t_uchar *)lim_realloc (0, dir, dir_size + REVC_DIR_ENTRY_SIZE);
            revc_copy_fname_to_dir_entry (dir + dir_size, fname);
            revc_copy_blob_to_dir_entry (dir + dir_size, blob);
            revc_copy_metadata_to_dir_entry (dir + dir_size, meta);
            dir_size += REVC_DIR_ENTRY_SIZE;
            lim_free (0, path);
          }

        if (root_path_ret)
          *root_path_ret = str_save (0, orig_next);

        if (revc_string_blob (errname, root_blob_ret, dir, dir_size))
          goto bail;

        if (revc_commit_needs_str_blob (errname,
                                        archs, txn_n,
                                        root_blob_ret, dir, dir_size))
          goto bail;

        *selected_pos = subtree_in_selected_end;
        *selected_remain = subtree_in_selected_tail_size;

        *manifest_pos = subtree_in_manifest_end;
        *manifest_remain = subtree_in_manifest_tail_size;

        *ignore_pos = subtree_in_ignore_end;
        *ignore_remain = subtree_in_ignore_tail_size;

        goto committed_node_leave;
      }
    }
}


static int
commit_from_tree (const t_uchar * const errname,
                  t_uchar ** const root_path_ret,
                  t_uchar * const root_blob_ret,
                  t_uchar * const root_metadata_ret,
                  const t_uchar * const path_to_root,
                  struct revc_archives * archs,
                  const t_uchar * const revision,
                  int const txn_n,
                  t_uchar * const prereqs,
                  size_t const prereqs_size,
                  t_uchar * const manifest,
                  size_t const manifest_size)
{
  t_uchar * const node_to_commit = manifest;
  t_uchar * subtree_manifest = manifest;
  size_t subtree_manifest_size = manifest_size;
  t_uchar node_type;
  size_t node_size;
  t_uchar * node_path = 0;
  int answer = -69;
  t_uchar * dir_blob = 0;
  size_t dir_blob_size = 0;

  revc_list_skip (&subtree_manifest, &subtree_manifest_size);

  node_path = file_name_in_vicinity (0, path_to_root, node_to_commit);

  if (revc_file_stats (errname, &node_type, &node_size, root_metadata_ret, node_path))
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, node_path);
      lim_free (0, dir_blob);
      return answer;
    }

  switch (node_type)
    {
    case 'f':
      {
        if (subtree_manifest_size)
          {
            revc_error (errname,
                        "dir in manifest replaced by file in tree\n  node: %s\n  path: %s\n",
                        node_to_commit, node_path);
            goto bail;
          }
        if (revc_file_blob_committing (errname, root_blob_ret, archs, txn_n, node_path, node_size))
          goto bail;
        if (root_path_ret)
          *root_path_ret = str_save (0, node_to_commit);
        answer = 0;
        goto leave;
      }
    case 'd':
      {
        t_uchar * subtree_pos = subtree_manifest;
        size_t subtree_remain = subtree_manifest_size;

        while (subtree_remain)
          {
            t_uchar * subsubtree;
            size_t subsubtree_size;
            t_uchar blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
            t_uchar meta[REVC_MAX_METADATA_SIZE + 1];
            t_uchar * fname;

            if (!file_name_is_dir_prefix (node_to_commit, subtree_pos))
              {
                revc_error (errname, "corrupt manifest? (around %s)\n", node_to_commit);
                goto bail;
              }

            if (revc_list_find_subdir (errname,
                                       &subsubtree, &subsubtree_size,
                                       subtree_pos,
                                       subtree_pos, subtree_remain))
              goto bail;

            if (commit_from_tree (errname, 0, blob, meta,
                                  path_to_root,
                                  archs, revision, txn_n, prereqs, prereqs_size,
                                  subtree_pos, ((subsubtree + subsubtree_size) - subtree_pos)))
              goto bail;

            dir_blob = (t_uchar *)lim_realloc (0, dir_blob, dir_blob_size + REVC_DIR_ENTRY_SIZE);

            revc_copy_blob_to_dir_entry (dir_blob + dir_blob_size, blob);
            revc_copy_metadata_to_dir_entry (dir_blob + dir_blob_size, meta);

            fname = str_chr_rindex (subtree_pos, '/');
            if (fname)
              ++fname;
            else
              {
                revc_error (errname, "corrupt manifest? (around %s)\n", subtree_pos);
                goto bail;
              }

            revc_copy_fname_to_dir_entry (dir_blob + dir_blob_size, fname);

            dir_blob_size += REVC_DIR_ENTRY_SIZE;

            subtree_pos = subsubtree + subsubtree_size;
            subtree_remain = ((subtree_manifest + subtree_manifest_size) - subtree_pos);
          }

        if (revc_string_blob (errname, root_blob_ret, dir_blob, dir_blob_size))
          goto bail;

        if (root_path_ret)
          *root_path_ret = str_save (0, node_to_commit);


        if (revc_commit_needs_str_blob (errname,
                                        archs, txn_n,
                                        root_blob_ret, dir_blob, dir_blob_size))
          goto bail;

        answer = 0;
        goto leave;
      }
    case 0:
      {
        revc_error (errname, "file in manifest but missing from tree (%s)\n  path: %s\n", node_to_commit, node_path);
        goto bail;
      }
    default:
      {
        revc_error (errname, "internal error");
        panic ("abort");
        goto bail;
      }
    }
}




/* arch-tag: Tom Lord Mon Jun 27 17:52:38 2005 (commit.c)
 */
