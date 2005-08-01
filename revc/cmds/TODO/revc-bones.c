/* recv-bones.c
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/machine/types.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "hackerlab/vu/safe.h"
#include "hackerlab/fs/file-names.h"
#include "revc/librevc/rmrf.h"
#include "revc/librevc/find-in-archives.h"
#include "revc/librevc/parse-ticket.h"
#include "revc/librevc/get-blob.h"
#include "revc/librevc/forward-blob.h"
#include "revc/librevc/perms-from-metadata.h"
#include "revc/librevc/constants.h"


/* __STDC__ prototypes for static functions */

static int build_dir (const t_uchar * const errname,
                      const t_uchar * const output_dir,
                      const t_uchar * const changeset_dir,
                      t_uchar * root_blob,
                      t_uchar * root_meta);
static int build_file (const t_uchar * const errname,
                       const t_uchar * const output_path,
                       const t_uchar * const changeset_dir,
                       t_uchar * blob,
                       t_uchar * meta);



int
main (int argc, char ** argv)
{
  int answer = 2;
  t_uchar * errname;
  int argx;
  t_uchar * output_dir = 0;
  t_uchar * output_dir_dir = 0;
  t_uchar * output_dir_tail = 0;
  t_uchar * tmp_output_dir = 0;
  t_uchar * changeset = 0;
  t_uchar * changeset_dir = 0;
  t_uchar * ticket_path = 0;
  int ticket_fd = -1;
  t_uchar * ticket = 0;
  size_t ticket_size = 0;
  t_uchar revision_name[REVC_MAX_FNAME_SIZE];
  t_uchar root_blob[REVC_MAX_BLOB_ADDR_SIZE];
  t_uchar root_meta[REVC_MAX_METADATA_SIZE];

  if (argc > 0)
    errname = argv[0];
  else
    {
    usage:
      safe_printfmt (2, "%s: usage -- %s [-o output-dir] changeset\n", errname, errname);
    bail:
      answer = 2;
    leave:
      lim_free (0, output_dir);
      lim_free (0, output_dir_dir);
      lim_free (0, output_dir_tail);
      lim_free (0, tmp_output_dir);
      lim_free (0, changeset);
      lim_free (0, changeset_dir);
      lim_free (0, ticket_path);
      lim_free (0, ticket);
      if (ticket_fd >= 0)
        safe_close (ticket_fd);
      return answer;
    }

  argx = 1;
  while ((argx < argc) && (argv[argx][0] == '-'))
    {
      if (!str_cmp (argv[argx], "-o"))
        {
          if ((argx + 1) == argc)
            goto usage;
          lim_free (0, output_dir);
          output_dir = str_save (0, argv[argx + 1]);
          argx += 2;
        }
      else
        goto usage;
    }

  if ((argx + 1) != argc)
    goto usage;

  changeset = str_save (0, argv[argx++]);

  if ((changeset[0] == '/') || !str_cmp_prefix ("./", changeset))
    {
      changeset_dir = str_save (0, changeset);
    }
  else
    {
      changeset_dir = revc_find_in_archives (errname, changeset);
      if (!changeset_dir)
        goto bail;
    }

  if (!output_dir)
    output_dir = str_save (0, ",,skeleton");

  output_dir_dir = file_name_directory_file (0, output_dir);
  if (!output_dir_dir)
    output_dir_dir = str_save (0, ".");

  output_dir_tail = file_name_tail (0, output_dir);

  tmp_output_dir = str_alloc_cat_many (0, output_dir_dir, "/,,", output_dir_tail, str_end);

  revc_rmrf (errname, tmp_output_dir);

  ticket_path = str_alloc_cat (0, changeset_dir, "/ticket");
  ticket_fd = safe_open (ticket_path, O_RDONLY, 0);
  safe_file_to_string (&ticket, &ticket_size, ticket_fd);
  safe_close (ticket_fd);
  ticket_fd = -1;

  if (revc_parse_ticket (errname,
                         revision_name, root_blob, root_meta,
                         ticket, ticket_size))
    goto bail;

  if (build_dir (errname,
                 tmp_output_dir,
                 changeset_dir,
                 root_blob, root_meta))
    goto bail;

  safe_rename (tmp_output_dir, output_dir);
  
  answer = 0;

  goto leave;
}


static int
build_dir (const t_uchar * const errname,
           const t_uchar * const output_dir,
           const t_uchar * const changeset_dir,
           t_uchar * root_blob,
           t_uchar * root_meta)
{
  t_uchar * root_dir = 0;
  size_t root_dir_size = 0;
  mode_t perm_bits;
  size_t pos;
  int answer = -1;

  if (revc_get_blob (errname, &root_dir, &root_dir_size, changeset_dir, root_blob))
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, root_dir);
      return answer;
    }

  if (revc_perms_from_metadata (errname, &perm_bits, root_meta))
    goto bail;

  safe_mkdir (output_dir, perm_bits);

  pos = 0;
  while (pos < root_dir_size)
    {
      t_uchar node_name[REVC_MAX_FNAME_SIZE + 1];
      t_uchar * node_path = 0;
      int status;

      mem_move (node_name, root_dir + pos, REVC_MAX_FNAME_SIZE);
      node_name[REVC_MAX_FNAME_SIZE] = 0;

      node_path = str_alloc_cat_many (0, output_dir, "/", node_name, str_end);

      if (REVC_DIR_ENTRY_METADATA(root_dir + pos)[0] == 'd')
        status = build_dir (errname, node_path, changeset_dir,
                            REVC_DIR_ENTRY_BLOB_ADDR (root_dir + pos),
                            REVC_DIR_ENTRY_METADATA (root_dir + pos));
      else
        status = build_file (errname, node_path, changeset_dir,
                             REVC_DIR_ENTRY_BLOB_ADDR (root_dir + pos),
                             REVC_DIR_ENTRY_METADATA (root_dir + pos));

      lim_free (0, node_path);

      if (status)
        goto bail;

      pos += REVC_DIR_ENTRY_SIZE;
    }

  answer = 0;
  goto leave;
}


static int
build_file (const t_uchar * const errname,
            const t_uchar * const output_path,
            const t_uchar * const changeset_dir,
            t_uchar * blob,
            t_uchar * meta)
{
  t_uchar * output_dir = 0;
  t_uchar * output_tail = 0;
  t_uchar * tmp_output_path = 0;
  int out_fd;
  mode_t perm_bits;
  int answer = -1;

  if (revc_perms_from_metadata (errname, &perm_bits, meta))
    {
    bail:
      answer = -1;
    leave:
      lim_free (0, output_dir);
      lim_free (0, output_tail);
      lim_free (0, tmp_output_path);
      return answer;
    }

  output_dir = file_name_directory_file (0, output_path);
  if (!output_dir)
    output_dir = str_save (0, ".");

  output_tail = file_name_tail (0, output_path);
  tmp_output_path = str_alloc_cat_many (0, output_dir, "/,,", output_tail, str_end);

  revc_rmrf (errname, tmp_output_path);

  out_fd = safe_open (tmp_output_path, O_WRONLY | O_CREAT | O_EXCL, perm_bits);

  if (revc_forward_blob (errname, out_fd, changeset_dir, blob))
    goto bail;

  safe_close (out_fd);

  safe_rename (tmp_output_path, output_path);

  answer = 0;
  goto leave;
}





/* arch-tag: Tom Lord Tue Jun 21 14:38:11 2005 (cmds/revc-bones.c)
 */


