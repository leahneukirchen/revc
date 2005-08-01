/* udstr.c:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/bugs/panic.h"
#include "hackerlab/uni/invariant.h"
#include "hackerlab/strings/udstr.h"



/************************************************************************
 *(h1 "Dynamic Unicode String Functions"
 *     :includes ("hackerlab/strings/udstr.h"))
 *
 * 
 */

/*(c t_udstr :category type)
 * typedef <unspecified> t_udstr;
 *
 * `t_udstr' values are mutable, resizable Unicode strings.
 * 
 * Internally, strings of this type may be of any supported encoding
 * form and may contain any codepoint valid in that encoding.  The
 * length of an udstr (pronounced "uhd-stir") in both encoding values
 * and code-points is explicitly recorded.
 */
struct udstr_handle
{
  int refs;

  uni_string str;
  enum uni_encoding_scheme enc;
  ustr_cv_index_t cv_len;
  ustr_cp_index_t cp_len;

  alloc_limits limits;
};


/*(menu)
 */



/* __STDC__ prototypes for static functions */
static enum uni_encoding_scheme pick_fw_of (enum uni_encoding_scheme a,
                                            enum uni_encoding_scheme b);
static void take_new_data (t_udstr dstr, t_udstr new_data);


/************************************************************************
 *(h2 "udstr Naming Conventions")
 * 
 * Some `udstr' functions require users to pass string indexes or lengths
 * as parameters.   Such parameters are always expressed in units of
 * code values within the relevent encoding form.
 * 
 * Functions having the suffix `_x' work by modifying their first string
 * argument rather than by returning a newly allocated string.
 * 
 * Finally, `_fw' functions are the "full width" varients (see below).
 * 
 */

/************************************************************************
 *(h2 "Convervative Width vs. Full Width Unicode String Functions")
 * 
 * Normally, `udstr' functions choose the encoding form of their
 * output strings (or the strings they modify) by copying the encoding
 * of the first string argument.  Thus, for example, concatenating a
 * UTF-8 string (on the left) and a UTF-16 string (on the right) produces
 * a UTF-8 string.
 * 
 * Many `udstr' functions also have a varient whose name contains the
 * suffix `_fw' ("full width").  These choose output encoding forms by
 * choosing the _narrowest_ encoding wide enough so that each
 * codepoint in the string occupies exactly one coding value.  For example,
 * the concatenation of a full-width UTF-8 string (which must contain
 * only codepoints in the range 0..127) with a full-width UTF-16 string 
 * (which can contain no surrogate pair codepoints are codepoints larger
 * than 16 bits) may be either UTF-8 or UTF-16, depending on whether the
 * UTF-16 argument contains any codepoints outside of the range 0..127.
 * 
 * [Add table of precise conversion rules.]
 */



/************************************************************************
 *(h2 "Constructors")
 *
 */


/*(c udstr_save)
 * t_udstr udstr_save (alloc_limits limits,
 *                     uni_string str,
 *                     enum uni_encoding_scheme enc);
 * 
 * Create a `t_udstr' from a 0-terminated unicode string
 * in the indicated encoding form.
 * 
 * The internal encoding form of the resulting `t_udstr'
 * is the same as the encoding of `str', namely, `enc'.
 */
t_udstr
udstr_save (alloc_limits limits,
            uni_string str,
            enum uni_encoding_scheme enc)
{
  ustr_cv_index_t len;
  ustr_cp_index_t cp_len;

  len = ustr_lengths (&cp_len, 0, str, enc);
  return udstr_save_generic (limits, enc, len, str, enc, len, cp_len);
}


/*(c udstr_save_n)
 * t_udstr udstr_save_n (alloc_limits limits,
 *                       uni_string str,
 *                       enum uni_encoding_scheme enc,
 *                       ustr_cv_index_t len);
 * 
 * 
 * Create a `t_udstr' from a unicode string
 * in the indicated encoding form and of the
 * indicated length (in code values).
 * 
 * The internal encoding form of the resulting `t_udstr'
 * is the same as the encoding of `str', namely, `enc'.
 */
t_udstr
udstr_save_n (alloc_limits limits,
              uni_string str,
              enum uni_encoding_scheme enc,
              ustr_cv_index_t len)
{
  ustr_cp_index_t cp_len;

  cp_len = ustr_cp_length_n (0, str, enc, len);
  return udstr_save_generic (limits, enc, len, str, enc, len, cp_len);
}



/*(c udstr_save_fw)
 * t_udstr udstr_save_fw (alloc_limits limits,
 *                        uni_string str,
 *                        enum uni_encoding_scheme enc);
 * 
 * 
 * Create a `t_udstr' from a 0-terminated unicode string
 * in the indicated encoding form.
 * 
 * The internal encoding form of the resulting `t_udstr'
 * is the narrowest among:
 * 
 *	uni_iso8859_1
 * 	uni_utf16
 * 	uni_utf32
 * 	uni_bogus32
 * 
 * in which the data from `str' can be represented with
 * exactly one code value per coding point.
 */
t_udstr
udstr_save_fw (alloc_limits limits,
               uni_string str,
               enum uni_encoding_scheme enc)
{
  ustr_cv_index_t len;
  ustr_cp_index_t cp_len;
  ustr_cv_index_t dest_len;
  enum uni_encoding_scheme fw_enc;

  len = ustr_lengths (&cp_len, &fw_enc, str, enc);
  dest_len.cv = cp_len.cp;
  return udstr_save_generic (limits, fw_enc, dest_len, str, enc, len, cp_len);
}


/*(c udstr_save_fw_n)
 * t_udstr udstr_save_fw_n (alloc_limits limits,
 *                          uni_string str,
 *                          enum uni_encoding_scheme enc,
 *                          ustr_cv_index_t len);
 * 
 * Create a `t_udstr' from a unicode string
 * in the indicated encoding form and of the
 * indicated length (in code values).
 * 
 * The internal encoding form of the resulting `t_udstr'
 * is the narrowest among:
 * 
 *	uni_iso8859_1
 * 	uni_utf16
 * 	uni_utf32
 * 	uni_bogus32
 * 
 * in which the data from `str' can be represented with
 * exactly one code value per coding point.
 */
t_udstr
udstr_save_fw_n (alloc_limits limits,
                 uni_string str,
                 enum uni_encoding_scheme enc,
                 ustr_cv_index_t len)
{
  ustr_cp_index_t cp_len;
  ustr_cv_index_t dest_len;
  enum uni_encoding_scheme fw_enc;

  (void)ustr_lengths_n (&cp_len, &fw_enc, str, enc, len);
  dest_len.cv = cp_len.cp;
  return udstr_save_generic (limits, fw_enc, dest_len, str, enc, len, cp_len);
}




t_udstr
udstr_save_generic (alloc_limits limits,
                    enum uni_encoding_scheme dest_enc,
                    ustr_cv_index_t dest_len,
                    uni_string str,
                    enum uni_encoding_scheme enc,
                    ustr_cv_index_t len,
                    ustr_cp_index_t cp_len)
{
  t_udstr answer;
  size_t dest_cv_sizeof;

  answer = (t_udstr)lim_malloc (limits, sizeof (*answer));
  if (!answer)
    return 0;

  answer->refs = 1;
  answer->enc = dest_enc;
  answer->cv_len = dest_len;
  answer->cp_len = cp_len;

  dest_cv_sizeof = uni_cv_sizeof (dest_enc);

  answer->str.raw = lim_malloc (limits, (1 + dest_len.cv) * dest_cv_sizeof);
  if (!answer->str.raw)
    {
      lim_free (limits, (t_uchar *)answer);
      answer = 0;
    }
  else
    {
      if (dest_enc == enc)
        {
          mem_move (answer->str.raw, str.raw, len.cv * dest_cv_sizeof);
          mem_set0 (answer->str.raw + (dest_len.cv * dest_cv_sizeof), dest_cv_sizeof);
        }
      else
        {
          ustr_copy_n (answer->str, dest_enc, dest_len, str, enc, len);
        }
    }

  return answer;
}



