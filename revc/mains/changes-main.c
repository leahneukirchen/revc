/* changes-main.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/char/str.h"
#include "hackerlab/arrays/ar.h"
#include "hackerlab/fs/file-names.h"
#include "hackerlab/mem/alloc-limits.h"
#include "libdiff/diff.h"
#include "revc/deps/usage.h"
#include "revc/deps/error.h"
#include "revc/deps/dangle.h"
#include "revc/deps/fmt.h"
#include "revc/deps/file-stats.h"
#include "revc/deps/get-to-string.h"
#include "revc/archives/archives.h"
#include "revc/archives/init-archives.h"
#include "revc/archives/commit-ticket.h"
#include "revc/archives/get-blob.h"
#include "revc/archives/close-archives.h"
#include "revc/api/opts.h"
#include "revc/api/diff-line.h"
#include "revc/api/str-to-lines.h"
#include "revc/api/is-binary-string.h"
#include "revc/api/file-blob.h"
#include "revc/api/traversal-cmp.h"
#include "revc/api/pathlist-from-argv.h"
#include "revc/api/tree-setup.h"
#include "revc/api/list-skip.h"
#include "revc/api/list-skip-subtree.h"
#include "revc/api/begin-revtrav.h"
#include "revc/api/revtrav-next.h"
#include "revc/api/revtrav-blob.h"
#include "revc/api/revtrav-path.h"
#include "revc/api/revtrav-metadata.h"
#include "revc/api/end-revtrav.h"
#include "revc/mains/changes-main.h"


/* __STDC__ prototypes for static functions */



static const void * line_index (const void * base,
                                size_t idx,
                                void * ign);
static int line_cmp (const void * va,
                     const void * vb,
                     void * ign);
static void print_diffs (const t_uchar * const errname,
                         struct diff_edit * edits,
                         struct revc_diff_line * orig_lines,
                         struct revc_diff_line * mod_lines,
                         const t_uchar * const  orig,
                         size_t const orig_size,
                         const t_uchar * const  mod,
                         size_t const mod_size);



const t_uchar * const revc_changes_usage = "[-r revision] subtree*";

