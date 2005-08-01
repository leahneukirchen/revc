/* lint-loop-main.c
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/machine/types.h"
#include "hackerlab/char/str.h"
#include "hackerlab/vu/safe.h"
#include "hackerlab/fs/file-names.h"
#include "revc/api/list-skip.h"
#include "revc/api/check-list.h"
#include "revc/api/traversal-cmp.h"
#include "revc/api/list-find-subdir.h"
#include "revc/api/traverse-subtree.h"
#include "revc/api/lint.h"


/* __STDC__ prototypes for static functions */



static int show (const t_uchar * const errname,
                 void * vclosure,
                 const t_uchar * const parent_path_from_root,
                 const t_uchar * const path_from_root,
                 const t_uchar * const fname,
                 t_uchar ** const mfst, size_t * const mlen,
                 t_uchar ** const ign, size_t * const ilen);



struct show_closure
{
  const t_uchar * cwd_path_from_root;
  int has_new;
  int has_missing;
  t_revc_lint_callback_fn new_callback;
  t_revc_lint_callback_fn missing_callback;
  void * rock;
};



int
revc_lint (const t_uchar * const errname,
           int * unclean_ret,
           const t_uchar * const path_from_root,
           t_uchar * const manifest, size_t manifest_size,
           t_uchar * const ignore, size_t ignore_size,
           t_revc_lint_callback_fn new_callback,
           t_revc_lint_callback_fn missing_callback,
           void * rock)
{
  t_uchar * manifest_pos = 0;
  size_t manifest_remain = 0;
  t_uchar * ignore_pos = 0;
  size_t ignore_remain = 0;
  struct show_closure closure;
  int answer = -1;

  if (revc_list_find_subdir (errname, &manifest_pos, &manifest_remain, path_from_root, manifest, manifest_size))
    return -1;

  if (revc_list_find_subdir (errname, &ignore_pos, &ignore_remain, path_from_root, ignore, ignore_size))
    return -1;

  closure.cwd_path_from_root = path_from_root;
  closure.has_new = 0;
  closure.has_missing = 0;
  closure.new_callback = new_callback;
  closure.missing_callback = missing_callback;
  closure.rock = rock;

  answer = revc_traverse_subtree (errname,
                                  path_from_root,
                                  show,
                                  (void *)&closure,
                                  &manifest_pos, &manifest_remain,
                                  &ignore_pos, &ignore_remain);

  if ((answer > 0) && unclean_ret && ((new_callback && closure.has_new) || (missing_callback && closure.has_missing)))
    *unclean_ret = 1;

  return answer;
}


static int
show (const t_uchar * const errname,
      void * vclosure,
      const t_uchar * const parent_path_from_root,
      const t_uchar * const path_from_root,
      const t_uchar * const fname,
      t_uchar ** const mfst, size_t * const mlen,
      t_uchar ** const ign, size_t * const ilen)
{
  struct show_closure * closure = (struct show_closure *)vclosure;

  if (revc_check_list (errname, *mfst, *mlen) || revc_check_list (errname, *ign, *ilen))
    return -1;

  /* no file at all? (end of traversal?) */

  if (!fname)
    {
      if (*mlen && closure->missing_callback)
        {
          closure->has_missing = 1;
          while (*mlen)
            {
              if (0 > closure->missing_callback (errname,
                                                 *mfst,
                                                 str_length (*mfst),
                                                 closure->cwd_path_from_root,
                                                 closure->rock))
                return -1;
              revc_list_skip (mfst, mlen);
            }
        }

      return 0;
    }


  /* is this file in the ignore list? */

  while (*ilen)
    {
      int const cmp = revc_traversal_cmp (path_from_root, *ign);

      if (cmp < 0)
        break;
      else if (!cmp)
        {
          revc_list_skip (ign, ilen);
          return 0;
        }
      else /* if (cmp > 0) */
        revc_list_skip (ign, ilen);
    }

  /* stuff ahead of this in manifest? that stuff is missing;
   * plus, this might be in the manifest list.
   */

  while (*mlen)
    {
      int const cmp = revc_traversal_cmp (path_from_root, *mfst);

      if (cmp < 0)
        {
          /* nothing less than or equal to this in the manifest.
           * This is new.
           */
          break;
        }
      else if (!cmp)
        {
          /* found in the manifest; neither new nor missing
           */
          revc_list_skip (mfst, mlen);
          return 1;
        }
      else /* if (cmp > 0) */
        {
          /* something ahead of this file in the manifest.
           * That manifest entry is missing.
           */
          if (closure->missing_callback)
            {
              closure->has_missing = 1;
              if (0 > closure->missing_callback (errname,
                                                 *mfst,
                                                 str_length (*mfst),
                                                 closure->cwd_path_from_root,
                                                 closure->rock))
                return -1;
            }
          revc_list_skip (mfst, mlen);
        }
    }

  /* is this file one whose name suggests it should be, by
   * default, ignored (since this is a *lint* loop).
   */
  if ((fname[0] < '/') || (fname[0] && ('~' == fname[str_length (fname) - 1])))
    return 0;

  /* must be new. */
  if (closure->new_callback)
    {
      closure->has_new = 1;
      if (0 > closure->new_callback (errname,
                                     path_from_root,
                                     str_length (path_from_root),
                                     closure->cwd_path_from_root,
                                     closure->rock))
        return -1;
    }

  /* Returning 1 ensures that if this is a directory,
   * the traversal will descend into it.  That means
   * that a new directory is listed along with 
   * all the new subnodes it contains.
   */
  return 1;
}




/* arch-tag: Tom Lord Fri Jun 24 10:50:40 2005 (api/lint.c)
 */


