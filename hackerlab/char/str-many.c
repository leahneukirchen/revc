/* str-many.c - vararg string functions
 *
 ****************************************************************
 * Copyright (C) 2002 Scott Parish
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include <stdarg.h>
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/char-class.h"
#include "hackerlab/char/str.h"


/* __STDC__ prototypes for static functions */
static void str_cat_many_va (t_uchar * to, va_list ap);
static size_t str_length_many_va (va_list ap);


/************************************************************************
 *(h2 "Vararg String Functions")
 * 
 * 
 */



/* str_end
 * 
 * An end-of-arglist marker for the vararg string functions.
 */
t_uchar str_end[1];



/*(c str_cat_many)
 * t_uchar * str_cat_many (const t_uchar * to, ...);
 * 
 * Append 0-terminated strings to the 0-terminated string `to'. The
 * strings should not overlap. The last argument should be the value
 * `str_end'.
 *
 * Returns `to'.
 */
t_uchar *
str_cat_many (t_uchar * to, ...)
{
  va_list ap;

  va_start (ap, to);
  str_cat_many_va (to, ap);
  va_end (ap);

  return to;
}


/*(c str_alloc_cat_many)
 * t_uchar * str_alloc_cat_many (alloc_limits limits, ...);
 * 
 * Allocate a new string large enough to hold the concatenation of
 * 0-terminated strings (including a final 0). Initialize the new string
 * with the concatenation of the given strings. The last argument should
 * be the value `str_end'.
 *
 * Allocate storage according to `limits'.  (See xref:"Allocation
 * With Limitations".)
 */
t_uchar *
str_alloc_cat_many (alloc_limits limits, ...)
{
  va_list ap;
  size_t len;

  va_start (ap, limits);
  len = str_length_many_va (ap);
  va_end (ap);

  if (!len)
    {
      return str_save (limits, "");
    }
  else
    {
      t_uchar * answer;

      answer = (t_uchar *)lim_malloc (limits, len + 1);
      if (!answer)
        return 0;
      *answer = 0;

      va_start (ap, limits);
      str_cat_many_va (answer, ap);
      va_end (ap);

      return answer;
    }
}


/*(c str_realloc_cat_many)
 * t_uchar * str_realloc_cat_many (alloc_limits limits,
 *                                 t_uchar * str1,
 *                                 ...)
 * 
 * Reallocate `str1' to be large enough to hold the concatenation of
 * 0-terminated strings (including a final 0). Initialize the new string
 * with the concatenation of the strings. The last argument should be the
 * value `str_end'.
 *
 * Allocate storage according to `limits'.  (See xref:"Allocation
 * With Limitations".)
 */
t_uchar *
str_realloc_cat_many (alloc_limits limits,
		      t_uchar * str1,
		      ...)
{
  va_list ap;
  size_t added_len;
  size_t str1_len;
  size_t total_len;

  va_start (ap, str1);
  added_len = str_length_many_va (ap);
  va_end (ap);

  if (!added_len)
    return str1;

  str1_len = str_length (str1);
  total_len = str1_len + added_len + 1;

  str1 = (t_uchar *)lim_realloc (limits, str1, total_len);
  if (!str1)
    return 0;
  str1[str1_len] = 0;

  va_start (ap, str1);
  str_cat_many_va (str1 + str1_len, ap);
  va_end (ap);

  return str1;
}



static void
str_cat_many_va (t_uchar * to, va_list ap)
{
  const t_uchar * from;

  while (*to)
    ++to;

  for (from = va_arg (ap, const t_uchar *); from != str_end; from = va_arg (ap, const t_uchar *))
    {
      if (from)
        while (*from)
          *to++ = *from++;
    }

  *to = 0;
}


static size_t
str_length_many_va (va_list ap)
{
  const t_uchar * str;
  size_t len;

  len = 0;

  for (str = va_arg (ap, const t_uchar *); str != str_end; str = va_arg (ap, const t_uchar *))
    {
      len += str_length (str);
    }

  return len;
}