int
revc_changes_main (int argc, char ** argv)
{
  t_uchar * errname = 0;
  int argx = 0;
  int answer = 2;
  t_uchar * selected_subtrees = 0;
  size_t selected_subtrees_size = 0;
  t_uchar * path_to_root = 0;
  t_uchar * path_from_root = 0;
  t_uchar * manifest = 0;
  size_t manifest_size = 0;
  t_uchar * ignore = 0;
  size_t ignore_size = 0;
  t_uchar * ancestry = 0;
  size_t ancestry_size = 0;
  t_uchar * prereqs = 0;
  size_t prereqs_size = 0;
  struct revc_archives archs = REVC_ARCHIVES_NIL_INIT;
  t_uchar orig_fq_revision[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar orig_revision[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar orig_root_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar orig_root_metadata[REVC_MAX_METADATA_SIZE + 1];
  t_uchar orig_ignore_blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  struct revc_revtrav_state origtrav_state;
  t_uchar * select_pos = 0;
  size_t select_remain = 0;
  t_uchar * manifest_pos = 0;
  size_t manifest_remain = 0;
  t_uchar * orig = 0;
  size_t orig_size = 0;
  t_uchar * mod = 0;
  size_t mod_size = 0;

  struct revc_opts opts[] =
    {
      { REVC_END_OPTS, }
    };

  if (revc_opts (&errname, &argx, opts, argc, argv, revc_changes_usage))
    {
    bail:
      answer = 2;
    leave:
      lim_free (0, errname);
      lim_free (0, path_to_root);
      lim_free (0, path_from_root);
      lim_free (0, orig);
      lim_free (0, mod);
      if (revc_close_archives (errname, &archs))
        answer = 2;
      return answer;
    }

  if (revc_tree_setup (errname,
                       &path_to_root, &path_from_root,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       &manifest, &manifest_size, &ignore, &ignore_size, &ancestry, &ancestry_size, &prereqs, &prereqs_size))
    goto bail;


  if (argc != argx)
    {
      if (revc_pathlist_from_argv (errname,
                                   &selected_subtrees, &selected_subtrees_size,
                                   path_from_root,
                                   argc - argx, argv + argx))
        goto bail;
      argx = argc;
    }
  else
    {
      selected_subtrees = str_save (0, ".");
      selected_subtrees_size = 2;
    }

  if (revc_init_archives (errname, &archs))
    goto bail;

  if (revc_commit_ticket (errname,
                          orig_fq_revision,
                          0,
                          orig_revision,
                          orig_root_blob,
                          orig_root_metadata,
                          0,
                          orig_ignore_blob,
                          0,
                          0, 0, 0, 0,
                          0, 0,
                          &archs,
                          prereqs))
    goto bail;


  if (revc_begin_revtrav (errname, &origtrav_state, orig_root_blob, orig_root_metadata))
    goto bail;


  select_pos = selected_subtrees;
  select_remain = selected_subtrees_size;

  manifest_pos = manifest;
  manifest_remain = manifest_size;

  while (select_remain)
    {
      /* skip to this subtree in the traversal of ORIG */
      while (1)
        {
          int c;
          int x;
          t_uchar * orig_path = 0;

          x = revc_revtrav_path (errname, &orig_path, &origtrav_state);
          if (x < 0)
            goto bail;

          if (!x)
            break;

          c = revc_traversal_cmp (orig_path, select_pos);
          lim_free (0, orig_path);

          if (c >= 0)
            break;

          if (0 > revc_revtrav_next (errname,
                                     &origtrav_state,
                                     &archs, orig_revision,
                                     prereqs, prereqs_size,
                                     1))
            goto bail;
        }
      
      /* skip to this subtree in the tree manifest */
      while (manifest_remain)
        {
          int c;

          c = revc_traversal_cmp (manifest_pos, select_pos);
          if (c >= 0)
            break;

          revc_list_skip_subtree (&manifest_pos, &manifest_remain);
        }
      
      
      /* iterate over the listings of the two subtrees */

      while (1)
        {
          t_uchar * orig_path = 0;
          t_uchar * tree_path = 0;
          int x;
          int orig_is_in = -69;
          int mod_is_in = -42;

          x = revc_revtrav_path (errname, &orig_path, &origtrav_state);
          if (x < 0)
            goto bail;

          if (!orig_path && !manifest_remain)
            {
              break;
            loop_bail:
              lim_free (0, orig_path);
              lim_free (0, tree_path);
              goto bail;
            }

          if (manifest_remain)
            {
              tree_path = file_name_in_vicinity (0, path_to_root, manifest_pos);
            }

          orig_is_in = (orig_path ? file_name_is_path_prefix (select_pos, orig_path) : 0);
          mod_is_in = (manifest_remain ? file_name_is_path_prefix (select_pos, manifest_pos) : 0);


          if (orig_is_in && !mod_is_in)
            {
            as_del:
              revc_fmt (errname, "* del %s\n", orig_path);
              if (0 > revc_revtrav_next (errname,
                                         &origtrav_state,
                                         &archs, orig_revision,
                                         prereqs, prereqs_size,
                                         1))
                goto loop_bail;
            }
          else if (!orig_is_in && mod_is_in)
            {
            as_add:
              revc_fmt (errname, "* add %s\n", manifest_pos);
              revc_list_skip_subtree (&manifest_pos, &manifest_remain);
            }
          else
            {
              if (!orig_is_in)
                {
                skip_both_subtrees:
                  if (0 > revc_revtrav_next (errname,
                                             &origtrav_state,
                                             &archs, orig_revision,
                                             prereqs, prereqs_size,
                                             1))
                    goto loop_bail;
                  revc_list_skip_subtree (&manifest_pos, &manifest_remain);
                }
              else
                {
                  int orig_mod_cmp = -13;
                  t_uchar orig_meta[REVC_MAX_METADATA_SIZE + 1];
                  t_uchar orig_blob[REVC_MAX_METADATA_SIZE + 1];
                  t_uchar tree_meta[REVC_MAX_METADATA_SIZE + 1];
                  t_uchar tree_type;
                  size_t tree_size;

                  orig_mod_cmp = revc_traversal_cmp (orig_path, manifest_pos);
                  if (orig_mod_cmp < 0)
                    goto as_del;
                  else if (orig_mod_cmp > 0)
                    goto as_add;

                  if (0 > revc_revtrav_metadata (errname, orig_meta, &origtrav_state))
                    goto loop_bail;

                  if (0 > revc_revtrav_blob (errname, orig_blob, &origtrav_state))
                    goto loop_bail;

                  if (0 > revc_file_stats (errname, &tree_type, &tree_size, tree_meta, tree_path))
                    goto loop_bail;

                  if (orig_meta[0] != tree_type)
                    {
                      if (!tree_type)
                        {
                          revc_fmt (errname, "* missing %s\n", manifest_pos);
                          goto skip_both_subtrees;
                        }
                      else if (orig_meta[0] == 'd')
                        {
                          revc_fmt (errname, "* dir->file %s\n", manifest_pos);
                          goto skip_both_subtrees;
                        }
                      else
                        {
                          revc_fmt (errname, "* file->dir %s\n", manifest_pos);
                          goto skip_both_subtrees;
                        }
                    }
                  else
                    {
                      if (str_cmp (orig_meta, tree_meta))
                        {
                          revc_fmt (errname, "* meta %s (%s -> %s)\n", manifest_pos, orig_meta, tree_meta);
                        }

                      if (tree_type == 'f')
                        {
                          t_uchar tree_blob[REVC_MAX_METADATA_SIZE + 1];

                          if (0 > revc_file_blob (errname, tree_blob, tree_path, tree_size))
                            goto loop_bail;

                          if (str_cmp (orig_blob, tree_blob))
                            {
                              if (0 > revc_get_blob (errname, &orig, &orig_size,
                                                     &archs, orig_revision,
                                                     prereqs, prereqs_size,
                                                     orig_blob, 0))
                                goto loop_bail;

                              if (0 > revc_get_to_string (errname, &mod, &mod_size, tree_path))
                                goto loop_bail;

                              if (revc_is_binary_string (orig, orig_size)
                                  || revc_is_binary_string (mod, mod_size))
                                {
                                  revc_fmt (errname, "* mod-binary %s\n  (%s -> %s)\n", manifest_pos, orig_blob, tree_blob);
                                }
                              else
                                {
                                  struct revc_diff_line * orig_lines = 0;
                                  struct revc_diff_line * mod_lines = 0;
                                  struct diff_edit * edits = 0;

                                  if (revc_str_to_lines (errname, &orig_lines, orig, orig_size))
                                    goto loop_bail;

                                  if (revc_str_to_lines (errname, &mod_lines, mod, mod_size))
                                    goto loop_bail;

                                  if (0 > diff (&edits,
                                                orig_lines, ar_size ((void *)orig_lines, 0, sizeof (struct revc_diff_line)),
                                                mod_lines, ar_size ((void *)mod_lines, 0, sizeof (struct revc_diff_line)),
                                                line_index, line_cmp, 0,
                                                0))
                                    {
                                      revc_error (errname, "error computing diff (%s)\n", manifest_pos);
                                      goto loop_bail;
                                    }

                                  revc_fmt (errname, "\014\n* modified %s\n  (%s -> %s)\n", manifest_pos, orig_blob, tree_blob);
                                  print_diffs (errname, edits, orig_lines, mod_lines, orig, orig_size, mod, mod_size);
                                  revc_fmt (errname, "\n");
                                }

                            }
                        }
                      
                    }
                  if (0 > revc_revtrav_next (errname,
                                             &origtrav_state,
                                             &archs, orig_revision,
                                             prereqs, prereqs_size,
                                             0))
                    goto loop_bail;
                  revc_list_skip (&manifest_pos, &manifest_remain);
                }
            }

          lim_free (0, orig_path);
        }

      revc_list_skip_subtree (&select_pos, &select_remain);
    }


  answer = 0;
  
  goto leave;
}


static const void *
line_index (const void * base,
            size_t idx,
            void * ign)
{
  return (void *)((struct revc_diff_line *)base + idx);
}


static int
line_cmp (const void * va,
          const void * vb,
          void * ign)
{
  struct revc_diff_line * a = (struct revc_diff_line *)va;
  struct revc_diff_line * b = (struct revc_diff_line *)vb;

  if (a->hash == b->hash)
    return 0;
  else if (a->hash < b->hash)
    return -1;
  else
    return 1;
}

static void
print_diffs (const t_uchar * const errname,
             struct diff_edit * edits,
             struct revc_diff_line * orig_lines,
             struct revc_diff_line * mod_lines,
             const t_uchar * const  orig,
             size_t const orig_size,
             const t_uchar * const  mod,
             size_t const mod_size)
{
  ssize_t i;

  for (i = 0; i < ar_size ((void *)edits, 0, sizeof (struct diff_edit)); i++)
    {
      struct diff_edit * e = &edits[i];
  
      switch (e->op)
        {
        case DIFF_MATCH:
          break;
        case DIFF_INSERT:
        case DIFF_DELETE:
          {
            if (((i + 1) >= ar_size ((void *)edits, 0, sizeof (struct diff_edit)))
                || ((e[1].op == DIFF_MATCH)
                    && (e->op != e[1].op)))
              {
                char marker = ((e->op == DIFF_DELETE) ? '<' : '>');
                size_t first_line = e->off;
                size_t end_line = e->off + e->len;
                struct revc_diff_line * lines_of_interest = ((e->op == DIFF_DELETE) ? orig_lines : mod_lines);
                const t_uchar * str_of_interest = ((e->op == DIFF_DELETE) ? orig : mod);
                size_t l;
                
                revc_fmt (errname, "%ld,%lda\n", (t_ulong)first_line, (t_ulong)end_line);
                for (l = first_line; l < end_line; ++l)
                  revc_fmt (errname, "%c %.*s",
                            marker,
                            (int)(lines_of_interest[l].end - lines_of_interest[l].start),
                            str_of_interest + lines_of_interest[l].start);
              }
            else
              {
                int del_index = (e->op == DIFF_INSERT) ? 1 : 0;
                int ins_index = !del_index;
                size_t first_del_line = e[del_index].off;
                size_t end_del_line = e[del_index].off + e[del_index].len;
                size_t first_ins_line = e[ins_index].off;
                size_t end_ins_line = e[ins_index].off + e[ins_index].len;
                size_t l;

                revc_fmt (errname, "%ld,%ldr%ld,%ld\n",
                          (t_ulong)first_del_line, (t_ulong)end_del_line,
                          (t_ulong)first_ins_line, (t_ulong)end_ins_line);
                for (l = first_del_line; l < end_del_line; ++l)
                  revc_fmt (errname, " - %.*s",
                            (int)(orig_lines[l].end - orig_lines[l].start),
                            orig + orig_lines[l].start);
                revc_fmt (errname, "---\n");
                for (l = first_ins_line; l < end_ins_line; ++l)
                  revc_fmt (errname, " +  %.*s",
                            (int)(mod_lines[l].end - mod_lines[l].start),
                            mod + mod_lines[l].start);
                ++i;
              }
          }
        }
    }
}

#if 0
static int
edit_cmp (void * va, void * vb, void * ign)

{
  struct diff_edit * a = (struct diff_edit *)va;
  struct diff_edit * b = (struct diff_edit *)vb;

  if (a->off == b->off)
    return 0;
  else if (a->off < b->off)
    return -1;
  else
    return 1;
}
#endif


/* arch-tag: Tom Lord Sun Jul 10 09:11:10 2005 (mains/changes-main.c)
 */
