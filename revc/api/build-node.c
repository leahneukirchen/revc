/* build-node.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fs/file-names.h"
#include "revc/deps/error.h"
#include "revc/deps/mkdir.h"
#include "revc/deps/put-from-str.h"
#include "revc/deps/metadata-to-perms.h"
#include "revc/archives/get-blob.h"
#include "revc/api/constants.h"
#include "revc/api/build-node.h"

int
revc_build_node (const t_uchar * const errname,
                 t_uchar ** const manifest,
                 size_t * manifest_size,
                 struct revc_archives * archs,
                 const t_uchar * const fq_rev,
                 t_uchar * const prereq_list,
                 size_t const prereq_list_size,
                 const t_uchar * const logical_path,
                 const t_uchar * const output_node,
                 const t_uchar * const blob,
                 const t_uchar * const metadata)
{
  size_t logical_path_len = str_length (logical_path);

  *manifest = lim_realloc (0, *manifest, *manifest_size + logical_path_len + 1);
  mem_move (*manifest + *manifest_size, logical_path, logical_path_len + 1);
  *manifest_size += logical_path_len + 1;

  if (metadata[0] == 'd')
    {
      t_uchar * dir_listing = 0;
      size_t dir_listing_size = 0;
      int answer = -69;
      t_uchar * dir_pos;
      size_t dir_remain;

      if (revc_mkdir (errname, output_node, metadata))
        {
        dir_bail:
          answer = -1;
        dir_leave:
          lim_free (0, dir_listing);
          return answer;
        }

      if (0 > revc_get_blob (errname,
                             &dir_listing, &dir_listing_size,
                             archs,
                             fq_rev, prereq_list, prereq_list_size,
                             blob,
                             0))
        goto dir_bail;

      dir_pos = dir_listing;
      dir_remain = dir_listing_size;

      while (dir_remain >= REVC_DIR_ENTRY_SIZE)
        {
          t_uchar * sub_logical_path = file_name_in_vicinity (0, logical_path, REVC_DIR_ENTRY_FNAME(dir_pos));
          t_uchar * sub_path = file_name_in_vicinity (0, output_node, REVC_DIR_ENTRY_FNAME(dir_pos));
          int sub_result = revc_build_node (errname,
                                            manifest,
                                            manifest_size,
                                            archs,
                                            fq_rev, prereq_list, prereq_list_size,
                                            sub_logical_path,
                                            sub_path,
                                            REVC_DIR_ENTRY_BLOB_ADDR(dir_pos),
                                            REVC_DIR_ENTRY_METADATA(dir_pos));

          lim_free (0, sub_logical_path);
          lim_free (0, sub_path);
          if (sub_result)
            goto dir_bail;

          dir_pos += REVC_DIR_ENTRY_SIZE;
          dir_remain -= REVC_DIR_ENTRY_SIZE;
        }

      if (dir_remain)
        {
          revc_error (errname, "extra junk in directory\n");
          goto dir_bail;
        }

      answer = 0;
      goto dir_leave;
    }
  else if (metadata[0] == 'f')
    {
      t_uchar * file_contents = 0;
      size_t file_size = 0;
      int file_answer = -69;

      if (0 > revc_get_blob (errname,
                             &file_contents, &file_size,
                             archs,
                             fq_rev, prereq_list, prereq_list_size,
                             blob,
                             0))
        {
        file_bail:
          file_answer = -1;
        file_leave:
          lim_free (0, file_contents);
          return file_answer;
        }

      if (revc_put_from_str (errname, output_node, file_contents, file_size, metadata))
        goto file_bail;

      file_answer = 0;
      goto file_leave;
    }
  else
    {
      revc_error (errname, "unrecognized metadata\n  rev=%s\n  metadata=%s\n", fq_rev, metadata);
      return -1;
    }
}



/* arch-tag: Tom Lord Mon Jun 27 12:19:43 2005 (build-node.c)
 */