/************************************************************************
 *(h2 "udstr Memory Management")
 * 
 * `t_udstr' values are reference counted objects.
 * Constructors return objects with a reference count
 * of 1.
 * 
 */

/*(c udstr_ref)
 * void udstr_ref (t_udstr d);
 * 
 * Increment the reference count of `d'.
 */
void
udstr_ref (t_udstr d)
{
  if (!d)
    return;

  ++d->refs;
}


/*(c udstr_unref)
 * void udstr_unref (t_udstr d);
 * 
 * Decrement the reference count of `d'.
 * If it drops to 0, free all storage associated
 * wtih `d' (thus invalidating all references 
 * to `d').
 */
void
udstr_unref (t_udstr d)
{
  if (!d)
    return;

  if (d->refs > 1)
    --d->refs;
  else
    {
      lim_free (d->limits, d->str.raw);
    }
}



/************************************************************************
 *(h2 "udstr Deconstruction")
 * 
 * 
 * 
 */

/*(c udstr_cv_length)
 * ustr_cv_index_t udstr_cv_length (t_udstr dstr);
 * 
 * Return the length of `dstr', measured in coding values.
 */
ustr_cv_index_t
udstr_cv_length (t_udstr dstr)
{
  return dstr->cv_len;
}


/*(c udstr_cp_length)
 * ustr_cp_index_t udstr_cp_length (t_udstr dstr);
 * 
 * Return the length of `dstr', measured in codepoints.
 */
ustr_cp_index_t
udstr_cp_length (t_udstr dstr)
{
  return dstr->cp_len;
}


/*(c udstr_encoding)
 * enum uni_encoding_scheme udstr_encoding (t_udstr dstr);
 * 
 * Return the encoding form used internally for `dstr'.
 */
enum uni_encoding_scheme
udstr_encoding (t_udstr dstr)
{
  return dstr->enc;
}


/*(c udstr_str)
 * uni_string udstr_str (t_udstr dstr);
 * 
 * Return a pointer to the string data used internally for `dstr'.
 * Subsequent calls to `udstr' functions with `dstr' as a parameter
 * can invalidate the return value of this function.
 */
uni_string
udstr_str (t_udstr dstr)
{
  return dstr->str;
}


/*(c udstr_cv_ref)
 * t_unicode udstr_cv_ref (ustr_cv_index_t * pos_after,
 *                         t_udstr dstr,
 *                         ustr_cv_index_t pos);
 * 
 * Return the codepoint found at the indicated 
 * code value index in `dstr'.  Optionally return the
 * codevalue index of the subsequent character.
 */
t_unicode
udstr_cv_ref (ustr_cv_index_t * pos_after,
              t_udstr dstr,
              ustr_cv_index_t pos)
{
  return ustr_cv_ref_n (pos_after, dstr->str, dstr->enc, dstr->cv_len, pos);
}


/*(c udstr_cp_ref)
 * t_unicode udstr_cp_ref (ustr_cv_index_t * pos_after,
 *                         t_udstr dstr,
 *                         ustr_cp_index_t pos);
 *
 * Return the codepoint found at the indicated 
 * codepoint index in `dstr'.  Optionally return the
 * codevalue index of the subsequent character.
 */
t_unicode
udstr_cp_ref (ustr_cv_index_t * pos_after,
              t_udstr dstr,
              ustr_cp_index_t pos)
{
  uni_string pos_str;
  ustr_cv_index_t pos_cv;

  if (pos.cp >= dstr->cp_len.cp)
    {
      return 0;
    }
  else
    {
      pos_str = ustr_cp_offset_n (dstr->str, dstr->enc, dstr->cv_len, pos);
      pos_cv = ustr_str_subtract (pos_str, dstr->str, dstr->enc);
      return udstr_cv_ref (pos_after, dstr, pos_cv);
    }
}


/*(c udstr_cv_set)
 * t_udstr udstr_cv_set (alloc_limits limits,
 *                       t_udstr s,
 *                       ustr_cv_index_t x,
 *                       t_unicode c);
 * 
 * Return a new copy of `s' with the character at the 
 * indicated code value replaced by `c'.
 */
t_udstr
udstr_cv_set (alloc_limits limits,
              t_udstr s,
              ustr_cv_index_t x,
              t_unicode c)
{
  struct udstr_handle tmp;
  ustr_cv_index_t end;

  tmp.refs = 1;
  tmp.str.utf32 = &c;
  tmp.enc = uni_bogus32;
  tmp.cv_len = ustr_cv_index (1);
  tmp.cp_len = ustr_cp_index (1);
  tmp.limits = 0;

  end = udstr_cv_inc (s, x);

  return udstr_cv_replace (limits,
                           s, x, end,
                           &tmp, ustr_cv_index (0), ustr_cv_index (1));
}


/*(c udstr_cv_set_fw)
 * t_udstr udstr_cv_set_fw (alloc_limits limits,
 *                          t_udstr s,
 *                          ustr_cv_index_t x,
 *                          t_unicode c);
 * 
 * 
 * Return a new copy of `s' with the character at the 
 * indicated code value replaced by `c'.
 * 
 * The returned string uses a full-width encoding.
 */
t_udstr
udstr_cv_set_fw (alloc_limits limits,
                 t_udstr s,
                 ustr_cv_index_t x,
                 t_unicode c)
{
  struct udstr_handle tmp;
  ustr_cv_index_t end;

  tmp.refs = 1;
  tmp.str.utf32 = &c;
  tmp.enc = uni_bogus32;
  tmp.cv_len = ustr_cv_index (1);
  tmp.cp_len = ustr_cp_index (1);
  tmp.limits = 0;

  end = udstr_cv_inc (s, x);

  return udstr_cv_replace_fw (limits,
                              s, x, end,
                              &tmp, ustr_cv_index (0), ustr_cv_index (1));
}


/*(c udstr_cp_set)
 * t_udstr udstr_cp_set (alloc_limits limits,
 *                       t_udstr s,
 *                       ustr_cp_index_t x,
 *                       t_unicode c);
 * 
 * 
 * Return a new copy of `s' with the character at the 
 * indicated code value replaced by `c'.
 */
t_udstr
udstr_cp_set (alloc_limits limits,
              t_udstr s,
              ustr_cp_index_t x,
              t_unicode c)
{
  struct udstr_handle tmp;

  tmp.refs = 1;
  tmp.str.utf32 = &c;
  tmp.enc = uni_bogus32;
  tmp.cv_len = ustr_cv_index (1);
  tmp.cp_len = ustr_cp_index (1);
  tmp.limits = 0;

  return udstr_cp_replace (limits,
                           s, x, ustr_cp_index (x.cp + 1),
                           &tmp, ustr_cp_index (0), ustr_cp_index (1));
}


