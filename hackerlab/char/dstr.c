/* dstr.c - dynamically sized string functions
 *
 ****************************************************************
 * Copyright (C) 1998, 2000 Thomas Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include "hackerlab/mem/mem.h"
#include "hackerlab/mem/alloc-limits.h"
#include "hackerlab/char/char-class.h"
#include "hackerlab/char/dstr.h"



struct dstr_handle
{
  size_t length;
  t_uchar * data;
  alloc_limits limits;
  int refs;
};



/************************************************************************
 *(h1 "Dynamic String Functions")
 *
 * These functions operate on dynamically resizable strings of `char'
 * values.
 */
/*(menu)
 */

/************************************************************************
 *(h2 "The t_dstr Type")
 * 
 * 
 * 
 */
/*(c t_dstr :category type)
 * typedef <unspecified pointer type> t_dstr;
 * 
 * The type of dynamic strings.
 */


/************************************************************************
 *(h2 "Allocating New Dynamic Strings")
 * 
 * 
 * 
 */


/*(c dstr_save)
 * t_dstr dstr_save (alloc_limits limits, const t_uchar * str);
 * 
 * Create a new dynamic string which is a copy of the 0-terminated
 * string `str'.
 */
t_dstr
dstr_save (alloc_limits limits, const t_uchar * str)
{
  return dstr_save_n (limits, str, str_length (str));
}


/*(c dstr_make_n)
 * t_dstr dstr_make_n (alloc_limits limits, size_t len);
 * 
 * Create a new dynamic string which is initialized to `len' NUL 
 * characters.
 */
t_dstr
dstr_make_n (alloc_limits limits, size_t len)
{
  t_dstr answer;

  answer = (t_dstr)lim_malloc (limits, sizeof (*answer));
  if (!answer)
    return 0;

  answer->limits = limits;
  answer->length = len;
  answer->refs = 1;
  answer->data = (t_uchar *)lim_malloc (limits, len + 1);

  if (answer->data)
    {
      mem_set0 (answer->data, len + 1);
    }
  else
    {
      lim_free (0, answer);
      answer = 0;
    }

  return answer;
}



/*(c dstr_save_n)
 * t_dstr dstr_save_n (alloc_limits limits, const t_uchar * str, size_t len);
 * 
 * Create a new dynamic string which is a copy of `len'-byte
 * string `str'.
 */
t_dstr
dstr_save_n (alloc_limits limits, const t_uchar * str, size_t len)
{
  t_dstr answer;

  answer = (t_dstr)lim_malloc (limits, sizeof (*answer));
  if (!answer)
    return 0;

  answer->limits = limits;
  answer->length = len;
  answer->refs = 1;
  answer->data = (t_uchar *)lim_malloc (limits, 1 + len);

  if (answer->data)
    {
      mem_move (answer->data, str, len);
      answer->data[len] = 0;
    }
  else
    {
      lim_free (0, answer);
      answer = 0;
    }

  return answer;
}



/*(c dstr_copy)
 * t_dstr dstr_copy (alloc_limits limits, t_dstr orig);
 * 
 * Allocate a new copy of a dynamic string.
 */
t_dstr
dstr_copy (alloc_limits limits, t_dstr orig)
{
  return dstr_save_n (limits, orig->data, orig->length);
}


/*(c dstr_take)
 * t_dstr dstr_take (alloc_limits limits,
 *                   t_uchar * str, size_t len);
 * 
 * Allocate a new dstr, "taking over" the allocated
 * string `str'.
 * 
 * If the new dstr can not be allocated, `str' is freed.
 */
t_dstr
dstr_take (alloc_limits limits,
           t_uchar * str, size_t len)
{
  t_dstr answer;

  answer = (t_dstr)lim_malloc (limits, sizeof (*answer));
  if (!answer)
    {
      lim_free (limits, str);
      return 0;
    }

  answer->limits = limits;
  answer->length = len;
  answer->refs = 1;
  answer->data = str;

  return answer;
}


/************************************************************************
 *(h2 "Reference Counting Dynamic Strings")
 * 
 * Dynamic strings are reference counte and are initially allocated with
 * a reference count of 1.
 * 
 */

/*(c dstr_ref)
 * void dstr_ref (t_dstr s);
 * 
 * Increase the reference count of `s'.
 */
void
dstr_ref (t_dstr s)
{
  if (s)
    ++s->refs;
}



/*(c dstr_unref)
 * void dstr_unref (t_dstr s);
 * 
 * Decrement the reference count of `s', freeing it if the
 * reference count drops to 0.
 */
void
dstr_unref (t_dstr s)
{
  if (s && !--s->refs)
    {
      lim_free (s->limits, s->data);
      lim_free (s->limits, s);
    }
}




/************************************************************************
 *(h2 "Dynamic String Data")
 * 
 * 
 * 
 */


/*(c dstr_length)
 * size_t dstr_length (t_dstr x);
 * 
 * Return the length of the dynamic string x.
 */
size_t
dstr_length (t_dstr x)
{
  if (!x)
    return 0;
  else
    return x->length;
}


