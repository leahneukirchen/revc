/* parse-ancestry-line.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/char-class.h"
#include "hackerlab/fmt/cvt.h"
#include "revc/deps/error.h"
#include "revc/api/parse-ancestry-line.h"

int
revc_parse_ancestry_line (const t_uchar * const errname,
                          size_t * const ancestor_number_ret,
                          size_t * const merge_number_ret,
                          t_uchar ** const revision_ret,
                          t_uchar * line)
{
  int errn;
  t_uchar * anc_no_begin;
  t_uchar * anc_no_end;
  t_uchar * merge_no_begin;
  t_uchar * merge_no_end;
  t_uchar * revision_name_begin;
  t_ulong anc_no;
  t_ulong merge_no;

  anc_no_begin = line;

  anc_no_end = anc_no_begin;
  while (char_is_digit (*anc_no_end))
    ++anc_no_end;

  if (*anc_no_end != '.')
    {
    syntax:
      revc_error (errname, "illegal entry in ancestry file (%s)\n", line);
      return -1;
    }

  merge_no_begin = anc_no_end + 1;
  
  merge_no_end = merge_no_begin;
  while (char_is_digit (*merge_no_end))
    ++merge_no_end;

  if (*merge_no_end != '.')
    goto syntax;

  revision_name_begin = merge_no_end + 1;

  if (cvt_decimal_to_ulong (&errn, &anc_no, anc_no_begin, anc_no_end - anc_no_begin))
    goto syntax;

  if (cvt_decimal_to_ulong (&errn, &merge_no, merge_no_begin, merge_no_end - merge_no_begin))
    goto syntax;

  if (ancestor_number_ret)
    *ancestor_number_ret = (size_t)anc_no;

  if (merge_number_ret)
    *merge_number_ret = (size_t)merge_no;

  if (revision_ret)
    *revision_ret = revision_name_begin;

  return 0;
}




/* arch-tag: Tom Lord Sun Jun 26 15:12:05 2005 (parse-ancestry-line.c)
 */
