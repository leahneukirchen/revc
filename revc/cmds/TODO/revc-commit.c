/* revc-commit.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

/* includes */

#include "hackerlab/machine/types.h"
#include "hackerlab/os/errno-to-string.h"
#include "hackerlab/arrays/ar.h"
#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fmt/cvt.h"
#include "hackerlab/vu/safe.h"
#include "hackerlab/vu/vu.h"
#include "hackerlab/fs/file-names.h"
#include "libsha1/sha1.h"
#include "revc/librevc/constants.h"
#include "revc/librevc/begin-saved-commit.h"
#include "revc/librevc/finish-saved-commit.h"
#include "revc/librevc/insertion-archive.h"
#include "revc/librevc/file-data.h"
#include "revc/librevc/zip-file.h"
#include "revc/librevc/zip-string.h"
#include "revc/librevc/fill-in-metadata.h"
#include "revc/librevc/inventory-skip.h"
#include "revc/librevc/begin-put-blob.h"
#include "revc/librevc/finish-put-blob.h"
#include "revc/librevc/rmrf.h"
#include "revc/librevc/find-root.h"



struct commit_closure
{
  struct sha1_ctx sha1_ctx;
  t_uchar sha1[20];
  size_t n_blobs;
  size_t total_stream_length;
};


/* __STDC__ prototypes for static functions */



static int write_ticket (const t_uchar * const errname,
                         struct commit_closure * closure,
                         const t_uchar * const dir,
                         const t_uchar * const revision_name,
                         const t_uchar root_blob_addr[],
                         const t_uchar root_metadata[]);
static int write_commit_ticket (const t_uchar * const errname,
                                struct commit_closure * closure,
                                const t_uchar * const dir,
                                const t_uchar * const revision_name,
                                const t_uchar root_blob_addr[],
                                const t_uchar root_metadata[]);
static int commit_subdir (t_uchar * errname,
                          struct commit_closure * closure,
                          t_uchar * const blob_addr_ret,
                          t_uchar * const metadata_ret,
                          const t_uchar * const tmp_output_dir_name,
                          const t_uchar * const path_from_root,
                          t_uchar ** const target_pos,
                          size_t * const target_remain,
                          t_uchar ** const root_set_pos,
                          size_t * const root_set_remain,
                          t_uchar ** const from_this_pos,
                          size_t * const from_this_remain);
static int from_this_dir (t_uchar * errname,
                          struct commit_closure * closure,
                          t_uchar * const blob_addr_ret,
                          t_uchar * const metadata_ret,
                          const t_uchar * const tmp_output_dir_name,
                          const t_uchar * const path_from_root,
                          struct stat * statb,
                          t_uchar ** const target_pos,
                          size_t * const target_remain,
                          t_uchar ** const from_this_pos,
                          size_t * const from_this_remain);
static int from_this_file (t_uchar * errname,
                           struct commit_closure * closure,
                           t_uchar * const blob_addr_ret,
                           t_uchar * const metadata_ret,
                           const t_uchar * const tmp_output_dir_name,
                           const t_uchar * const path_from_root,
                           struct stat * statb,
                           t_uchar ** const target_pos,
                           size_t * const target_remain,
                           t_uchar ** const from_this_pos,
                           size_t * const from_this_remain);



