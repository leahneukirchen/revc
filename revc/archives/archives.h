/* archives.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__ARCHIVES_H
#define INCLUDE__API__ARCHIVES_H


#include "hackerlab/machine/types.h"
#include "revc/api/constants.h"

struct revc_get_txn
{
  ssize_t next;
  t_uchar * revision;
  t_uchar * prereqs;
  size_t prereqs_size;
};

struct revc_commit_txn
{
  ssize_t next;
  t_uchar * tmp_dir;
  int get_n;
  t_uchar ** blob_names;
  size_t n_blobs;
  size_t total_zip_size;
  size_t total_file_size;
  t_uchar has_hint[REVC_BLOB_HINT_SET_SIZE];
  t_uchar needs_hint[REVC_BLOB_HINT_SET_SIZE];
};

struct revc_archives
{
  struct revc_commit_txn _commits[REVC_MAX_SIMULTANEOUS_COMMITS];
  int _first_free_commmit;

  struct revc_get_txn _gets[REVC_MAX_SIMULTANEOUS_COMMITS];
  int _first_free_get;

  t_uchar * _archive_dir;
};

#define REVC_ARCHIVES_NIL_INIT {}



/* automatically generated __STDC__ prototypes */
#endif  /* INCLUDE__API__ARCHIVES_H */


/* arch-tag: Tom Lord Sun Jun 26 17:37:44 2005 (archives.h)
 */