/*(c udstr_cp_set_fw)
 * t_udstr udstr_cp_set_fw (alloc_limits limits,
 *                          t_udstr s,
 *                          ustr_cp_index_t x,
 *                          t_unicode c);
 * 
 * 
 * Return a new copy of `s' with the character at the 
 * indicated code value replaced by `c'.
 * 
 * The returned string uses a full-width encoding.
 */
t_udstr
udstr_cp_set_fw (alloc_limits limits,
                 t_udstr s,
                 ustr_cp_index_t x,
                 t_unicode c)
{
  struct udstr_handle tmp;

  tmp.refs = 1;
  tmp.str.utf32 = &c;
  tmp.enc = uni_bogus32;
  tmp.cv_len = ustr_cv_index (1);
  tmp.cp_len = ustr_cp_index (1);
  tmp.limits = 0;

  return udstr_cp_replace_fw (limits,
                              s, x, ustr_cp_index (x.cp + 1),
                              &tmp, ustr_cp_index (0), ustr_cp_index (1));
}





/************************************************************************
 *(h2 "Full-width Conversion")
 * 
 */


/*(c udstr_fw_x)
 * t_udstr udstr_fw_x (t_udstr d);
 * 
 * Modify (if necessary) `d' to be of a narrowest encoding such that
 * each codepoint in `d' occupies exactly one code value.
 */
t_udstr
udstr_fw_x (t_udstr d)
{
  enum uni_encoding_scheme fw_enc;

  (void)ustr_cp_length_n (&fw_enc, d->str, d->enc, d->cv_len);

  if (d->enc == fw_enc)
    return d;

  {
    ustr_cv_index_t fw_len;
    t_udstr new_data = 0;

    fw_len = ustr_cv_length_in_encoding_n (fw_enc, d->str, d->enc, d->cv_len);

    new_data = udstr_save_generic (d->limits, fw_enc, fw_len, d->str, d->enc, d->cv_len, d->cp_len);
    if (!new_data)
      return 0;

    take_new_data (d, new_data);

    return d;
  }
}



/************************************************************************
 *(h2 "udstr String Copying")
 * 
 * 
 * 
 */

/*(c udstr_copy)
 * t_udstr udstr_copy (alloc_limits limits, t_udstr dstr);
 * 
 * Allocate a fresh copy of `dstr'.
 */
t_udstr
udstr_copy (alloc_limits limits, t_udstr dstr)
{
  return udstr_save_generic (limits, dstr->enc, dstr->cv_len, dstr->str, dstr->enc, dstr->cv_len, dstr->cp_len);
}


/*(c udstr_copy_fw)
 * t_udstr udstr_copy_fw (alloc_limits limits, t_udstr dstr);
 * 
 * Allocate a fresh copy of `dstr', converting it (if necessary)
 * to a full-width encoding.
 */
t_udstr
udstr_copy_fw (alloc_limits limits, t_udstr dstr)
{
  return udstr_save_fw_n (limits, dstr->str, dstr->enc, dstr->cv_len);
}


/************************************************************************
 *(h2 "udstr Substrings")
 * 
 * 
 * 
 */


/*(c udstr_cv_substr)
 * t_udstr udstr_cv_substr (alloc_limits limits,
 *                          t_udstr dstr,
 *                          ustr_cv_index_t from,
 *                          ustr_cv_index_t to);
 * 
 * Return a freshly allocated substring of `dstr'
 * containing the indicated half-open range of characters
 * (measured in code values).
 */
t_udstr
udstr_cv_substr (alloc_limits limits,
                 t_udstr dstr,
                 ustr_cv_index_t from,
                 ustr_cv_index_t to)
{
  uni_string str;
  ustr_cv_index_t len;

  str = ustr_cv_offset (dstr->str, dstr->enc, from);
  len = ustr_cv_index (to.cv - from.cv);

  return udstr_save_n (limits, str, dstr->enc, len);
}


/*(c udstr_cv_substr_x)
 * t_udstr udstr_cv_substr_x (t_udstr dstr,
 *                            ustr_cv_index_t from,
 *                            ustr_cv_index_t to);
 * 
 * Modify `dstr' to contain only its indicated
 * substring.
 */
t_udstr
udstr_cv_substr_x (t_udstr dstr,
                   ustr_cv_index_t from,
                   ustr_cv_index_t to)
{
  t_udstr almost_answer;

  almost_answer = udstr_cv_substr (dstr->limits, dstr, from, to);

  if (!almost_answer)
    {
      return 0;
    }
  else
    {
      take_new_data (dstr, almost_answer);

      return dstr;
    }
}


/*(c udstr_cv_substr_fw)
 * t_udstr udstr_cv_substr_fw (alloc_limits limits,
 *                             t_udstr dstr,
 *                             ustr_cv_index_t from,
 *                             ustr_cv_index_t to);
 * 
 * 
 * Return a freshly allocated substring of `dstr'
 * containing the indicated half-open range of characters
 * (measured in code values).
 * 
 * The returned string uses a full-width encoding (all of
 * it's codepoints fit in exactly one code value).
 */
t_udstr
udstr_cv_substr_fw (alloc_limits limits,
                    t_udstr dstr,
                    ustr_cv_index_t from,
                    ustr_cv_index_t to)
{
  uni_string str;
  ustr_cv_index_t len;

  str = ustr_cv_offset (dstr->str, dstr->enc, from);
  len = ustr_cv_index (to.cv - from.cv);

  return udstr_save_fw_n (limits, str, dstr->enc, len);
}


/*(c udstr_cv_substr_fw_x)
 * t_udstr udstr_cv_substr_fw_x (t_udstr dstr,
 *                               ustr_cv_index_t from,
 *                               ustr_cv_index_t to);
 * 
 * Modify `dstr' to contain only its indicated substring
 * and to be in a full-width encoding.
 */
t_udstr
udstr_cv_substr_fw_x (t_udstr dstr,
                      ustr_cv_index_t from,
                      ustr_cv_index_t to)
{
  t_udstr almost_answer;

  almost_answer = udstr_cv_substr_fw (dstr->limits, dstr, from, to);

  if (!almost_answer)
    {
      return 0;
    }
  else
    {
      take_new_data (dstr, almost_answer);

      return dstr;
    }
}


/*(c udstr_cp_substr)
 * t_udstr udstr_cp_substr (alloc_limits limits,
 *                          t_udstr dstr,
 *                          ustr_cp_index_t from,
 *                          ustr_cp_index_t to);
 * 
 * Return a freshly allocated substring of `dstr'
 * containing the indicated half-open range of characters
 * (measured in codepoints).
 */
t_udstr
udstr_cp_substr (alloc_limits limits,
                 t_udstr dstr,
                 ustr_cp_index_t from,
                 ustr_cp_index_t to)
{
  ustr_cv_index_t f_v;
  ustr_cv_index_t t_v;

  udstr_cp_to_cv_range (&f_v, &t_v, dstr, from, to);
  return udstr_cv_substr (limits, dstr, f_v, t_v);
}


/*(c udstr_cp_substr_x)
 * t_udstr udstr_cp_substr_x (t_udstr dstr,
 *                            ustr_cp_index_t from,
 *                            ustr_cp_index_t to);
 * 
 * Modify `dstr' to contain only its indicated substring.
 */
t_udstr
udstr_cp_substr_x (t_udstr dstr,
                   ustr_cp_index_t from,
                   ustr_cp_index_t to)
{
  t_udstr almost_answer;

  almost_answer = udstr_cp_substr (dstr->limits, dstr, from, to);

  if (!almost_answer)
    {
      return 0;
    }
  else
    {
      take_new_data (dstr, almost_answer);

      return dstr;
    }
}