int
main (int argc, char * argv[])
{
  struct commit_closure closure;
  t_uchar * errname;
  int answer = 2;
  int argx;
  t_uchar * revision_name = 0;
  t_uchar * output_dir_name = 0;
  t_uchar * tmp_output_dir_name = 0;
  t_uchar * path_to_root = 0;
  t_uchar * path_from_root = 0;
  t_uchar * path_to_manifest_file = 0;
  t_uchar * path_to_ignore_file = 0;
  t_uchar * manifest_list = 0;
  size_t manifest_list_size = 0;
  t_uchar * ignore_list = 0;
  size_t ignore_list_size = 0;
  t_uchar * root_set_for_this_commit_list = 0;
  size_t root_set_for_this_commit_list_size = 0;
  t_uchar * from_this_tree_list = 0;
  size_t from_this_tree_list_size = 0;
  t_uchar * target_list = 0;
  size_t target_list_size = 0;
  t_uchar root_blob_addr[REVC_MAX_BLOB_ADDR_SIZE];
  t_uchar root_metadata[REVC_MAX_METADATA_SIZE];
  t_uchar * target_pos, * root_set_pos, * from_this_pos;
  size_t target_remain, root_set_remain, from_this_remain;

  sha1_init_ctx (&closure.sha1_ctx);
  closure.n_blobs = 0;
  closure.total_stream_length = 0;

  if (argc > 0)
    errname = argv[0];
  else
    {
      errname = "revc-commit";
    usage:
      safe_printfmt (2, "%s: usage -- %s [-o dir] [-r revision-name] subtree*\n", errname, errname);
    bail:
      answer = 2;
    leave:
      lim_free (0, path_to_root);
      lim_free (0, path_from_root);
      lim_free (0, path_to_manifest_file);
      lim_free (0, path_to_ignore_file);
      lim_free (0, manifest_list);
      lim_free (0, ignore_list);
      lim_free (0, revision_name);
      lim_free (0, output_dir_name);
      return answer;
    }

  argx = 1;
  while ((argx < argc) && (argv[argx][0] == '-'))
    {
      if (!str_cmp (argv[argx], "-o"))
        {
          if ((argx + 1) == argc)
            goto usage;
          lim_free (0, output_dir_name);
          output_dir_name = str_save (0, argv[argx + 1]);
          tmp_output_dir_name = str_alloc_cat (0, ",,", output_dir_name);
          argx += 2;
        }
      else if (!str_cmp (argv[argx], "-r"))
        {
          if ((argx + 1) == argc)
            goto usage;
          lim_free (0, revision_name);
          revision_name = str_save (0, argv[argx + 1]);
          argx += 2;
        }
      else
        goto usage;
    }

  if (argx != argc)
    {
      safe_printfmt (2, "%s: partial commits not handled yet\n", errname);
      goto usage;
    }

  if (!revision_name)
    {
      if (output_dir_name)
        revision_name = str_save (0, output_dir_name);
      else
        {
          safe_printfmt (2, "%s: default revision names not supported yet\n", errname);
          goto bail;
        }
    }

  if (!output_dir_name)
    {
      tmp_output_dir_name = revc_begin_saved_commit (errname, revision_name);
    }

  if (revc_find_root (errname, &path_to_root, &path_from_root))
    goto bail;

  if (str_cmp (path_from_root, "."))
    {
      safe_printfmt (2, "%s: partial commit not handled yet (cd to the tree root)\n", errname);
      goto bail;
    }

  path_to_manifest_file = str_alloc_cat (0, path_to_root, "/.revc/manifest");
  path_to_ignore_file = str_alloc_cat (0, path_to_root, "/.revc/ignored");

  {
    int const manifest_fd = safe_open (path_to_manifest_file, O_RDONLY, 0);
    int const ignore_fd = safe_open (path_to_ignore_file, O_RDONLY, 0);

    safe_file_to_string (&manifest_list, &manifest_list_size, manifest_fd);
    safe_file_to_string (&ignore_list, &ignore_list_size, ignore_fd);

    safe_close (manifest_fd);
    safe_close (ignore_fd);
  }

  /* since we support only ancestorless, whole-tree commit:
   */

  root_set_for_this_commit_list = 0;
  root_set_for_this_commit_list_size = 0;

  from_this_tree_list = lim_malloc (0, manifest_list_size);
  mem_move (from_this_tree_list, manifest_list, manifest_list_size);
  from_this_tree_list_size = manifest_list_size;

  target_list = lim_malloc (0, manifest_list_size);
  mem_move (target_list, manifest_list, manifest_list_size);
  target_list_size = manifest_list_size;

  /* create the output dir
   */
  revc_rmrf (errname, tmp_output_dir_name);
  safe_mkdir (tmp_output_dir_name, 0777);

  /* write necessary blobs and compute the root blob */

  target_pos = target_list;
  target_remain = target_list_size;
  root_set_pos = root_set_for_this_commit_list;
  root_set_remain = root_set_for_this_commit_list_size;
  from_this_pos = from_this_tree_list;
  from_this_remain = from_this_tree_list_size;

  safe_chdir (path_to_root);

  if (commit_subdir (errname,
                     &closure,
                     root_blob_addr, root_metadata,
                     tmp_output_dir_name,
                     path_from_root,
                     &target_pos, &target_remain,
                     &root_set_pos, &root_set_remain,
                     &from_this_pos, &from_this_remain))
    goto bail;

  /* write the ticket */
  if (write_ticket (errname,
                    &closure,
                    tmp_output_dir_name,
                    revision_name,
                    root_blob_addr, root_metadata))
    goto bail;
  
  /* write the ticket */
  if (write_commit_ticket (errname,
                           &closure,
                           tmp_output_dir_name,
                           revision_name,
                           root_blob_addr, root_metadata))
    goto bail;
  
  /* rename the dir into place, failing gracefully if not. */
  if (!output_dir_name)
    {
      if (revc_finish_saved_commit (errname, revision_name, tmp_output_dir_name))
        goto bail;
    }
  else
    {
      int errn;

      if (vu_rename (&errn, tmp_output_dir_name, output_dir_name))
        {
          safe_printfmt (2, "%s: error renaming output directory (%s)\n  output left in %s\n",
                         errname, errno_to_string (errn), tmp_output_dir_name);
          goto bail;
        }
    }

  answer = 0;
  goto leave;
}


