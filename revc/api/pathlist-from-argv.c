/* pathlist-from-argv.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/sort/qsort.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "hackerlab/mem/alloc-limits.h"
#include "revc/api/qsort-traversal-cmp.h"
#include "revc/api/pathlist-from-argv.h"

int
revc_pathlist_from_argv (const t_uchar * const errname,
                         t_uchar ** const list_ret,
                         size_t * const list_size_ret,
                         const t_uchar * const path_from_root,
                         int argc, char ** argv)
{
  size_t const path_from_root_length = str_length (path_from_root);
  size_t const argv_size = argc * sizeof (char *);
  char ** my_argv = (char **)lim_malloc (0, argv_size);
  size_t total_size;
  int x;
  t_uchar * answer = 0;
  size_t fill_size;


  mem_move ((t_uchar *)my_argv, (t_uchar *)argv, argv_size);
  quicksort ((void *)my_argv, (size_t)argc, sizeof (t_uchar *), revc_qsort_traversal_cmp, 0);

  total_size = 0;
  for (x = 0; x < argc; ++x)
    total_size += path_from_root_length + 1 + str_length (argv[x]) + 1;

  answer = lim_malloc (0, total_size);

  fill_size = 0;
  for (x = 0; x < argc; ++x)
    {
      size_t this_len = str_length (argv[x]);

      if ((this_len == 1) && (argv[x][0] == '.'))
        {
          mem_move (&answer[fill_size], path_from_root, path_from_root_length);
          fill_size += path_from_root_length;
          answer[fill_size] = 0;
          ++fill_size;
        }
      else
        {
          mem_move (&answer[fill_size], path_from_root, path_from_root_length);
          fill_size += path_from_root_length;
          answer[fill_size] = '/';
          ++fill_size;
          mem_move (&answer[fill_size], argv[x], this_len);
          fill_size += this_len;
          answer[fill_size] = 0;
          ++fill_size;
        }
    }

  invariant (fill_size <= total_size);

  lim_free (0, my_argv);
  *list_ret = answer;
  *list_size_ret = fill_size;

  return 0;
}




/* arch-tag: Tom Lord Thu Jun 23 14:05:12 2005 (pathlist-from-argv.c)
 */
