/* lsr.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/deps/get-to-string.h"
#include "revc/api/find-root.h"
#include "revc/api/list-skip.h"
#include "revc/api/traversal-cmp.h"
#include "revc/api/traverse-subtree.h"
#include "revc/api/lsr.h"


/* __STDC__ prototypes for static functions */



static int show_unignored (const t_uchar * const errname,
                           void * vclosure,
                           const t_uchar * const parent_path_from_root,
                           const t_uchar * const path_from_root,
                           const t_uchar * const fname,
                           t_uchar ** const mfst, size_t * const mlen,
                           t_uchar ** const ign, size_t * const ilen);



struct show_unignored_closure
{
  t_uchar * path_from_root;
  t_revc_lsr_fn callback;
  void * rock;
};


int
revc_lsr (const t_uchar * const errname,
          t_revc_lsr_fn const callback,
          void * const rock)
{
  t_uchar * path_to_root = 0;
  t_uchar * path_from_root = 0;
  t_uchar * path_to_ignore_file = 0;
  t_uchar * manifest_pos = 0;
  size_t manifest_remain = 0;
  t_uchar * ignore_list = 0;
  size_t ignore_list_size = 0;
  t_uchar * ignore_pos = 0;
  size_t ignore_remain = 0;
  int traverse_status;

 if (revc_find_root (errname, &path_to_root, &path_from_root))
    return -1;

  path_to_ignore_file = str_alloc_cat (0, path_to_root, "/.revc/ignore");

  if (revc_get_to_string (errname, &ignore_list, &ignore_list_size, path_to_ignore_file))
    return -1;

  lim_free (0, path_to_ignore_file);
  path_to_ignore_file = 0;

  ignore_pos = ignore_list;
  ignore_remain = ignore_list_size;

  {
    struct show_unignored_closure closure;

    closure.path_from_root = path_from_root;
    closure.callback = callback;
    closure.rock = rock;

    traverse_status = revc_traverse_subtree (errname,
                                             path_from_root,
                                             show_unignored,
                                             (void *)&closure,
                                             &manifest_pos, &manifest_remain,
                                             &ignore_pos, &ignore_remain);
  }
  lim_free (0, path_to_root);
  path_to_root = 0;
  lim_free (0, path_from_root);
  path_from_root = 0;
  lim_free (0, ignore_list);
  ignore_list = 0;

  if (traverse_status)
    return -1;

  return 0;
}


static int
show_unignored (const t_uchar * const errname,
                void * vclosure,
                const t_uchar * const parent_path_from_root,
                const t_uchar * const path_from_root,
                const t_uchar * const fname,
                t_uchar ** const mfst, size_t * const mlen,
                t_uchar ** const ign, size_t * const ilen)
{
  struct show_unignored_closure * closure = (struct show_unignored_closure *)vclosure;

  if (!fname)
    return 0;

  if (fname[0] < '/')
    return 0;

  if ('~' == fname[str_length (fname) - 1])
    return 0;

  while (*ilen && (revc_traversal_cmp (*ign, path_from_root) < 0))
    revc_list_skip (ign, ilen);

  if (*ilen && !revc_traversal_cmp (*ign, path_from_root))
    return 0;

  return closure->callback (errname, path_from_root, str_length (path_from_root), closure->path_from_root, closure->rock);
}





/* arch-tag: Tom Lord Thu Jun 23 09:57:14 2005 (lsr.c)
 */