/*(c udstr_cp_substr_fw)
 * t_udstr udstr_cp_substr_fw (alloc_limits limits,
 *                             t_udstr dstr,
 *                             ustr_cp_index_t from,
 *                             ustr_cp_index_t to);
 * 
 * Return a freshly allocated substring of `dstr'
 * containing the indicated half-open range of characters
 * (measured in codepoints).
 * 
 * The returned string uses a full-width encoding (all of
 * it's codepoints fit in exactly one code value).
 */
t_udstr
udstr_cp_substr_fw (alloc_limits limits,
                    t_udstr dstr,
                    ustr_cp_index_t from,
                    ustr_cp_index_t to)
{
  ustr_cv_index_t f_v;
  ustr_cv_index_t t_v;

  udstr_cp_to_cv_range (&f_v, &t_v, dstr, from, to);
  return udstr_cv_substr_fw (limits, dstr, f_v, t_v);
}


/*(c udstr_cp_substr_fw_x)
 * t_udstr udstr_cp_substr_fw_x (t_udstr dstr,
 *                               ustr_cp_index_t from,
 *                               ustr_cp_index_t to);
 * 
 * Modify `dstr' to contain only its indicated substring
 * and to be in a full-width encoding.
 */
t_udstr
udstr_cp_substr_fw_x (t_udstr dstr,
                      ustr_cp_index_t from,
                      ustr_cp_index_t to)
{
  t_udstr almost_answer;

  almost_answer = udstr_cp_substr_fw (dstr->limits, dstr, from, to);

  if (!almost_answer)
    {
      return 0;
    }
  else
    {
      take_new_data (dstr, almost_answer);

      return dstr;
    }
}





/************************************************************************
 *(h2 "udstr String Concatentation")
 * 
 * 
 * 
 */



/*(c udstr_append)
 * t_udstr udstr_append (alloc_limits limits,
 *                       t_udstr a_dstr,
 *                       t_udstr b_dstr);
 * 
 * Return a freshly allocated string containing
 * the concatenation of the argument strings.
 */
t_udstr
udstr_append (alloc_limits limits,
              t_udstr a_dstr,
              t_udstr b_dstr)
{
  t_udstr answer = 0;

  answer = udstr_copy (limits, a_dstr);

  if (answer)
    {
      if (!udstr_append_x (answer, b_dstr))
        {
          udstr_unref (answer);
          answer = 0;
        }
    }

  return answer;
}


/*(c udstr_append_x)
 * t_udstr udstr_append_x (t_udstr a_dstr,
 *                         t_udstr b_dstr);
 * 
 * Modify `a_dstr' to contain the concatenation
 * of `a_dstr' and `b_dstr'.
 */
t_udstr
udstr_append_x (t_udstr a_dstr,
                t_udstr b_dstr)
{
  ustr_cv_index_t b_len_in_a_enc;
  ustr_cv_index_t total_len;
  size_t a_enc_size;
  size_t proper_a_size;
  uni_string b_dest;

  if (a_dstr->enc == b_dstr->enc)
    {
      b_len_in_a_enc = b_dstr->cv_len;
    }
  else
    {
      b_len_in_a_enc = ustr_cv_length_in_encoding_n (a_dstr->enc, b_dstr->str, b_dstr->enc, b_dstr->cv_len);
    }

  total_len = ustr_cv_index (b_len_in_a_enc.cv + a_dstr->cv_len.cv);

  a_enc_size = uni_cv_sizeof (a_dstr->enc);
  proper_a_size = a_dstr->cv_len.cv * a_enc_size;

  {
    t_uchar * resized;

    resized = lim_realloc (a_dstr->limits, a_dstr->str.raw, proper_a_size);
    if (!resized)
      return 0;

    a_dstr->str.raw = resized;
  }

  b_dest = ustr_cv_offset_n (0, a_dstr->str, a_dstr->enc, a_dstr->cv_len, a_dstr->cv_len);

  ustr_copy_n (b_dest, a_dstr->enc, b_len_in_a_enc, b_dstr->str, b_dstr->enc, b_dstr->cv_len);

  a_dstr->cp_len = ustr_cp_length_n (0, a_dstr->str, a_dstr->enc, a_dstr->cv_len);

  return a_dstr;
}


/*(c udstr_append_fw)
 * t_udstr udstr_append_fw (alloc_limits limits,
 *                          t_udstr a_dstr,
 *                          t_udstr b_dstr);
 * 
 * Return a freshly allocated string containing
 * the concatenation of the argument strings.
 * 
 * The new string uses a full-width encoding.
 */
t_udstr
udstr_append_fw (alloc_limits limits,
                 t_udstr a_dstr,
                 t_udstr b_dstr)
{
  t_udstr answer = 0;

  answer = udstr_copy (limits, a_dstr);

  if (answer)
    {
      if (!udstr_append_fw_x (answer, b_dstr))
        {
          udstr_unref (answer);
          answer = 0;
        }
    }

  return answer;
}


/*(c udstr_append_fw_x)
 * t_udstr udstr_append_fw_x (t_udstr a_dstr,
 *                            t_udstr b_dstr);
 * 
 * Modify `a_dstr' to contain the concatenation
 * of `a_dstr' and `b_dstr' and to use a full-width
 * encoding.
 */
t_udstr
udstr_append_fw_x (t_udstr a_dstr,
                   t_udstr b_dstr)
{
  enum uni_encoding_scheme a_fw;
  enum uni_encoding_scheme b_fw;
  enum uni_encoding_scheme best_encoding;

  ustr_lengths_n (0, &a_fw, a_dstr->str, a_dstr->enc, a_dstr->cv_len);
  ustr_lengths_n (0, &b_fw, a_dstr->str, a_dstr->enc, a_dstr->cv_len);

  best_encoding = pick_fw_of (a_fw, b_fw);

  if (a_dstr->enc != best_encoding)
    {
      t_udstr new_a;

      new_a = udstr_save_generic (a_dstr->limits,
                                  best_encoding,
                                  ustr_cv_index (a_dstr->cp_len.cp),
                                  a_dstr->str,
                                  a_dstr->enc,
                                  a_dstr->cv_len,
                                  a_dstr->cp_len);

      if (!new_a)
        return 0;

      a_dstr->str.raw = new_a->str.raw;
      a_dstr->enc = new_a->enc;
      
      lim_free (new_a->limits, new_a);
      
      /* new_a dropped deliberately --- a_dstr took it over
       */
    }

  return udstr_append_x (a_dstr, b_dstr);
}





/************************************************************************
 *(h2 "udstr Substring Deletion")
 * 
 * 
 * 
 */


/*(c udstr_cv_delete)
 * t_udstr udstr_cv_delete (alloc_limits limits,
 *                          t_udstr d,
 *                          ustr_cv_index_t from,
 *                          ustr_cv_index_t to);
 * 
 * Return a new string which is a copy of `d' with
 * code values in the half-open range `from' to `to'
 * removed.
 */