static int
write_ticket (const t_uchar * const errname,
              struct commit_closure * closure,
              const t_uchar * const dir,
              const t_uchar * const revision_name,
              const t_uchar root_blob_addr[],
              const t_uchar root_metadata[])
{
  t_uchar * const file = str_alloc_cat (0, dir, "/ticket");
  int const fd = safe_open (file, O_CREAT | O_WRONLY | O_EXCL, 0666);
  t_uchar * str =  0;
  size_t str_size;
  t_uchar header[1024];
  size_t header_size;

  str = str_alloc_cat (0, "revision: ", revision_name);
  str = str_realloc_cat (0, str, "\nroot-blob: ");
  str = str_realloc_cat_n (0, str, root_blob_addr, REVC_MAX_BLOB_ADDR_SIZE);
  str = str_realloc_cat (0, str, "\nroot-metadata: ");
  str = str_realloc_cat_n (0, str, root_metadata, REVC_MAX_METADATA_SIZE);
  str = str_realloc_cat (0, str, "\n");
  str_size = str_length (str);

  mem_move (header, "ticket", 7);
  header_size = 7 + cvt_ulong_to_decimal (header + 7, (t_ulong)header_size);

  sha1_process_bytes (&closure->sha1_ctx, header, header_size);
  closure->total_stream_length += header_size;
  closure->total_stream_length += str_size;

  safe_write_retry (fd, str, str_size);
  safe_close (fd);
  lim_free (0, file);
  return 0;
}


static int
write_commit_ticket (const t_uchar * const errname,
                     struct commit_closure * closure,
                     const t_uchar * const dir,
                     const t_uchar * const revision_name,
                     const t_uchar root_blob_addr[],
                     const t_uchar root_metadata[])
{
  t_uchar * const file = str_alloc_cat (0, dir, "/commit-ticket");
  int const fd = safe_open (file, O_CREAT | O_WRONLY | O_EXCL, 0666);
  t_uchar * str =  0;
  size_t str_size;
  t_uchar sha1_hex[41];
  t_uchar decimal_stream_size[sizeof (t_ulong) * 4 + 1];
  t_uchar decimal_n_blobs[sizeof (t_ulong) * 4 + 1];


  sha1_finish_ctx (closure->sha1, &closure->sha1_ctx);
  sha1_to_hex (sha1_hex, closure->sha1);
  sha1_hex[40] = 0;

  (void)cvt_ulong_to_decimal (decimal_stream_size, (t_ulong)closure->total_stream_length);
  (void)cvt_ulong_to_decimal (decimal_n_blobs, (t_ulong)closure->n_blobs);

  str = str_alloc_cat (0, "revision: ", revision_name);
  str = str_realloc_cat (0, str, "\nroot-blob: ");
  str = str_realloc_cat_n (0, str, root_blob_addr, REVC_MAX_BLOB_ADDR_SIZE);
  str = str_realloc_cat (0, str, "\nroot-metadata: ");
  str = str_realloc_cat_n (0, str, root_metadata, REVC_MAX_METADATA_SIZE);
  str = str_realloc_cat (0, str, "\nstream-sha1: ");
  str = str_realloc_cat (0, str, sha1_hex);
  str = str_realloc_cat (0, str, "\nstream-size: ");
  str = str_realloc_cat (0, str, decimal_stream_size);
  str = str_realloc_cat (0, str, "\nstream-n-items: ");
  str = str_realloc_cat (0, str, decimal_n_blobs);
  str = str_realloc_cat (0, str, "\n");
  str_size = str_length (str);

  safe_write_retry (fd, str, str_size);
  safe_close (fd);
  lim_free (0, file);
  return 0;
}


