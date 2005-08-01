/* next-revname.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/char-class.h"
#include "hackerlab/char/str.h"
#include "revc/deps/error.h"
#include "revc/api/constants.h"
#include "revc/api/next-revname.h"

int
revc_next_revname (const t_uchar * const errname,
                   t_uchar * const rev_ret,
                   const t_uchar * const revision)
{
  int answer = -69;
  size_t len;
  t_uchar new_name[REVC_MAX_REVNAME_SIZE + 1];
  t_uchar * end;
  t_uchar * digs;
  int all_nines;


  len = str_length_n (revision, REVC_MAX_REVNAME_SIZE + 1);
  if (len > REVC_MAX_REVNAME_SIZE)
    {
    too_long:
      revc_error (errname, "revision name too long (%s)\n", revision);
    bail:
      answer = -1;
    leave:
      if (!answer && rev_ret)
        str_cpy (rev_ret, new_name);
      return answer;
    }

  str_cpy (new_name, revision);

  end = new_name;
  while (*end && (*end != '/'))
    ++end;

  *end = 0;

  if (end == new_name)
    {
      revc_error (errname, "empty revision name?\n");
      goto bail;
    }

  digs = end - 1;
  all_nines = 1;
  while ((digs >= new_name) && char_is_digit (*digs))
    {
      if (*digs != '9')
        all_nines = 0;
      --digs;
    }
  ++digs;

  if (digs == end)
    {
      if ((len + 3) > REVC_MAX_REVNAME_SIZE)
        goto too_long;
      end[0] = '.';
      end[1] = '0';
      end[2] = 0;
      answer = 0;
      goto leave;
    }
  else if (all_nines)
    {
      t_uchar * t;

      if ((len + 1) > REVC_MAX_REVNAME_SIZE)
        goto too_long;

      end[0] = '0';
      end[1] = 0;
      digs[0] = '1';

      t = digs + 1;
      while (t < end)
        {
          *t = '0';
          ++t;
        }
      answer = 0;
      goto leave;
    }
  else
    {
      t_uchar * s;

      s = (end - 1);
      while ((s > digs) && (*s == 9))
        {
          *s = '0';
          --s;
        }

      ++(*s);
      answer = 0;
      goto leave;
    }
}



/* arch-tag: Tom Lord Thu Jul  7 09:11:23 2005 (next-revname.c)
 */
