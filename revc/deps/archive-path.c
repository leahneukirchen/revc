/* archive-path.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/os/stdlib.h"
#include "hackerlab/char/str.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/vu/safe.h"
#include "revc/deps/archive-path.h"



t_uchar * 
revc_archive_path (const t_uchar * const errname,
                   size_t * const size_ret)
{
  t_uchar * env = getenv ("REVC_ARCHIVES");
  t_uchar * home = getenv ("HOME");
  t_uchar * dflt = 0;
  t_uchar * answer = 0;
  size_t answer_size = 0;

  if (!env)
    {
      if (home)
        {
          dflt = str_alloc_cat (0, home, "/.revc-commits");
          env = dflt;
        }
      else
        {
          safe_printfmt (2, "%s: REVC_ARCHIVES not set in environment\n", errname);
          return 0;
        }
    }

  while (1)
    {
      t_uchar * colon;
      size_t new_path_elt_length;
      size_t new_answer_size;
      t_uchar * new_answer = 0;
      

      colon = str_chr_index (env, ':');
      if (!colon)
        colon = env + str_length (env);

      new_path_elt_length = (colon - env);
      new_answer_size = answer_size + new_path_elt_length + 1;

      new_answer = lim_malloc (0, new_answer_size);
      mem_move (new_answer, answer, answer_size);
      mem_move (new_answer + answer_size, env, new_path_elt_length);
      new_answer[new_answer_size - 1] = 0;

      lim_free (0, answer);
      answer = new_answer;
      answer_size = new_answer_size;
      
      if (*colon)
        env = colon + 1;
      else
        break;
    }

  lim_free (0, dflt);

  if (size_ret)
    *size_ret = answer_size;

  return answer;
}





/* arch-tag: Tom Lord Thu Jun 23 09:03:42 2005 (archive-path.c)
 */