t_udstr
udstr_cv_delete (alloc_limits limits,
                 t_udstr d,
                 ustr_cv_index_t from,
                 ustr_cv_index_t to)
{
  uni_string right_source;
  ustr_cv_index_t right_len;
  ustr_cv_index_t total_len;
  ustr_cv_index_t left_len;
  uni_string right_dest;
  t_udstr answer = 0;


  right_source = ustr_cv_offset_n (&right_len, d->str, d->enc, d->cv_len, to);
  left_len = from;
  total_len = ustr_cv_index (left_len.cv + right_len.cv);


  answer = udstr_save_generic (limits, d->enc, total_len, d->str, d->enc, d->cv_len, d->cp_len);
  if (!answer)
    return 0;
  /*
   * answer is missing half of its data and has the wrong cp_len now.
   */

  right_dest = ustr_cv_offset_n (0, answer->str, answer->enc, answer->cv_len, left_len);
  ustr_copy_n (right_dest, answer->enc, right_len, right_source, d->enc, right_len);

  answer->cp_len = ustr_cp_length_n (0, answer->str, answer->enc, answer->cv_len);

  return answer;
}


/*(c udstr_cp_delete)
 * t_udstr udstr_cp_delete (alloc_limits limits,
 *                          t_udstr d,
 *                          ustr_cp_index_t from,
 *                          ustr_cp_index_t to);
 * 
 * Return a new string which is a copy of `d' with
 * codepoints in the half-open range `from' to `to'
 * removed.
 */
t_udstr
udstr_cp_delete (alloc_limits limits,
                 t_udstr d,
                 ustr_cp_index_t from,
                 ustr_cp_index_t to)
{
  ustr_cv_index_t fv;
  ustr_cv_index_t tv;

  udstr_cp_to_cv_range (&fv, &tv, d, from, to);
  return udstr_cv_delete (limits, d, fv, tv);
}


/*(c udstr_cv_delete_x)
 * t_udstr udstr_cv_delete_x (t_udstr d,
 *                            ustr_cv_index_t from,
 *                            ustr_cv_index_t to);
 * 
 * Modify `d' by removing code values in the half-open range `from' to
 * `to'.
 */
t_udstr
udstr_cv_delete_x (t_udstr d,
                   ustr_cv_index_t from,
                   ustr_cv_index_t to)
{
  size_t cv_sizeof;
  size_t from_offset;
  size_t to_offset;
  size_t current_length;

  cv_sizeof = uni_cv_sizeof (d->enc);

  from_offset = from.cv * cv_sizeof;
  to_offset = to.cv * cv_sizeof;
  current_length = d->cv_len.cv * cv_sizeof;

  mem_move (d->str.raw + from_offset, d->str.raw + to_offset, current_length - to_offset);

  d->str.raw = lim_realloc (d->limits, d->str.raw, current_length - (to_offset - from_offset));
  d->cv_len.cv = (ssize_t)(current_length - (to_offset - from_offset));

  return d;
}


/*(c udstr_cp_delete_x)
 * t_udstr udstr_cp_delete_x (t_udstr d,
 *                            ustr_cp_index_t from,
 *                            ustr_cp_index_t to);
 * 
 * Modify `d' by removing codepoints in the half-open range `from' to
 * `to'.
 */
t_udstr
udstr_cp_delete_x (t_udstr d,
                   ustr_cp_index_t from,
                   ustr_cp_index_t to)
{
  ustr_cv_index_t fv;
  ustr_cv_index_t tv;

  udstr_cp_to_cv_range (&fv, &tv, d, from, to);
  return udstr_cv_delete_x (d, fv, tv);
}


/*(c udstr_cv_delete_fw)
 * t_udstr udstr_cv_delete_fw (alloc_limits limits,
 *                             t_udstr d,
 *                             ustr_cv_index_t from,
 *                             ustr_cv_index_t to);
 * 
 * Return a new string which is a copy of `d' with
 * code values in the half-open range `from' to `to'
 * removed.
 * 
 * The new string uses a full-width encoding.
 */
t_udstr
udstr_cv_delete_fw (alloc_limits limits,
                    t_udstr d,
                    ustr_cv_index_t from,
                    ustr_cv_index_t to)
{
  uni_string right_source;
  ustr_cv_index_t right_len;
  ustr_cp_index_t right_cp_len;
  enum uni_encoding_scheme right_fw;
  ustr_cp_index_t left_cp_len;
  enum uni_encoding_scheme left_fw;
  enum uni_encoding_scheme actual_fw;
  ustr_cv_index_t right_len_fw;
  ustr_cv_index_t left_len_fw;
  ustr_cv_index_t len_fw;
  ustr_cp_index_t cp_len;
  t_udstr answer = 0;
  uni_string right_dest;
  size_t d_cv_sizeof;
  size_t d_right_sizeof;


  right_source = ustr_cv_offset_n (&right_len, d->str, d->enc, d->cv_len, to);
  right_cp_len = ustr_cp_length_n (&right_fw, right_source, d->enc, right_len);

  left_cp_len = ustr_cp_length_n (&left_fw, d->str, d->enc, from);

  actual_fw = pick_fw_of (right_fw, left_fw);

  if (actual_fw == d->enc)
    {
      right_len_fw = right_len;
      left_len_fw = from;
    }
  else
    {
      right_len_fw = ustr_cv_length_in_encoding_n (actual_fw, right_source, d->enc, right_len);
      left_len_fw = ustr_cv_length_in_encoding_n (actual_fw, d->str, d->enc, from);
    }

  len_fw = ustr_cv_index (right_len_fw.cv + left_len_fw.cv);
  cp_len = ustr_cp_index (right_cp_len.cp + left_cp_len.cp);

  answer = udstr_save_generic (limits, actual_fw, len_fw, d->str, d->enc, from, left_cp_len);
  if (!answer)
    return 0;
  
  answer->cp_len = cp_len;

  right_dest = ustr_cv_offset_n (0, answer->str, answer->enc, answer->cv_len, left_len_fw);
  d_cv_sizeof = uni_cv_sizeof (d->enc);
  d_right_sizeof = right_len.cv * d_cv_sizeof;
  mem_move (right_dest.raw, right_source.raw, d_right_sizeof);

  return answer;
}


/*(c udstr_cp_delete_fw)
 * t_udstr udstr_cp_delete_fw (alloc_limits limits,
 *                             t_udstr d,
 *                             ustr_cp_index_t from,
 *                             ustr_cp_index_t to);
 * 
 * Return a new string which is a copy of `d' with
 * codepoints in the half-open range `from' to `to'
 * removed.
 * 
 * The new string uses a full-width encoding.
 */
t_udstr
udstr_cp_delete_fw (alloc_limits limits,
                    t_udstr d,
                    ustr_cp_index_t from,
                    ustr_cp_index_t to)
{
  ustr_cv_index_t fv;
  ustr_cv_index_t tv;

  udstr_cp_to_cv_range (&fv, &tv, d, from, to);
  return udstr_cv_delete_fw (limits, d, fv, tv);
}


/*(c udstr_cv_delete_fw_x)
 * t_udstr udstr_cv_delete_fw_x (t_udstr d,
 *                               ustr_cv_index_t from,
 *                               ustr_cv_index_t to);
 * 
 * Modify `d' by removing code values in the half-open range `from' to
 * `to' and ensuring that `d' uses a full-width encoding.
 */
t_udstr
udstr_cv_delete_fw_x (t_udstr d,
                      ustr_cv_index_t from,
                      ustr_cv_index_t to)
{
  t_udstr almost_answer = 0;

  almost_answer = udstr_cv_delete_fw (d->limits, d, from, to);

  if (!almost_answer)
    return 0;

  take_new_data (d, almost_answer);

  return d;
}


/*(c udstr_cp_delete_fw_x)
 * t_udstr udstr_cp_delete_fw_x (t_udstr d,
 *                               ustr_cp_index_t from,
 *                               ustr_cp_index_t to);
 * 
 * Modify `d' by removing codepoints in the half-open range `from' to
 * `to' and ensuring that `d' uses a full-width encoding.
 */