/*(c dstr_data)
 * t_uchar * dstr_data (size_t * length, t_dstr x);
 * 
 * Return a pointer to the data for dynamic string `x'.  Store the
 * length of `x' in `*length' (if `length' is not 0).
 * 
 * The pointer remains valid until the string is freed or modified.
 * 
 * A final `NUL' character is promised to be appended to the data 
 * but this does not preclude there being other `NUL' characters
 * within the string.
 */
t_uchar *
dstr_data (size_t * length, t_dstr x)
{
  if (!x)
    {
      if (length)
        *length = 0;
      return 0;
    }
  else
    {
      if (length)
        *length = x->length;
      return x->data;
    }
}




/************************************************************************
 *(h2 "Computing Hash Values From Dynamic Strings")
 * 
 */

/*(c dstr_hash_n)
 * unsigned long dstr_hash (t_dstr str);
 * 
 * Compute an `unsigned long' hash value for a string.
 */
unsigned long
dstr_hash (t_dstr str)
{
  t_uchar * data;
  size_t len;

  data = dstr_data (&len, str);
  return str_hash_n (data, len);
}



/************************************************************************
 *(h2 "Dynamic String Comparisons")
 * 
 * 
 * 
 */

/*(c dstr_cmp)
 * int dstr_cmp (t_dstr a, t_dstr b);
 * 
 * Compare strings `a' and `b' returning -1 if `a' is lexically first,
 * 0 if the two strings are equal, 1 if `b' is lexically first.
 */
int
dstr_cmp (t_dstr ad, t_dstr bd)
{
  t_uchar * a;
  size_t a_l;
  t_uchar * b;
  size_t b_l;

  a = dstr_data (&a_l, ad);
  b = dstr_data (&b_l, bd);

  if ((a == b) && (a_l == b_l))
    return 0;
  return str_cmp_n (a, a_l, b, b_l);
}


/*(c dstr_casecmp)
 * int dstr_casecmp (t_dstr a, t_dstr b);
 * 
 * Compare strings `a' and `b', ignoring case, returning -1 if `a' is
 * lexically first, 0 if the two strings are equal, 1 if `b' is
 * lexically first.
 */
int
dstr_casecmp (t_dstr ad, t_dstr bd)
{
  t_uchar * a;
  size_t a_l;
  t_uchar * b;
  size_t b_l;

  a = dstr_data (&a_l, ad);
  b = dstr_data (&b_l, bd);

  if ((a == b) && (a_l == b_l))
    return 0;

  return str_casecmp_n (a, a_l, b, b_l);
}



/************************************************************************
 *(h2 "Dynamic String Concatenation")
 * 
 * 
 * 
 */

/*(c dstr_cat)
 * int dstr_cat (t_dstr to, t_dstr from);
 * 
 * Append `from' to `to', modifying `to'.
 * 
 * Return 0 normally, non-0 if an allocation failure occurs
 * (in which case `to' is unmodified).
 */
int
dstr_cat (t_dstr to, t_dstr from)
{
  size_t new_length;
  t_uchar * new_data;


  if (!from)
    return 0;

  new_length = to->length + from->length;
  new_data = lim_realloc (to->limits, to->data, new_length + 1);

  if (!new_data)
    return -1;

  to->data = new_data;
  mem_move (to->data + to->length, from->data, from->length);
  to->data[new_length] = 0;
  to->length = new_length;

  return 0;
}


/*(c dstr_append)
 * t_dstr dstr_append (alloc_limits limits,
 *                     t_dstr left, t_dstr right);
 * 
 * Construct a new dynamic string which is the concatenation of
 * dynamic strings `left' and `right'.
 */
t_dstr
dstr_append (alloc_limits limits,
             t_dstr left, t_dstr right)
{
  t_dstr answer = 0;
  size_t new_length;

  new_length = left->length + right->length;
  answer = dstr_make_n (limits, new_length);

  if (answer)
    {
      mem_move (answer->data, left->data, left->length);
      mem_move (answer->data + left->length, right->data, right->length);
    }
  
  return answer;
}


/************************************************************************
 *(h2 "Dynamic String Substring Extraction")
 * 
 * 
 * 
 */


/*(c dstr_shrink)
 * void dstr_shrink (t_dstr str, size_t start, size_t end);
 * 
 * Reduce `str' to only it's substring from `start' (inclusive)
 * to `end' (exclusive).
 */
void
dstr_shrink (t_dstr str, size_t start, size_t end)
{
  size_t new_length;
  t_uchar * new_data;

  new_length = end - start;
  mem_move (str->data, str->data + start, new_length);
  str->data[new_length] = 0;

  new_data = lim_realloc (str->limits, str->data, new_length + 1);
  if (new_data)
    str->data = new_data;
}


/*(c dstr_substr)
 * t_dstr dstr_substr (alloc_limits limits,
 *                     t_dstr str,
 *                     size_t start, size_t end);
 * 
 * Allocate a new dynamic string which is the subset of `str'
 * from `start' (inclusive) to `end' (exclusive).
 */
t_dstr
dstr_substr (alloc_limits limits,
             t_dstr str,
             size_t start, size_t end)
{
  size_t new_length;

  new_length = end - start;
  return dstr_save_n (limits, str->data + start, new_length);
}




/* tag: Tom Lord Thu Jan  1 13:08:18 2004 (dstr.c)
 */