static int
commit_subdir (t_uchar * errname,
               struct commit_closure * closure,
               t_uchar * const blob_addr_ret,
               t_uchar * const metadata_ret,
               const t_uchar * const tmp_output_dir_name,
               const t_uchar * const path_from_root,
               t_uchar ** const target_pos,
               size_t * const target_remain,
               t_uchar ** const root_set_pos,
               size_t * const root_set_remain,
               t_uchar ** const from_this_pos,
               size_t * const from_this_remain)
{
  struct stat statb;

  safe_stat (path_from_root, &statb);

  if (!S_ISDIR (statb.st_mode))
    {
      safe_printfmt (2, "%s: bad directory confusion\n", errname);
      return -1;
    }

  return from_this_dir (errname,
                        closure,
                        blob_addr_ret,
                        metadata_ret,
                        tmp_output_dir_name,
                        path_from_root,
                        &statb,
                        target_pos,
                        target_remain,
                        from_this_pos,
                        from_this_remain);
}





static int
from_this_dir (t_uchar * errname,
               struct commit_closure * closure,
               t_uchar * const blob_addr_ret,
               t_uchar * const metadata_ret,
               const t_uchar * const tmp_output_dir_name,
               const t_uchar * const path_from_root,
               struct stat * statb,
               t_uchar ** const target_pos,
               size_t * const target_remain,
               t_uchar ** const from_this_pos,
               size_t * const from_this_remain)
{
  t_uchar * dir_listing = 0;
  size_t dir_size = 0;
  t_uchar blob_addr[REVC_MAX_BLOB_ADDR_SIZE];
  size_t blob_addr_size;
  t_uchar sha1[20];

  while (*target_remain
         && file_name_is_dir_prefix (path_from_root, *target_pos))
    {
      t_uchar * const file_name = str_chr_rindex (*target_pos, '/') + 1;
      size_t file_name_len = str_length (file_name);
      t_uchar * dir_entry = (t_uchar *)ar_push ((void **)&dir_listing, 0, REVC_DIR_ENTRY_SIZE);
      t_uchar * item_path_from_root = str_alloc_cat_many (0, path_from_root, "/", file_name, str_end);
      struct stat item_statb;

      dir_size += REVC_DIR_ENTRY_SIZE;

      if (file_name_len > REVC_MAX_FNAME_SIZE)
        {
          safe_printfmt (2, "%s: filename too long (%s)\n", errname, file_name);
        bail:
          lim_free (0, item_path_from_root);
          return -1;
        }

      mem_move (REVC_DIR_ENTRY_FNAME (dir_entry), file_name, file_name_len);
      mem_set0 (REVC_DIR_ENTRY_FNAME (dir_entry) + file_name_len, REVC_MAX_FNAME_SIZE - file_name_len);

      safe_stat (item_path_from_root, &item_statb);

      revc_inventory_skip (target_pos, target_remain);
      revc_inventory_skip (from_this_pos, from_this_remain);

      if (S_ISDIR (item_statb.st_mode))
        {
          if (from_this_dir (errname,
                             closure,
                             REVC_DIR_ENTRY_BLOB_ADDR (dir_entry),
                             REVC_DIR_ENTRY_METADATA (dir_entry),
                             tmp_output_dir_name,
                             item_path_from_root,
                             &item_statb,
                             target_pos, target_remain,
                             from_this_pos, from_this_remain))
            goto bail;
        }
      else
        {
          if (from_this_file (errname,
                              closure,
                              REVC_DIR_ENTRY_BLOB_ADDR (dir_entry),
                              REVC_DIR_ENTRY_METADATA (dir_entry),
                              tmp_output_dir_name,
                              item_path_from_root,
                              &item_statb,
                              target_pos, target_remain,
                              from_this_pos, from_this_remain))
            goto bail;
        }
      
    }

  sha1_buffer (sha1, dir_listing, dir_size);
  sha1_to_hex (blob_addr, sha1);
  blob_addr[40] = '.';
  blob_addr_size = 41 + cvt_ulong_to_decimal (blob_addr + 41, (t_ulong)dir_size);
  mem_set0 (blob_addr + blob_addr_size, sizeof (blob_addr) - blob_addr_size);

  /* if not in ancestors */
  {
    size_t zipped_size = 0;
    int blob_fd;
    t_uchar decimal_size[sizeof (t_ulong) * 4 + 1];

    blob_fd = revc_begin_put_blob (errname, tmp_output_dir_name, blob_addr);
    if (blob_fd < 0)
      return -1;

    sha1_process_bytes (&closure->sha1_ctx, "blob", 5);

    if (revc_zip_string (errname, &zipped_size, &closure->sha1_ctx, blob_fd, dir_listing, dir_size))
      return -1;
    
    (void)cvt_ulong_to_decimal (decimal_size, (t_ulong)zipped_size);
    sha1_process_bytes (&closure->sha1_ctx, decimal_size, str_length (decimal_size) + 1);

    ++closure->n_blobs;
    closure->total_stream_length += sizeof ("blob");
    closure->total_stream_length += 1 + str_length (decimal_size);
    closure->total_stream_length += zipped_size ;

    if (revc_finish_put_blob (errname, tmp_output_dir_name, blob_addr, blob_fd))
      return -1;
  }

  if (blob_addr_ret)
    mem_move (blob_addr_ret, blob_addr, sizeof (blob_addr));

  if (metadata_ret)
    revc_fill_in_metadata (metadata_ret, statb);

  ar_free ((void **)&dir_listing, 0);

  return 0;
}