t_udstr
udstr_cp_delete_fw_x (t_udstr d,
                      ustr_cp_index_t from,
                      ustr_cp_index_t to)
{
  ustr_cv_index_t fv;
  ustr_cv_index_t tv;

  udstr_cp_to_cv_range (&fv, &tv, d, from, to);
  return udstr_cv_delete_fw_x (d, fv, tv);
}





/************************************************************************
 *(h2 "udstr Substring Replacement")
 * 
 * 
 * 
 */


/*(c udstr_cv_replace)
 * t_udstr udstr_cv_replace (alloc_limits limits,
 *                           t_udstr into,
 *                           ustr_cv_index_t i_from,
 *                           ustr_cv_index_t i_to,
 *                           t_udstr from,
 *                           ustr_cv_index_t f_from,
 *                           ustr_cv_index_t f_to);
 * 
 * Return a new string in which the indicated substring
 * of `into' is replaced by the indicated substring of `from'.
 */
t_udstr
udstr_cv_replace (alloc_limits limits,
                  t_udstr into,
                  ustr_cv_index_t i_from,
                  ustr_cv_index_t i_to,
                  t_udstr from,
                  ustr_cv_index_t f_from,
                  ustr_cv_index_t f_to)
{
  t_udstr answer = 0;
  t_udstr middle = 0;
  t_udstr end = 0;

  answer = udstr_cv_substr (limits, into, ustr_cv_index (0), i_from);
  middle = udstr_cv_substr (limits, from, f_from, f_to);
  end = udstr_cv_substr (limits, into, i_to, into->cv_len);

  if (!(answer && middle && end)
      || !udstr_append_x (answer, middle)
      || !udstr_append_x (answer, end))
    {
      udstr_unref (answer);
      udstr_unref (middle);
      udstr_unref (end);
      answer = 0;
    }
  else
    {
      udstr_unref (middle);
      udstr_unref (end);
    }

  return answer;
}


/*(c udstr_cp_replace)
 * t_udstr udstr_cp_replace (alloc_limits limits,
 *                           t_udstr into,
 *                           ustr_cp_index_t i_from,
 *                           ustr_cp_index_t i_to,
 *                           t_udstr from,
 *                           ustr_cp_index_t f_from,
 *                           ustr_cp_index_t f_to);
 * 
 * Return a new string in which the indicated substring
 * of `into' is replaced by the indicated substring of `from'.
 */
t_udstr
udstr_cp_replace (alloc_limits limits,
                  t_udstr into,
                  ustr_cp_index_t i_from,
                  ustr_cp_index_t i_to,
                  t_udstr from,
                  ustr_cp_index_t f_from,
                  ustr_cp_index_t f_to)
{
  ustr_cv_index_t i_f;
  ustr_cv_index_t i_t;
  ustr_cv_index_t f_f;
  ustr_cv_index_t f_t;

  udstr_cp_to_cv_range (&i_f, &i_t, into, i_from, i_to);
  udstr_cp_to_cv_range (&f_f, &f_t, from, f_from, f_to);

  return udstr_cv_replace (limits, into, i_f, i_t, from, f_f, f_t);
}




/*(c udstr_cv_replace_x)
 * t_udstr udstr_cv_replace_x (t_udstr into,
 *                             ustr_cv_index_t i_from,
 *                             ustr_cv_index_t i_to,
 *                             t_udstr from,
 *                             ustr_cv_index_t f_from,
 *                             ustr_cv_index_t f_to);
 * 
 * Modify `into' by replacing the indicated substring with
 * the indicated substring of `from'.
 */
t_udstr
udstr_cv_replace_x (t_udstr into,
                    ustr_cv_index_t i_from,
                    ustr_cv_index_t i_to,
                    t_udstr from,
                    ustr_cv_index_t f_from,
                    ustr_cv_index_t f_to)
{
  t_udstr almost_answer;

  almost_answer = udstr_cv_replace (into->limits, into, i_from, i_to, from, f_from, f_to);

  if (!almost_answer)
    return 0;

  take_new_data (into, almost_answer);
  return into;
}


/*(c udstr_cp_replace_x)
 * t_udstr udstr_cp_replace_x (t_udstr into,
 *                             ustr_cp_index_t i_from,
 *                             ustr_cp_index_t i_to,
 *                             t_udstr from,
 *                             ustr_cp_index_t f_from,
 *                             ustr_cp_index_t f_to);
 * 
 * Modify `into' by replacing the indicated substring with
 * the indicated substring of `from'.
 */
t_udstr
udstr_cp_replace_x (t_udstr into,
                    ustr_cp_index_t i_from,
                    ustr_cp_index_t i_to,
                    t_udstr from,
                    ustr_cp_index_t f_from,
                    ustr_cp_index_t f_to)
{
  ustr_cv_index_t i_f;
  ustr_cv_index_t i_t;
  ustr_cv_index_t f_f;
  ustr_cv_index_t f_t;

  udstr_cp_to_cv_range (&i_f, &i_t, into, i_from, i_to);
  udstr_cp_to_cv_range (&f_f, &f_t, from, f_from, f_to);

  return udstr_cv_replace_x (into, i_f, i_t, from, f_f, f_t);
}


/*(c udstr_cv_replace_fw)
 * t_udstr udstr_cv_replace_fw (alloc_limits limits,
 *                              t_udstr into,
 *                              ustr_cv_index_t i_from,
 *                              ustr_cv_index_t i_to,
 *                              t_udstr from,
 *                              ustr_cv_index_t f_from,
 *                              ustr_cv_index_t f_to);
 * 
 * Return a new string in which the indicated substring
 * of `into' is replaced by the indicated substring of `from'.
 * 
 * The returned string uses a full-width encoding.
 */
t_udstr
udstr_cv_replace_fw (alloc_limits limits,
                     t_udstr into,
                     ustr_cv_index_t i_from,
                     ustr_cv_index_t i_to,
                     t_udstr from,
                     ustr_cv_index_t f_from,
                     ustr_cv_index_t f_to)
{
  t_udstr answer = 0;
  t_udstr middle = 0;
  t_udstr end = 0;

  answer = udstr_cv_substr_fw (limits, into, ustr_cv_index (0), i_from);
  middle = udstr_cv_substr (limits, from, f_from, f_to);
  end = udstr_cv_substr (limits, into, i_to, into->cv_len);

  if (!(answer && middle && end)
      || !udstr_append_fw_x (answer, middle)
      || !udstr_append_fw_x (answer, end))
    {
      udstr_unref (answer);
      udstr_unref (middle);
      udstr_unref (end);
      answer = 0;
    }
  else
    {
      udstr_unref (middle);
      udstr_unref (end);
    }

  return answer;
}


/*(c udstr_cp_replace_fw)
 * t_udstr udstr_cp_replace_fw (alloc_limits limits,
 *                              t_udstr into,
 *                              ustr_cp_index_t i_from,
 *                              ustr_cp_index_t i_to,
 *                              t_udstr from,
 *                              ustr_cp_index_t f_from,
 *                              ustr_cp_index_t f_to);
 * 
 * Return a new string in which the indicated substring
 * of `into' is replaced by the indicated substring of `from'.
 * 
 * The returned string uses a full-width encoding.
 */
