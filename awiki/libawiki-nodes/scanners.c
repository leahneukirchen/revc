/* scanners.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "awiki/libawiki-nodes/scanners.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */

Regexp_decl (dash3_line, "^[[:blank:]]*----*[[:blank:]]*\n", REG_EXTENDED);


/* code */

int
awiki_scan_til_dash3_line_or_end (t_buffer_point * pt,
                                  t_buffer_point * end_pt)
{
  while (!bufpt_is_at_end (pt, end_pt))
    {
      if (bufpt_looking_at_re (pt, end_pt, Regexp(dash3_line), 0))
        return 1;

      if (0 > bufpt_next_line (pt, end_pt))
        return -1;
    }

  return 0;
}



/* arch-tag: Tom Lord Sat Nov  6 10:51:01 2004 (scanners.c)
 */