static int
from_this_file (t_uchar * errname,
                struct commit_closure * closure,
                t_uchar * const blob_addr_ret,
                t_uchar * const metadata_ret,
                const t_uchar * const tmp_output_dir_name,
                const t_uchar * const path_from_root,
                struct stat * statb,
                t_uchar ** const target_pos,
                size_t * const target_remain,
                t_uchar ** const from_this_pos,
                size_t * const from_this_remain)
{
  t_uchar local_blob_addr[REVC_MAX_BLOB_ADDR_SIZE];

  if (revc_file_data (errname,
                      local_blob_addr,
                      metadata_ret,
                      path_from_root,
                      statb))
    return -1;

  /* if !found in ancestor */

  {
    size_t zipped_size = 0;
    int blob_fd;
    t_uchar decimal_size[sizeof (t_ulong) * 4 + 1];

    blob_fd = revc_begin_put_blob (errname, tmp_output_dir_name, local_blob_addr);
    if (blob_fd < 0)
      return -1;

    sha1_process_bytes (&closure->sha1_ctx, "blob", 5);

    if (revc_zip_file (errname, &zipped_size, &closure->sha1_ctx, blob_fd, path_from_root))
      return -1;

    (void)cvt_ulong_to_decimal (decimal_size, (t_ulong)zipped_size);
    sha1_process_bytes (&closure->sha1_ctx, decimal_size, str_length (decimal_size) + 1);

    ++closure->n_blobs;
    closure->total_stream_length += sizeof ("blob");
    closure->total_stream_length += 1 + str_length (decimal_size);
    closure->total_stream_length += zipped_size ;
    
    if (revc_finish_put_blob (errname, tmp_output_dir_name, local_blob_addr, blob_fd))
      return -1;
  }

  if (blob_addr_ret)
    mem_move (blob_addr_ret, local_blob_addr, sizeof (local_blob_addr));

  return 0;
}



/* arch-tag: Tom Lord Tue Jun 21 09:12:42 2005 (revc-commit.c)
 */