t_udstr
udstr_cp_replace_fw (alloc_limits limits,
                     t_udstr into,
                     ustr_cp_index_t i_from,
                     ustr_cp_index_t i_to,
                     t_udstr from,
                     ustr_cp_index_t f_from,
                     ustr_cp_index_t f_to)
{
  ustr_cv_index_t i_f;
  ustr_cv_index_t i_t;
  ustr_cv_index_t f_f;
  ustr_cv_index_t f_t;

  udstr_cp_to_cv_range (&i_f, &i_t, into, i_from, i_to);
  udstr_cp_to_cv_range (&f_f, &f_t, from, f_from, f_to);

  return udstr_cv_replace_fw (limits, into, i_f, i_t, from, f_f, f_t);
}


/*(c udstr_cv_replace_fw_x)
 * t_udstr udstr_cv_replace_fw_x (t_udstr into,
 *                                ustr_cv_index_t i_from,
 *                                ustr_cv_index_t i_to,
 *                                t_udstr from,
 *                                ustr_cv_index_t f_from,
 *                                ustr_cv_index_t f_to);
 * 
 * Modify `into' by replacing the indicated substring with
 * the indicated substring of `from' and ensuring that
 * `into' uses a full-width encoding.
 */
t_udstr
udstr_cv_replace_fw_x (t_udstr into,
                       ustr_cv_index_t i_from,
                       ustr_cv_index_t i_to,
                       t_udstr from,
                       ustr_cv_index_t f_from,
                       ustr_cv_index_t f_to)
{
  t_udstr almost_answer;

  almost_answer = udstr_cv_replace_fw (into->limits, into, i_from, i_to, from, f_from, f_to);

  if (!almost_answer)
    return 0;

  take_new_data (into, almost_answer);
  return into;
}


/*(c udstr_cp_replace_fw_x)
 * t_udstr udstr_cp_replace_fw_x (t_udstr into,
 *                                ustr_cp_index_t i_from,
 *                                ustr_cp_index_t i_to,
 *                                t_udstr from,
 *                                ustr_cp_index_t f_from,
 *                                ustr_cp_index_t f_to);
 * 
 * Modify `into' by replacing the indicated substring with
 * the indicated substring of `from' and ensuring that
 * `into' uses a full-width encoding.
 */
t_udstr
udstr_cp_replace_fw_x (t_udstr into,
                       ustr_cp_index_t i_from,
                       ustr_cp_index_t i_to,
                       t_udstr from,
                       ustr_cp_index_t f_from,
                       ustr_cp_index_t f_to)
{
  ustr_cv_index_t i_f;
  ustr_cv_index_t i_t;
  ustr_cv_index_t f_f;
  ustr_cv_index_t f_t;

  udstr_cp_to_cv_range (&i_f, &i_t, into, i_from, i_to);
  udstr_cp_to_cv_range (&f_f, &f_t, from, f_from, f_to);

  return udstr_cv_replace_fw_x (into, i_f, i_t, from, f_f, f_t);
}





/************************************************************************
 *(h2 "udstr Index Normalization")
 * 
 * 
 * 
 */


/*(c udstr_cv_normalize)
 * ustr_cv_index_t udstr_cv_normalize (t_udstr dstr,
 *                                     ustr_cv_index_t orig_index);
 * 
 * Return a code value index derived from `orig_index' by adjusting
 * it to point to the first code value in its codepoint.
 */
ustr_cv_index_t
udstr_cv_normalize (t_udstr dstr,
                    ustr_cv_index_t orig_index)
{
  ustr_cv_index_t index = orig_index;


  switch (dstr->enc)
    {
    default:
    case uni_iso8859_1:
    case uni_utf32:
    case uni_utf32be:
    case uni_utf32le:
    case uni_bogus32:
    case uni_bogus32be:
    case uni_bogus32le:
      {
        if (index.cv > dstr->cv_len.cv)
          index.cv = dstr->cv_len.cv;
        else if (index.cv < 0)
          index.cv = 0;

        return index;
      }

    case uni_utf8:
      {
        if (index.cv > dstr->cv_len.cv)
          {
            index.cv = dstr->cv_len.cv;
            return index;
          }
        else if (index.cv <= 0)
          {
            index.cv = 0;
            return index;
          }
        else if (!(dstr->str.utf8[index.cv] & 0x80))
          {
            return index;
          }
        else
          {
            if (0x80 == (0xC0 & dstr->str.utf8[index.cv]))
              {
                int n_suffix_bytes = 1;

                while (1)
                  {
                    /* invarients:
                     *
                     * index.cv >= 1
                     * 
                     * str[index.cv] is some 10xxxxxx character
                     * 
                     * 1 <= n_suffix_bytes <= 4
                     */

                    /* Look at the previous character to decide
                     * what to do.
                     */
                    switch (0xc0 & dstr->str.utf8[index.cv - 1])
                      {
                      case 0x80:
                        {
                          /* preceeding is also a non-first multi-byte sequence
                           * code value.  This preserves one of three loop 
                           * invarients.
                           */

                          if (n_suffix_bytes == 4)
                            {
                              /* Then the one to the left means that there are 5 or
                               * more, which is illegal, so, our original index is
                               * just pointing at an ill-formed sequence.
                               * This preserves the second loop invarient while
                               * n_suffix_bytes counts up to detect ill-formed sequences.
                               */
                              return orig_index;
                            }
                          else if (index.cv == 1)
                            {
                              /* The string starts (at index 0) in the middle of
                               * a multi-char sequence.   So, once again, our
                               * original index is pointing at an ill-formed sequence.
                               * This preserves the final loop invarient.
                               */
                              return orig_index;
                            }
                          else
                            {
                              /* All invarients being preserved and having had made
                               * progress towards finding the start of the character
                               * sequence:
                               */
                              ++n_suffix_bytes;
                              --index.cv;
                              continue;
                            }
                        }
                      case 0x40:
                      case 0x00:
                        {
                          /* The place we started at is part of an ill-formed sequence (no 
                           * 0xc0 starting character.   This preserves our loop invarients.
                           */
                          return orig_index;
                        }

                      case 0xC0:
                        {
                          /* aha!  the first character of a multi-byte sequence.
                           */
                          --index.cv;
                          return index;
                        }
                      }
                  }
              }
          }
      }
    case uni_utf16:
      {
        if (index.cv > dstr->cv_len.cv)
          {
            index.cv = dstr->cv_len.cv;
            return index;
          }
        else if (index.cv <= 0)
          {
            index.cv = 0;
            return index;
          }
        else if (uni_is_low_surrogate (dstr->str.utf16[index.cv]))
          {
            if (uni_is_high_surrogate (dstr->str.utf16[index.cv - 1]))
              {
                --index.cv;
              }
            return index;
          }
        else
          return index;
      }
    case uni_utf16be:
      {
        size_t pos;
        t_unicode c;

        if (index.cv > dstr->cv_len.cv)
          {
            index.cv = dstr->cv_len.cv;
            return index;
          }
        else if (index.cv <= 0)
          {
            index.cv = 0;
            return index;
          }

        pos = 0;
        c = uni_utf16be_iscan ((t_uchar *)(dstr->str.utf16 + index.cv), &pos, (size_t)2);

        if (uni_is_low_surrogate (c))
          {
            pos = 0;
            c = uni_utf16be_iscan ((t_uchar *)(dstr->str.utf16 + index.cv - 1), &pos, (size_t)2);

            if (uni_is_high_surrogate (c))
              {
                --index.cv;
              }
            return index;
          }
        else
          return index;
      }
    case uni_utf16le:
      {
        size_t pos;
        t_unicode c;

        if (index.cv > dstr->cv_len.cv)
          {
            index.cv = dstr->cv_len.cv;
            return index;
          }
        else if (index.cv <= 0)
          {
            index.cv = 0;
            return index;
          }

        pos = 0;
        c = uni_utf16le_iscan ((t_uchar *)(dstr->str.utf16 + index.cv), &pos, (size_t)2);

        if (uni_is_low_surrogate (c))
          {
            pos = 0;
            c = uni_utf16le_iscan ((t_uchar *)(dstr->str.utf16 + index.cv - 1), &pos, (size_t)2);

            if (uni_is_high_surrogate (c))
              {
                --index.cv;
              }
            return index;
          }
        else
          return index;
      }
    }
}



/************************************************************************
 *(h2 "udstr String Iteration")
 * 
 */




/*(c udstr_cv_inc)
 * ustr_cv_index_t udstr_cv_inc (t_udstr dstr,
 *                               ustr_cv_index_t orig_index);
 * 
 * Increment `orig_index' (presumed to be the code value index
 * of the first code value of a possibly multi-code value codepoint)
 * to be the code value index of the beginning of the _next_ codepoint.
 * (!)
 */
ustr_cv_index_t
udstr_cv_inc (t_udstr dstr,
              ustr_cv_index_t orig_index)
{
  if (orig_index.cv >= dstr->cv_len.cv)
    return dstr->cv_len;

  if (orig_index.cv < 0)
    return ustr_cv_index (0);

  switch (dstr->enc)
    {
    default:
    case uni_utf32:
    case uni_utf32be:
    case uni_utf32le:
    case uni_bogus32:
    case uni_bogus32be:
    case uni_bogus32le:
    case uni_iso8859_1:
      {
        return ustr_cv_index (orig_index.cv + 1);
      }

#undef CASE_FOR

#define CASE_FOR(X) \
    case uni_ ## X: \
      { \
        size_t pos; \
        size_t len; \
        \
        pos = orig_index.cv; \
        len = dstr->cv_len.cv; \
        uni_ ## X ## _scan (dstr->str.X, &pos, len); \
        return ustr_cv_index ((ssize_t)pos); \
      }

   CASE_FOR(utf8);
   CASE_FOR(utf16);


#define ICASE_FOR(X) \
    case uni_ ## X: \
      { \
        size_t pos; \
        size_t len; \
        \
        pos = orig_index.cv * 2; \
        len = dstr->cv_len.cv * 2; \
        uni_ ## X ## _iscan (dstr->str.X, &pos, len); \
        return ustr_cv_index ((ssize_t)(pos / 2)); \
      }

   ICASE_FOR(utf16be);
   ICASE_FOR(utf16le);
    }
}



/*(c udstr_cv_inc)
 * ustr_cv_index_t udstr_cv_inc (t_udstr dstr,
 *                               ustr_cv_index_t orig_index);
 * 
 * Decrement `orig_index' (presumed to be the code value index
 * of the first code value of a possibly multi-code value codepoint)
 * to be the code value index of the beginning of the _previous_ codepoint.
 * (!)
 */
ustr_cv_index_t
udstr_cv_dec (t_udstr dstr,
              ustr_cv_index_t orig_index)
{
  if (orig_index.cv > dstr->cv_len.cv)
    return dstr->cv_len;

  if (orig_index.cv <= 0)
    return ustr_cv_index (0);

  return udstr_cv_normalize (dstr, ustr_cv_index (orig_index.cv - 1));
}


/************************************************************************
 *(hd "udstr Code Value and Codepoint Index Conversion")
 * 
 * 
 * 
 */


/*(c udstr_cp_to_cv)
 * ustr_cv_index_t udstr_cp_to_cv (t_udstr dstr,
 *                                 ustr_cp_index_t cpi);
 * 
 * Return the code value index of the indicated codepoint.
 */
ustr_cv_index_t
udstr_cp_to_cv (t_udstr dstr,
                ustr_cp_index_t cpi)
{
  uni_string addressed;

  if (cpi.cp < 0)
    return ustr_cv_index (0);

  if (cpi.cp >= dstr->cp_len.cp)
    return dstr->cv_len;

  addressed = ustr_cp_offset_n (dstr->str, dstr->enc, dstr->cv_len, cpi);
  return ustr_str_subtract (addressed, dstr->str, dstr->enc);
}


/*(c udstr_cp_to_cv_range)
 * void udstr_cp_to_cv_range (ustr_cv_index_t * from_v,
 *                            ustr_cv_index_t * to_v,
 *                            t_udstr d,
 *                            ustr_cp_index_t from,
 *                            ustr_cp_index_t to);
 *
 * Return the code value indices of the indicated codepoint range.
 * 
 */
void
udstr_cp_to_cv_range (ustr_cv_index_t * from_v,
                      ustr_cv_index_t * to_v,
                      t_udstr d,
                      ustr_cp_index_t from,
                      ustr_cp_index_t to)
{
  uni_string f_str;
  ustr_cv_index_t f_v;
  uni_string t_str;
  ustr_cv_index_t t_v;

  invariant (from.cp <= to.cp);

  f_str = ustr_cp_offset_n (d->str, d->enc, d->cv_len, from);
  f_v = ustr_str_subtract (f_str, d->str, d->enc);
  t_str = ustr_cp_offset_n (f_str,
                            d->enc,
                            ustr_cv_index (d->cv_len.cv - f_v.cv),
                            ustr_cp_index (to.cp - from.cp));
  t_v = ustr_str_subtract (t_str, f_str, d->enc);

  *from_v = f_v;
  *to_v = t_v;
}


/*(c udstr_cv_to_cp)
 * ustr_cp_index_t udstr_cv_to_cp (t_udstr dstr,
 *                                 ustr_cv_index_t cvi);
 * 
 * Return the codepoint index of the indicated code value.
 */
ustr_cp_index_t
udstr_cv_to_cp (t_udstr dstr,
                ustr_cv_index_t cvi)

{
  if (cvi.cv < 0)
    return ustr_cp_index (0);

  if (cvi.cv >= dstr->cv_len.cv)
    return dstr->cp_len;

  return ustr_cp_length_n (0, dstr->str, dstr->enc, cvi);
}





#if 0
cv_set
cv_set_x
cp_set
cp_set_x

#endif


static enum uni_encoding_scheme 
pick_fw_of (enum uni_encoding_scheme a,
            enum uni_encoding_scheme b)
{
  size_t a_size;
  size_t b_size;

  if (a == b)
    return a;

  a_size = uni_cv_sizeof (a);
  b_size = uni_cv_sizeof (b);

  if (a_size > b_size)
    return a;
  if (b_size > a_size)
    return b;

  switch (a)
    {
    case uni_iso8859_1: return uni_iso8859_1;

    case uni_utf8: return uni_utf8;

    case uni_utf16be:
    case uni_utf16le:
    case uni_utf16: return uni_utf16;

    case uni_utf32be:
    case uni_utf32le:
    case uni_utf32: return uni_utf32;

    default:
    case uni_bogus32be:
    case uni_bogus32le:
    case uni_bogus32: return uni_bogus32;
    }
}

static void
take_new_data (t_udstr dstr, t_udstr new_data)
{
    new_data->refs = dstr->refs;
    lim_free (dstr->limits, dstr->str.raw);
    *dstr = *new_data;
    lim_free (new_data->limits, (void *)new_data);
}



/* tag: Tom Lord Fri Jan  2 08:47:21 2004 (udstr.c)
 */
