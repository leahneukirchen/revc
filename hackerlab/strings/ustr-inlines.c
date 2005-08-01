/* ustr-inline.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/bugs/panic.h"
#include "hackerlab/mem/mem.h"
#include "hackerlab/char/str.h"
#include "hackerlab/uni/invariant.h"

#undef inline__
#ifdef __GCC__
#  define inline__ inline
#else
#  define inline__
#endif

#include "hackerlab/strings/ustr-inlines.h"


inline__ ustr_cv_index_t
ustri_cv_index (ssize_t n)
{
  ustr_cv_index_t answer;

  answer.cv = n;
  return answer;
}


inline__ ustr_cp_index_t
ustri_cp_index (ssize_t n)
{
  ustr_cp_index_t answer;

  answer.cp = n;
  return answer;
}


inline__ size_t
ustri_cv_index_to_scanner_index (enum uni_encoding_scheme enc,
                                 ustr_cv_index_t n)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
    case uni_utf8:
    case uni_utf16:
    case uni_utf32:
    case uni_bogus32:
      return (size_t)n.cv;
      
    case uni_utf16be:
    case uni_utf16le:
      return 2 * (size_t)n.cv;
      
    case uni_utf32be:
    case uni_utf32le:
    case uni_bogus32be:
    case uni_bogus32le:
      return 2 * (size_t)n.cv;
    }
}


inline__ ustr_cv_index_t
ustri_scanner_index_to_cv_index (enum uni_encoding_scheme enc,
                                 size_t n)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
    case uni_utf8:
    case uni_utf16:
    case uni_utf32:
    case uni_bogus32:
      return ustr_cv_index ((ssize_t)n);
      
    case uni_utf16be:
    case uni_utf16le:
      return ustr_cv_index ((ssize_t)(n / 2));
      
    case uni_utf32be:
    case uni_utf32le:
    case uni_bogus32be:
    case uni_bogus32le:
      return ustr_cv_index ((ssize_t)(n / 4));
    }
}



inline__ uni_string
ustri_cv_offset (uni_string str,
                 enum uni_encoding_scheme enc,
                 ustr_cv_index_t n)
{
  uni_string answer;

  switch (enc)
    {
    default:
    case uni_iso8859_1:
    case uni_utf8:
      answer.utf8 = str.utf8 + n.cv;
      return answer;

    case uni_utf16:
    case uni_utf16be:
    case uni_utf16le:
      answer.utf16 = str.utf16 + n.cv;
      return answer;

    case uni_utf32:
    case uni_utf32be:
    case uni_utf32le:
    case uni_bogus32:
    case uni_bogus32be:
    case uni_bogus32le:
      answer.utf32 = str.utf32 + n.cv;
      return answer;
    }
}


static inline__ void
shrink_cv_length_by (ustr_cv_index_t * new_len,
                     ustr_cv_index_t len,
                     ustr_cv_index_t n)
{
  if (new_len)
    {
      if (n.cv > len.cv)
        new_len->cv = 0;
      else
        new_len->cv = len.cv - n.cv;
    }
}

inline__ uni_string
ustri_cv_offset_n (ustr_cv_index_t * new_length,
                   uni_string str,
                   enum uni_encoding_scheme enc,
                   ustr_cv_index_t len,
                   ustr_cv_index_t n)
{
  uni_string answer;

  switch (enc)
    {
    default:
    case uni_iso8859_1:
    case uni_utf8:
      answer.utf8 = str.utf8 + n.cv;
      shrink_cv_length_by (new_length, len, n);
      return answer;

    case uni_utf16:
    case uni_utf16be:
    case uni_utf16le:
      answer.utf16 = str.utf16 + n.cv;
      shrink_cv_length_by (new_length, len, n);
      return answer;

    case uni_utf32:
    case uni_utf32be:
    case uni_utf32le:
    case uni_bogus32:
    case uni_bogus32be:
    case uni_bogus32le:
      answer.utf32 = str.utf32 + n.cv;
      shrink_cv_length_by (new_length, len, n);
      return answer;
    }
}




static inline__ void
string_from_scanner_pos (uni_string * new_string,
                         uni_string str,
                         enum uni_encoding_scheme enc,
                         size_t pos)
{
  if (new_string)
    {
      *new_string = ustri_cv_offset (str, enc, ustri_scanner_index_to_cv_index (enc, pos));
    }
}


inline__ t_unicode
ustri_scan (uni_string * str_after,
            uni_string str,
            enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_iso8859_1_scan (str.iso8859_1, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_iso8859_1, pos);
        return c;
      }

    case uni_utf8:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf8_scan (str.utf8, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_utf8, pos);
        return c;
      }

    case uni_utf16:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf16_scan (str.utf16, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_utf16, pos);
        return c;
      }

    case uni_utf32:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf32_scan (str.utf32, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_utf32, pos);
        return c;
      }

    case uni_bogus32:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_bogus32_scan (str.bogus32, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_bogus32, pos);
        return c;
      }


    case uni_utf16be:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf16be_iscan (str.raw, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_utf16be, pos);
        return c;
      }

    case uni_utf32be:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf32be_iscan (str.raw, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_utf32be, pos);
        return c;
      }

    case uni_bogus32be:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_bogus32be_iscan (str.raw, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_bogus32be, pos);
        return c;
      }


    case uni_utf16le:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf16le_iscan (str.raw, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_utf16le, pos);
        return c;
      }

    case uni_utf32le:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf32le_iscan (str.raw, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_utf32le, pos);
        return c;
      }

    case uni_bogus32le:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_bogus32le_iscan (str.raw, &pos, pos + 100);
        string_from_scanner_pos (str_after, str, uni_bogus32le, pos);
        return c;
      }
    }
}


inline__ ustr_cv_index_t
ustri_str_subtract (uni_string a,
                    uni_string b,
                    enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustr_cv_index ((ssize_t)(a.iso8859_1 - b.iso8859_1));
    case uni_utf8:
      return ustr_cv_index ((ssize_t)(a.utf8 - b.utf8));
    case uni_utf16:
    case uni_utf16be:
    case uni_utf16le:
      return ustr_cv_index ((ssize_t)(a.utf16 - b.utf16));
    case uni_utf32:
    case uni_utf32be:
    case uni_utf32le:
      return ustr_cv_index ((ssize_t)(a.utf32 - b.utf32));
    case uni_bogus32:
    case uni_bogus32be:
    case uni_bogus32le:
      return ustr_cv_index ((ssize_t)(a.bogus32 - b.bogus32));
    }

}

static inline__ void
ustri_scan_n_case (uni_string * str_after,
                   ustr_cv_index_t * len_after,
                   uni_string str,
                   enum uni_encoding_scheme enc,
                   ustr_cv_index_t length,
                   size_t scanner_pos)
{
  string_from_scanner_pos (str_after, str, enc, scanner_pos);
  if (len_after)
    len_after->cv = length.cv - ustri_scanner_index_to_cv_index (enc, scanner_pos).cv;
}


inline__ t_unicode
ustri_scan_n (uni_string * str_after,
              ustr_cv_index_t * len_after,
              uni_string str,
              enum uni_encoding_scheme enc,
              ustr_cv_index_t length)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_iso8859_1_scan (str.iso8859_1, &pos, ustri_cv_index_to_scanner_index (uni_iso8859_1, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }
    case uni_utf8:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf8_scan (str.utf8, &pos, ustri_cv_index_to_scanner_index (uni_utf8, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }
    case uni_utf16:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf16_scan (str.utf16, &pos, ustri_cv_index_to_scanner_index (uni_utf16, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }
    case uni_utf32:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf32_scan (str.utf32, &pos, ustri_cv_index_to_scanner_index (uni_utf32, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }
    case uni_bogus32:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_bogus32_scan (str.bogus32, &pos, ustri_cv_index_to_scanner_index (uni_bogus32, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }

    case uni_utf16be:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf16be_iscan (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_utf16be, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }
    case uni_utf32be:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf32be_iscan (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_utf32be, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }
    case uni_bogus32be:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_bogus32be_iscan (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_bogus32be, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }

    case uni_utf16le:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf16le_iscan (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_utf16le, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }
    case uni_utf32le:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_utf32le_iscan (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_utf32le, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }
    case uni_bogus32le:
      {
        size_t pos = 0;
        t_unicode c;

        c = uni_bogus32le_iscan (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_bogus32le, length));
        ustri_scan_n_case (str_after, len_after, str, enc, length, pos);
        return c;
      }
    }
}



static inline__ int
ustr_put_case_case_n (uni_string * str_after,
                      ustr_cv_index_t * len_after,
                      uni_string str,
                      enum uni_encoding_scheme enc,
                      ustr_cv_index_t len,
                      int put_result)
{
  invariant (put_result > 0);

  if (str_after)
    *str_after = ustri_cv_offset (str, enc, ustri_scanner_index_to_cv_index (enc, (size_t)put_result));
  
  if (len_after)
    len_after->cv = len.cv - (ssize_t)put_result;
  
  return 0;
}


static inline__ void
ustri_shift_characters_case (uni_string str,
                             enum uni_encoding_scheme enc,
                             ustr_cv_index_t amount)
{
  uni_string dest;
  ustr_cv_index_t cv_len_with_0;
  uni_string past_0;
  size_t move_amt;

  dest = ustri_cv_offset (str, enc, amount);

  cv_len_with_0 = ustri_cv_length (str, enc);
  ++cv_len_with_0.cv;
  past_0 = ustri_cv_offset (str, enc, cv_len_with_0);
  move_amt = (past_0.raw - str.raw);
  mem_move (dest.raw, str.raw, move_amt);
}


static inline__ void
ustri_shift_characters_n_case (uni_string str,
                               enum uni_encoding_scheme enc,
                               ustr_cv_index_t len,
                               ustr_cv_index_t amount)
{
  uni_string dest;
  uni_string past_dest;
  size_t move_amt;

  dest = ustri_cv_offset (str, enc, amount);
  past_dest = ustri_cv_offset (dest, enc, len);
  move_amt = (past_dest.raw - dest.raw);
  mem_move (dest.raw, str.raw, move_amt);
  mem_set0 (past_dest.raw, uni_cv_sizeof (enc));
}


static inline__ void
ustri_shift_characters (uni_string str,
                        enum uni_encoding_scheme enc,
                        ustr_cv_index_t len,
                        ustr_cv_index_t amount,
                        int shift_by_final_null)
{
  if (shift_by_final_null)
    {
      switch (enc)
        {
        default:
        case uni_iso8859_1:
          ustri_shift_characters_case (str, uni_iso8859_1, amount);
          return;
        case uni_utf8:
          ustri_shift_characters_case (str, uni_utf8, amount);
          return;
        case uni_utf16:
          ustri_shift_characters_case (str, uni_utf16, amount);
          return;
        case uni_utf32:
          ustri_shift_characters_case (str, uni_utf32, amount);
          return;
        case uni_bogus32:
          ustri_shift_characters_case (str, uni_bogus32, amount);
          return;
        case uni_utf16be:
          ustri_shift_characters_case (str, uni_utf16be, amount);
          return;
        case uni_utf32be:
          ustri_shift_characters_case (str, uni_utf32be, amount);
          return;
        case uni_bogus32be:
          ustri_shift_characters_case (str, uni_bogus32be, amount);
          return;
        case uni_utf16le:
          ustri_shift_characters_case (str, uni_utf16le, amount);
          return;
        case uni_utf32le:
          ustri_shift_characters_case (str, uni_utf32le, amount);
          return;
        case uni_bogus32le:
          ustri_shift_characters_case (str, uni_bogus32le, amount);
          return;
        }
    }
  else
    {
      switch (enc)
        {
        default:
        case uni_iso8859_1:
          ustri_shift_characters_n_case (str, uni_iso8859_1, len, amount);
          return;
        case uni_utf8:
          ustri_shift_characters_n_case (str, uni_utf8, len, amount);
          return;
        case uni_utf16:
          ustri_shift_characters_n_case (str, uni_utf16, len, amount);
          return;
        case uni_utf32:
          ustri_shift_characters_n_case (str, uni_utf32, len, amount);
          return;
        case uni_bogus32:
          ustri_shift_characters_n_case (str, uni_bogus32, len, amount);
          return;
        case uni_utf16be:
          ustri_shift_characters_n_case (str, uni_utf16be, len, amount);
          return;
        case uni_utf32be:
          ustri_shift_characters_n_case (str, uni_utf32be, len, amount);
          return;
        case uni_bogus32be:
          ustri_shift_characters_n_case (str, uni_bogus32be, len, amount);
          return;
        case uni_utf16le:
          ustri_shift_characters_n_case (str, uni_utf16le, len, amount);
          return;
        case uni_utf32le:
          ustri_shift_characters_n_case (str, uni_utf32le, len, amount);
          return;
        case uni_bogus32le:
          ustri_shift_characters_n_case (str, uni_bogus32le, len, amount);
          return;
        }
    }
}


static inline__ int
ustri_put_n_case (uni_string * str_after,
                  ustr_cv_index_t * len_after,
                  uni_string str,
                  enum uni_encoding_scheme enc,
                  ustr_cv_index_t len,
                  t_unicode c,
                  enum ustr_set_type set_type,
                  int shift_by_final_null)
{
  switch (set_type)
    {
    case ustr_fit:
      {
        t_unicode c_was = ustri_scan_n (0, 0, str, enc, len);
        int avail = (int)uni_cv_length_of_codepoint_in_encoding (enc, c_was);
        int wanted = (int)uni_cv_length_of_codepoint_in_encoding (enc, c);

        if (avail < 0)
          avail = 1;

        if (wanted < 0)
          return wanted;

        if (avail != wanted)
          return -1;

        goto store_character;
      }

    case ustr_shift:
      {
        t_unicode c_was = ustri_scan_n (0, 0, str, enc, len);
        int avail = (int)uni_cv_length_of_codepoint_in_encoding (enc, c_was);
        int wanted = (int)uni_cv_length_of_codepoint_in_encoding (enc, c);

        if (avail != wanted)
          ustri_shift_characters (str, enc, len, ustr_cv_index ((ssize_t)(wanted - avail)), shift_by_final_null);

        goto store_character;
      }


    default:
    case ustr_over:
      {
        int wanted = (int)uni_cv_length_of_codepoint_in_encoding (enc, c);

        if (wanted < len.cv)
          return -1;

      store_character:
        {
          size_t pos = 0;

          pos = 0;
          switch (enc)
            {
            default:
            case uni_iso8859_1:
              return ustr_put_case_case_n (str_after, len_after, str, uni_iso8859_1, len, uni_iso8859_1_put (str.iso8859_1, &pos, ustri_cv_index_to_scanner_index (uni_iso8859_1, len), c));
            case uni_utf8:
              return ustr_put_case_case_n (str_after, len_after, str, uni_utf8, len, uni_utf8_put (str.utf8, &pos, ustri_cv_index_to_scanner_index (uni_utf8, len), c));
            case uni_utf16:
              return ustr_put_case_case_n (str_after, len_after, str, uni_utf16, len, uni_utf16_put (str.utf16, &pos, ustri_cv_index_to_scanner_index (uni_utf16, len), c));
            case uni_utf32:
              return ustr_put_case_case_n (str_after, len_after, str, uni_utf32, len, uni_utf32_put (str.utf32, &pos, ustri_cv_index_to_scanner_index (uni_utf32, len), c));
            case uni_bogus32:
              return ustr_put_case_case_n (str_after, len_after, str, uni_bogus32, len, uni_bogus32_put (str.bogus32, &pos, ustri_cv_index_to_scanner_index (uni_bogus32, len), c));
            case uni_utf16be:
              return ustr_put_case_case_n (str_after, len_after, str, uni_utf16be, len, uni_utf16be_iput (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_utf16be, len), c));
            case uni_utf32be:
              return ustr_put_case_case_n (str_after, len_after, str, uni_utf32be, len, uni_utf32be_iput (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_utf32be, len), c));
            case uni_bogus32be:
              return ustr_put_case_case_n (str_after, len_after, str, uni_bogus32be, len, uni_bogus32be_iput (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_bogus32be, len), c));
            case uni_utf16le:
              return ustr_put_case_case_n (str_after, len_after, str, uni_utf16le, len, uni_utf16le_iput (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_utf16le, len), c));
            case uni_utf32le:
              return ustr_put_case_case_n (str_after, len_after, str, uni_utf32le, len, uni_utf32le_iput (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_utf32le, len), c));
            case uni_bogus32le:
              return ustr_put_case_case_n (str_after, len_after, str, uni_bogus32le, len, uni_bogus32le_iput (str.raw, &pos, ustri_cv_index_to_scanner_index (uni_bogus32le, len), c));
            }
        }
      }
    }
}


static inline__ int
ustri_put_n_cases (uni_string * str_after,
                   ustr_cv_index_t * len_after,
                   uni_string str,
                   enum uni_encoding_scheme enc,
                   ustr_cv_index_t len,
                   t_unicode c,
                   enum ustr_set_type set_type,
                   int shift_by_final_null)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_put_n_case (str_after, len_after, str, uni_iso8859_1, len, c, set_type, shift_by_final_null);
    case uni_utf8:
      return ustri_put_n_case (str_after, len_after, str, uni_utf8, len, c, set_type, shift_by_final_null);
    case uni_utf16:
      return ustri_put_n_case (str_after, len_after, str, uni_utf16, len, c, set_type, shift_by_final_null);
    case uni_utf32:
      return ustri_put_n_case (str_after, len_after, str, uni_utf32, len, c, set_type, shift_by_final_null);
    case uni_bogus32:
      return ustri_put_n_case (str_after, len_after, str, uni_bogus32, len, c, set_type, shift_by_final_null);
    case uni_utf16be:
      return ustri_put_n_case (str_after, len_after, str, uni_utf16be, len, c, set_type, shift_by_final_null);
    case uni_utf32be:
      return ustri_put_n_case (str_after, len_after, str, uni_utf32be, len, c, set_type, shift_by_final_null);
    case uni_bogus32be:
      return ustri_put_n_case (str_after, len_after, str, uni_bogus32be, len, c, set_type, shift_by_final_null);
    case uni_utf16le:
      return ustri_put_n_case (str_after, len_after, str, uni_utf16le, len, c, set_type, shift_by_final_null);
    case uni_utf32le:
      return ustri_put_n_case (str_after, len_after, str, uni_utf32le, len, c, set_type, shift_by_final_null);
    case uni_bogus32le:
      return ustri_put_n_case (str_after, len_after, str, uni_bogus32le, len, c, set_type, shift_by_final_null);
    }
}

inline__ int
ustri_put (uni_string * str_after,
           uni_string str,
           enum uni_encoding_scheme enc,
           t_unicode c,
           enum ustr_set_type set_type)
{
  switch (set_type)
    {
    case ustr_fit:
      {
        return ustri_put_n_cases (str_after, 0, str, enc, ustr_cv_index ((ssize_t)100), c, ustr_fit, 0);
      }
    case ustr_shift:
      {
        return ustri_put_n_cases (str_after, 0, str, enc, ustr_cv_index ((ssize_t)100), c, ustr_shift, 1);
      }
    default:
    case ustr_over:
      {
        return ustri_put_n_cases (str_after, 0, str, enc, ustr_cv_index ((ssize_t)100), c, ustr_over, 0);
      }
    }
}


inline__ int
ustri_put_n (uni_string * str_after,
             ustr_cv_index_t * len_after,
             uni_string str,
             enum uni_encoding_scheme enc,
             ustr_cv_index_t len,
             t_unicode c,
             enum ustr_set_type set_type)
{
  switch (set_type)
    {
    case ustr_fit:
      {
        return ustri_put_n_cases (str_after, len_after, str, enc, len, c, ustr_fit, 0);
      }
    case ustr_shift:
      {
        return ustri_put_n_cases (str_after, len_after, str, enc, len, c, ustr_shift, 0);
      }
    default:
    case ustr_over:
      {
        return ustri_put_n_cases (str_after, len_after, str, enc, len, c, ustr_over, 0);
      }
    }
}






static inline__ t_unicode
ustri_cv_ref_case (ustr_cv_index_t * index_after,
                 uni_string str,
                 enum uni_encoding_scheme enc,
                 ustr_cv_index_t index)
{
  if (!index_after)
    return ustri_scan (0, ustri_cv_offset (str, enc, index), enc);
  else
    {
      uni_string str_after;
      t_unicode answer;
            
      answer = ustri_scan (&str_after, ustri_cv_offset (str, enc, index), enc);
      *index_after = ustr_str_subtract (str_after, str, enc);
      return answer;
    }
}


inline__ t_unicode
ustri_cv_ref (ustr_cv_index_t * index_after,
            uni_string str,
            enum uni_encoding_scheme enc,
            ustr_cv_index_t index)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1: return ustri_cv_ref_case (index_after, str, uni_iso8859_1, index);
    case uni_utf8:      return ustri_cv_ref_case (index_after, str, uni_utf8, index);
    case uni_utf16:     return ustri_cv_ref_case (index_after, str, uni_utf16, index);
    case uni_utf32:     return ustri_cv_ref_case (index_after, str, uni_utf32, index);
    case uni_bogus32:   return ustri_cv_ref_case (index_after, str, uni_bogus32, index);
    case uni_utf16be:   return ustri_cv_ref_case (index_after, str, uni_utf16be, index);
    case uni_utf32be:   return ustri_cv_ref_case (index_after, str, uni_utf32be, index);
    case uni_bogus32be: return ustri_cv_ref_case (index_after, str, uni_bogus32be, index);
    case uni_utf16le:   return ustri_cv_ref_case (index_after, str, uni_utf16le, index);
    case uni_utf32le:   return ustri_cv_ref_case (index_after, str, uni_utf32le, index);
    case uni_bogus32le: return ustri_cv_ref_case (index_after, str, uni_bogus32le, index);
    }
}


static inline__ t_unicode
ustri_cv_ref_n_case (ustr_cv_index_t * index_after,
                   uni_string str,
                   enum uni_encoding_scheme enc,
                   ustr_cv_index_t len,
                   ustr_cv_index_t index)
{
  if (len.cv < index.cv)
    return UNI_SCAN_INPUT_TRUNCATED;
  else if (!index_after)
    {
      return ustri_scan_n (0, 0, ustri_cv_offset (str, enc, index), enc, ustr_cv_index (len.cv - index.cv));
    }
  else
    {
      uni_string str_after;
      t_unicode answer;
            
      answer = ustri_scan_n (&str_after, 0, ustri_cv_offset (str, enc, index), enc, ustr_cv_index (len.cv - index.cv));
      *index_after = ustr_str_subtract (str_after, str, enc);
      return answer;
    }
}


inline__ t_unicode
ustri_cv_ref_n (ustr_cv_index_t * index_after,
              uni_string str,
              enum uni_encoding_scheme enc,
              ustr_cv_index_t len,
              ustr_cv_index_t index)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1: return ustri_cv_ref_n_case (index_after, str, uni_iso8859_1, len, index);
    case uni_utf8:      return ustri_cv_ref_n_case (index_after, str, uni_utf8, len, index);
    case uni_utf16:     return ustri_cv_ref_n_case (index_after, str, uni_utf16, len, index);
    case uni_utf32:     return ustri_cv_ref_n_case (index_after, str, uni_utf32, len, index);
    case uni_bogus32:   return ustri_cv_ref_n_case (index_after, str, uni_bogus32, len, index);
    case uni_utf16be:   return ustri_cv_ref_n_case (index_after, str, uni_utf16be, index, len);
    case uni_utf32be:   return ustri_cv_ref_n_case (index_after, str, uni_utf32be, len, index);
    case uni_bogus32be: return ustri_cv_ref_n_case (index_after, str, uni_bogus32be, len, index);
    case uni_utf16le:   return ustri_cv_ref_n_case (index_after, str, uni_utf16le, len, index);
    case uni_utf32le:   return ustri_cv_ref_n_case (index_after, str, uni_utf32le, len, index);
    case uni_bogus32le: return ustri_cv_ref_n_case (index_after, str, uni_bogus32le, len, index);
    }
}




static inline__ int
ustri_cv_set_case (ustr_cv_index_t * index_after,
                   uni_string str,
                   enum uni_encoding_scheme enc,
                   ustr_cv_index_t index,
                   t_unicode c,
                   enum ustr_set_type set_type)
{
  if (!index_after)
    return ustri_put (0, ustri_cv_offset (str, enc, index), enc, c, set_type);
  else
    {
      uni_string str_after;
      int answer;
            
      answer = ustri_put (&str_after, ustri_cv_offset (str, enc, index), enc, c, set_type);
      *index_after = ustr_str_subtract (str_after, str, enc);
      return answer;
    }
}

inline__ int
ustri_cv_set (ustr_cv_index_t * index_after,
              uni_string str,
              enum uni_encoding_scheme enc,
              ustr_cv_index_t index,
              t_unicode c,
              enum ustr_set_type set_type)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1: return ustri_cv_set_case (index_after, str, uni_iso8859_1, index, c, set_type);
    case uni_utf8:      return ustri_cv_set_case (index_after, str, uni_utf8, index, c, set_type);
    case uni_utf16:     return ustri_cv_set_case (index_after, str, uni_utf16, index, c, set_type);
    case uni_utf32:     return ustri_cv_set_case (index_after, str, uni_utf32, index, c, set_type);
    case uni_bogus32:   return ustri_cv_set_case (index_after, str, uni_bogus32, index, c, set_type);
    case uni_utf16be:   return ustri_cv_set_case (index_after, str, uni_utf16be, index, c, set_type);
    case uni_utf32be:   return ustri_cv_set_case (index_after, str, uni_utf32be, index, c, set_type);
    case uni_bogus32be: return ustri_cv_set_case (index_after, str, uni_bogus32be, index, c, set_type);
    case uni_utf16le:   return ustri_cv_set_case (index_after, str, uni_utf16le, index, c, set_type);
    case uni_utf32le:   return ustri_cv_set_case (index_after, str, uni_utf32le, index, c, set_type);
    case uni_bogus32le: return ustri_cv_set_case (index_after, str, uni_bogus32le, index, c, set_type);
    }
}

static inline__ int
ustri_cv_set_n_case (ustr_cv_index_t * index_after,
                     ustr_cv_index_t * len_after,
                     uni_string str,
                     enum uni_encoding_scheme enc,
                     ustr_cv_index_t len,
                     ustr_cv_index_t index,
                     t_unicode c,
                     enum ustr_set_type set_type)
{
  if (!index_after && !len_after)
    {
      return ustri_put_n (0, 0, ustri_cv_offset (str, enc, index), enc, ustr_cv_index (len.cv - index.cv), c, set_type);
    }
  else
    {
      uni_string after_str;
      ustr_cv_index_t after_len;
      int answer;
            
      answer = ustri_put_n (&after_str, &after_len, ustri_cv_offset (str, enc, index), enc, ustr_cv_index (len.cv - index.cv) , c, set_type);
      if (index_after)
        *index_after = ustr_str_subtract (after_str, str, enc);
      if (len_after)
        len_after->cv = after_len.cv + index.cv;
      return answer;
    }
}


inline__ int
ustri_cv_set_n (ustr_cv_index_t * index_after,
                ustr_cv_index_t * len_after,
                uni_string str,
                enum uni_encoding_scheme enc,
                ustr_cv_index_t len,
                ustr_cv_index_t index,
                t_unicode c,
                enum ustr_set_type set_type)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1: return ustri_cv_set_n_case (index_after, len_after, str, uni_iso8859_1, len, index, c, set_type);
    case uni_utf8:      return ustri_cv_set_n_case (index_after, len_after, str, uni_utf8, len, index, c, set_type);
    case uni_utf16:     return ustri_cv_set_n_case (index_after, len_after, str, uni_utf16, len, index, c, set_type);
    case uni_utf32:     return ustri_cv_set_n_case (index_after, len_after, str, uni_utf32, len, index, c, set_type);
    case uni_bogus32:   return ustri_cv_set_n_case (index_after, len_after, str, uni_bogus32, len, index, c, set_type);
    case uni_utf16be:   return ustri_cv_set_n_case (index_after, len_after, str, uni_utf16be, len, index, c, set_type);
    case uni_utf32be:   return ustri_cv_set_n_case (index_after, len_after, str, uni_utf32be, len, index, c, set_type);
    case uni_bogus32be: return ustri_cv_set_n_case (index_after, len_after, str, uni_bogus32be, len, index, c, set_type);
    case uni_utf16le:   return ustri_cv_set_n_case (index_after, len_after, str, uni_utf16le, len, index, c, set_type);
    case uni_utf32le:   return ustri_cv_set_n_case (index_after, len_after, str, uni_utf32le, len, index, c, set_type);
    case uni_bogus32le: return ustri_cv_set_n_case (index_after, len_after, str, uni_bogus32le, len, index, c, set_type);
    }
}



static inline__ uni_string
ustri_cp_offset_case (uni_string str,
                      enum uni_encoding_scheme enc,
                      ustr_cp_index_t n)
{
  if (!str.raw)
    return str;

  while (n.cp)
    {
      t_unicode c;
      uni_string new_str;

      c = ustri_scan (&new_str, str, enc);

      if (!c || (   (c == UNI_SCAN_INPUT_TRUNCATED)
                 && (enc != uni_bogus32)
                 && (enc != uni_bogus32be)
                 && (enc != uni_bogus32le)))
        {
          uni_string answer;
          
          answer.raw = 0;
          return answer;
        }

      str = new_str;
      --n.cp;
    }

  return str;
}


inline__ uni_string
ustri_cp_offset (uni_string str,
                 enum uni_encoding_scheme enc,
                 ustr_cp_index_t n)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_cp_offset_case (str, uni_iso8859_1, n);
    case uni_utf8:
      return ustri_cp_offset_case (str, uni_utf8, n);
    case uni_utf16:
      return ustri_cp_offset_case (str, uni_utf16, n);
    case uni_utf32:
      return ustri_cp_offset_case (str, uni_utf32, n);
    case uni_bogus32:
      return ustri_cp_offset_case (str, uni_bogus32, n);
    case uni_utf16be:
      return ustri_cp_offset_case (str, uni_utf16be, n);
    case uni_utf32be:
      return ustri_cp_offset_case (str, uni_utf32be, n);
    case uni_bogus32be:
      return ustri_cp_offset_case (str, uni_bogus32be, n);
    case uni_utf16le:
      return ustri_cp_offset_case (str, uni_utf16le, n);
    case uni_utf32le:
      return ustri_cp_offset_case (str, uni_utf32le, n);
    case uni_bogus32le:
      return ustri_cp_offset_case (str, uni_bogus32le, n);
    }
}

static inline__ uni_string
ustri_cp_offset_n_case (uni_string str,
                        enum uni_encoding_scheme enc,
                        ustr_cv_index_t len,
                        ustr_cp_index_t n)
{
  if (!str.raw)
    return str;

  while (len.cv)
    {
      t_unicode c;
      uni_string new_str;

      c = ustri_scan_n (&new_str, &len, str, enc, len);

      if (!c || (   (c == UNI_SCAN_INPUT_TRUNCATED)
                 && (enc != uni_bogus32)
                 && (enc != uni_bogus32be)
                 && (enc != uni_bogus32le)))
        {
          uni_string answer;
          
          answer.raw = 0;
          return answer;
        }

      str = new_str;
    }

  return str;
}


inline__ uni_string
ustri_cp_offset_n (uni_string str,
                   enum uni_encoding_scheme enc,
                   ustr_cv_index_t len,
                   ustr_cp_index_t n)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_cp_offset_n_case (str, uni_iso8859_1, len, n);
    case uni_utf8:
      return ustri_cp_offset_n_case (str, uni_utf8, len, n);
    case uni_utf16:
      return ustri_cp_offset_n_case (str, uni_utf16, len, n);
    case uni_utf32:
      return ustri_cp_offset_n_case (str, uni_utf32, len, n);
    case uni_bogus32:
      return ustri_cp_offset_n_case (str, uni_bogus32, len, n);
    case uni_utf16be:
      return ustri_cp_offset_n_case (str, uni_utf16be, len, n);
    case uni_utf32be:
      return ustri_cp_offset_n_case (str, uni_utf32be, len, n);
    case uni_bogus32be:
      return ustri_cp_offset_n_case (str, uni_bogus32be, len, n);
    case uni_utf16le:
      return ustri_cp_offset_n_case (str, uni_utf16le, len, n);
    case uni_utf32le:
      return ustri_cp_offset_n_case (str, uni_utf32le, len, n);
    case uni_bogus32le:
      return ustri_cp_offset_n_case (str, uni_bogus32le, len, n);
    }
}




/************************************************************************
 *(h1 "Unicode String Length")
 *
 *
 *
 */


static inline__ ustr_cv_index_t
ustri_lengths_case (ustr_cp_index_t * cp_length,
                    enum uni_encoding_scheme * fw_enc,
                    uni_string str,
                    enum uni_encoding_scheme enc)
{
  ustr_cv_index_t answer;
  ustr_cp_index_t cp_answer;
  enum uni_encoding_scheme fw;

  answer.cv = (ssize_t)0;
  cp_answer.cp = (ssize_t)0;
  fw = uni_iso8859_1;

  while (1)
    {
      t_unicode c;
      uni_string str_after;
      ustr_cv_index_t amt;

      c = ustri_scan (&str_after, str, enc);

      if (!c || ((c == UNI_SCAN_INPUT_TRUNCATED)
                 && (enc != uni_bogus32)
                 && (enc != uni_bogus32be)
                 && (enc != uni_bogus32le)))
        {
          if (cp_length)
            *cp_length = cp_answer;
          if (fw_enc)
            *fw_enc = fw;
          return answer;
        }

      amt = ustr_str_subtract (str_after, str, enc);
      str = str_after;
      answer.cv += amt.cv;
      ++cp_answer.cp;
      if (c > 255)
        {
          if (c < ((t_unicode)1 << 16))
            {
              if (fw == uni_iso8859_1)
                fw = uni_utf16;
            }
          else if (uni_is_codepoint (c))
            {
              if (fw != uni_bogus32)
                fw = uni_utf32;
            }
          else
            {
              fw = uni_bogus32;
            }
        }
    }
}


inline__ ustr_cv_index_t
ustri_lengths (ustr_cp_index_t * cp_length,
               enum uni_encoding_scheme * fw_enc,
               uni_string str,
               enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_iso8859_1);
    case uni_utf8:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_utf8);
    case uni_utf16:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_utf16);
    case uni_utf32:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_utf32);
    case uni_bogus32:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_bogus32);
    case uni_utf16be:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_utf16be);
    case uni_utf32be:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_utf32be);
    case uni_bogus32be:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_bogus32be);
    case uni_utf16le:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_utf16le);
    case uni_utf32le:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_utf32le);
    case uni_bogus32le:
      return ustri_lengths_case (cp_length, fw_enc, str, uni_bogus32le);
    }
}


static inline__ ustr_cv_index_t
ustri_lengths_n_case (ustr_cp_index_t * cp_length,
                      enum uni_encoding_scheme * fw_enc,
                      uni_string str,
                      enum uni_encoding_scheme enc,
                      ustr_cv_index_t len)
{
  ustr_cv_index_t answer;
  ustr_cp_index_t cp_answer;
  enum uni_encoding_scheme fw;

  answer.cv = (ssize_t)0;
  cp_answer.cp = (ssize_t)0;
  fw = uni_iso8859_1;

  while (len.cv)
    {
      t_unicode c;
      uni_string str_after;
      ustr_cv_index_t amt;

      c = ustri_scan (&str_after, str, enc);

      if ((c == UNI_SCAN_INPUT_TRUNCATED)
          && (enc != uni_bogus32)
          && (enc != uni_bogus32be)
          && (enc != uni_bogus32le))
        {
          break;
        }

      amt = ustr_str_subtract (str_after, str, enc);
      str = str_after;
      answer.cv += amt.cv;
      len.cv -= amt.cv;
      ++cp_answer.cp;
      if (c > 255)
        {
          if (c < ((t_unicode)1 << 16))
            {
              if (fw == uni_iso8859_1)
                fw = uni_utf16;
            }
          else if (uni_is_codepoint (c))
            {
              if (fw != uni_bogus32)
                fw = uni_utf32;
            }
          else
            {
              fw = uni_bogus32;
            }
        }
    }

  if (cp_length)
    *cp_length = cp_answer;
  if (fw_enc)
    *fw_enc = fw;
  return answer;
}


inline__ ustr_cv_index_t
ustri_lengths_n (ustr_cp_index_t * cp_length,
                 enum uni_encoding_scheme * fw_enc,
                 uni_string str,
                 enum uni_encoding_scheme enc,
                 ustr_cv_index_t len)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_iso8859_1, len);
    case uni_utf8:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_utf8, len);
    case uni_utf16:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_utf16, len);
    case uni_utf32:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_utf32, len);
    case uni_bogus32:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_bogus32, len);
    case uni_utf16be:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_utf16be, len);
    case uni_utf32be:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_utf32be, len);
    case uni_bogus32be:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_bogus32be, len);
    case uni_utf16le:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_utf16le, len);
    case uni_utf32le:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_utf32le, len);
    case uni_bogus32le:
      return ustri_lengths_n_case (cp_length, fw_enc, str, uni_bogus32le, len);
    }
}



static inline__ ustr_cv_index_t
ustri_cv_length_case (uni_string str,
                      enum uni_encoding_scheme enc)
{
  ustr_cv_index_t answer;

  answer.cv = (ssize_t)0;

  while (1)
    {
      t_unicode c;
      uni_string str_after;
      ustr_cv_index_t amt;

      c = ustri_scan (&str_after, str, enc);

      if (!c || ((c == UNI_SCAN_INPUT_TRUNCATED)
                 && (enc != uni_bogus32)
                 && (enc != uni_bogus32be)
                 && (enc != uni_bogus32le)))
        return answer;

      amt = ustr_str_subtract (str_after, str, enc);
      str = str_after;
      answer.cv += amt.cv;
    }
}


inline__ ustr_cv_index_t
ustri_cv_length (uni_string str,
                 enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_cv_length_case (str, uni_iso8859_1);
    case uni_utf8:
      return ustri_cv_length_case (str, uni_utf8);
    case uni_utf16:
      return ustri_cv_length_case (str, uni_utf16);
    case uni_utf32:
      return ustri_cv_length_case (str, uni_utf32);
    case uni_bogus32:
      return ustri_cv_length_case (str, uni_bogus32);
    case uni_utf16be:
      return ustri_cv_length_case (str, uni_utf16be);
    case uni_utf32be:
      return ustri_cv_length_case (str, uni_utf32be);
    case uni_bogus32be:
      return ustri_cv_length_case (str, uni_bogus32be);
    case uni_utf16le:
      return ustri_cv_length_case (str, uni_utf16le);
    case uni_utf32le:
      return ustri_cv_length_case (str, uni_utf32le);
    case uni_bogus32le:
      return ustri_cv_length_case (str, uni_bogus32le);
    }
}


static inline__ ustr_cv_index_t
ustri_cv_length_in_encoding_case (enum uni_encoding_scheme dest_enc,
                                  uni_string str,
                                  enum uni_encoding_scheme enc)
{
  ustr_cv_index_t answer;

  answer.cv = (ssize_t)0;

  while (1)
    {
      t_unicode c;
      ssize_t amt;

      c = ustri_scan (&str, str, enc);

      if (!c || ((c == UNI_SCAN_INPUT_TRUNCATED)
                 && (enc != uni_bogus32)
                 && (enc != uni_bogus32be)
                 && (enc != uni_bogus32le)))
        return answer;

      
      amt = uni_cv_length_of_codepoint_in_encoding (dest_enc, c);
      if (amt < 0)
        return answer;
      else
        answer.cv += amt;
    }
}


inline__ ustr_cv_index_t
ustri_cv_length_in_encoding (enum uni_encoding_scheme dest_enc,
                             uni_string str,
                             enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_iso8859_1);
    case uni_utf8:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_utf8);
    case uni_utf16:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_utf16);
    case uni_utf32:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_utf32);
    case uni_bogus32:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_bogus32);
    case uni_utf16be:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_utf16be);
    case uni_utf32be:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_utf32be);
    case uni_bogus32be:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_bogus32be);
    case uni_utf16le:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_utf16le);
    case uni_utf32le:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_utf32le);
    case uni_bogus32le:
      return ustri_cv_length_in_encoding_case (dest_enc, str, uni_bogus32le);
    }
}


static inline__ ustr_cv_index_t
ustri_cv_length_in_encoding_n_case (enum uni_encoding_scheme dest_enc,
                                    uni_string str,
                                    enum uni_encoding_scheme enc,
                                    ustr_cv_index_t len)
{
  ustr_cv_index_t answer;

  answer.cv = (ssize_t)0;

  while (len.cv)
    {
      t_unicode c;

      c = ustri_scan_n (&str, &len, str, enc, len);

      if (   (c == UNI_SCAN_INPUT_TRUNCATED)
          && (enc != uni_bogus32)
          && (enc != uni_bogus32be)
          && (enc != uni_bogus32le))
        return answer;

      answer.cv += uni_cv_length_of_codepoint_in_encoding (dest_enc, c);
    }
  return answer;
}


inline__ ustr_cv_index_t
ustri_cv_length_in_encoding_n (enum uni_encoding_scheme dest_enc,
                               uni_string str,
                               enum uni_encoding_scheme enc,
                               ustr_cv_index_t len)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_iso8859_1, len);
    case uni_utf8:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_utf8, len);
    case uni_utf16:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_utf16, len);
    case uni_utf32:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_utf32, len);
    case uni_bogus32:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_bogus32, len);
    case uni_utf16be:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_utf16be, len);
    case uni_utf32be:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_utf32be, len);
    case uni_bogus32be:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_bogus32be, len);
    case uni_utf16le:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_utf16le, len);
    case uni_utf32le:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_utf32le, len);
    case uni_bogus32le:
      return ustri_cv_length_in_encoding_n_case (dest_enc, str, uni_bogus32le, len);
    }
}


static inline__ ustr_cp_index_t
ustri_cp_length_case (enum uni_encoding_scheme * fw_enc,
                      uni_string str,
                      enum uni_encoding_scheme enc)
{
  ustr_cp_index_t answer;
  enum uni_encoding_scheme fw;

  fw = uni_iso8859_1;

  answer.cp = (ssize_t)0;

  while (1)
    {
      t_unicode c;

      c = ustri_scan (&str, str, enc);

      if (!c || ((c == UNI_SCAN_INPUT_TRUNCATED)
                 && (enc != uni_bogus32)
                 && (enc != uni_bogus32be)
                 && (enc != uni_bogus32le)))
        {
          if (fw_enc)
            *fw_enc = fw;
          return answer;
        }

      if (fw_enc)
        {
          if (c > 255)
            {
              if (c < ((t_unicode)1<<16))
                {
                  if (fw == uni_iso8859_1)
                    fw = uni_utf16;
                }
              else if (uni_is_codepoint (c) && (c != UNI_SCAN_INPUT_TRUNCATED))
                {
                  if (fw != uni_bogus32)
                    fw = uni_utf32;
                }
              else
                {
                  fw = uni_bogus32;
                }
            }
        }

      ++answer.cp;
    }
}


inline__ ustr_cp_index_t
ustri_cp_length (enum uni_encoding_scheme * fw_enc,
                 uni_string str,
                 enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_cp_length_case (fw_enc, str, uni_iso8859_1);
    case uni_utf8:
      return ustri_cp_length_case (fw_enc, str, uni_utf8);
    case uni_utf16:
      return ustri_cp_length_case (fw_enc, str, uni_utf16);
    case uni_utf32:
      return ustri_cp_length_case (fw_enc, str, uni_utf32);
    case uni_bogus32:
      return ustri_cp_length_case (fw_enc, str, uni_bogus32);
    case uni_utf16be:
      return ustri_cp_length_case (fw_enc, str, uni_utf16be);
    case uni_utf32be:
      return ustri_cp_length_case (fw_enc, str, uni_utf32be);
    case uni_bogus32be:
      return ustri_cp_length_case (fw_enc, str, uni_bogus32be);
    case uni_utf16le:
      return ustri_cp_length_case (fw_enc, str, uni_utf16le);
    case uni_utf32le:
      return ustri_cp_length_case (fw_enc, str, uni_utf32le);
    case uni_bogus32le:
      return ustri_cp_length_case (fw_enc, str, uni_bogus32le);
    }
}


static inline__ ustr_cp_index_t
ustri_cp_length_n_case (enum uni_encoding_scheme * fw_enc,
                        uni_string str,
                        enum uni_encoding_scheme enc,
                        ustr_cv_index_t len)
{
  ustr_cp_index_t answer;
  enum uni_encoding_scheme fw;

  fw = uni_iso8859_1;

  answer.cp = (ssize_t)0;

  while (len.cv)
    {
      t_unicode c;

      c = ustri_scan_n (&str, &len, str, enc, len);

      if (   (c == UNI_SCAN_INPUT_TRUNCATED)
          && (enc != uni_bogus32)
          && (enc != uni_bogus32be)
          && (enc != uni_bogus32le))
        {
          if (fw_enc)
            *fw_enc = fw;
          return answer;
        }

      if (fw_enc)
        {
          if (c > 255)
            {
              if (c < ((t_unicode)1<<16))
                {
                  if (fw == uni_iso8859_1)
                    fw = uni_utf16;
                }
              else if (uni_is_codepoint (c) && (c != UNI_SCAN_INPUT_TRUNCATED))
                {
                  if (fw != uni_bogus32)
                    fw = uni_utf32;
                }
              else
                {
                  fw = uni_bogus32;
                }
            }
        }

      ++answer.cp;
    }
  return answer;
}


inline__ ustr_cp_index_t
ustri_cp_length_n (enum uni_encoding_scheme * fw_enc,
                   uni_string str,
                   enum uni_encoding_scheme enc,
                   ustr_cv_index_t len)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_cp_length_n_case (fw_enc, str, uni_iso8859_1, len);
    case uni_utf8:
      return ustri_cp_length_n_case (fw_enc, str, uni_utf8, len);
    case uni_utf16:
      return ustri_cp_length_n_case (fw_enc, str, uni_utf16, len);
    case uni_utf32:
      return ustri_cp_length_n_case (fw_enc, str, uni_utf32, len);
    case uni_bogus32:
      return ustri_cp_length_n_case (fw_enc, str, uni_bogus32, len);
    case uni_utf16be:
      return ustri_cp_length_n_case (fw_enc, str, uni_utf16be, len);
    case uni_utf32be:
      return ustri_cp_length_n_case (fw_enc, str, uni_utf32be, len);
    case uni_bogus32be:
      return ustri_cp_length_n_case (fw_enc, str, uni_bogus32be, len);
    case uni_utf16le:
      return ustri_cp_length_n_case (fw_enc, str, uni_utf16le, len);
    case uni_utf32le:
      return ustri_cp_length_n_case (fw_enc, str, uni_utf32le, len);
    case uni_bogus32le:
      return ustri_cp_length_n_case (fw_enc, str, uni_bogus32le, len);
    }
}




static inline__ uni_string
ustri_copy_case (uni_string dest,
                 enum uni_encoding_scheme dest_enc,
                 uni_string str,
                 enum uni_encoding_scheme enc)
{
  while (1)
    {
      t_unicode c;

      c = ustri_scan (&str, str, enc);
      if (!c)
        {
          ustri_put (&dest, dest, dest_enc, (t_unicode)0, ustr_over);
          return dest;
        }
      else if (   (c == UNI_SCAN_INPUT_TRUNCATED)
               && (enc != uni_bogus32)
               && (enc != uni_bogus32be)
               && (enc != uni_bogus32le))
        {
          return UNI_STRING_NULL;
        }
      else if (0 > ustri_put (&dest, dest, dest_enc, c, ustr_over))
        {
          return UNI_STRING_NULL;
        }
    }
}


inline__ uni_string
ustri_copy (uni_string dest,
            enum uni_encoding_scheme dest_enc,
            uni_string str,
            enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_copy_case (dest, dest_enc, str, uni_iso8859_1);
    case uni_utf8:
      return ustri_copy_case (dest, dest_enc, str, uni_utf8);
    case uni_utf16:
      return ustri_copy_case (dest, dest_enc, str, uni_utf16);
    case uni_utf32:
      return ustri_copy_case (dest, dest_enc, str, uni_utf32);
    case uni_bogus32:
      return ustri_copy_case (dest, dest_enc, str, uni_bogus32);
    case uni_utf16be:
      return ustri_copy_case (dest, dest_enc, str, uni_utf16be);
    case uni_utf32be:
      return ustri_copy_case (dest, dest_enc, str, uni_utf32be);
    case uni_bogus32be:
      return ustri_copy_case (dest, dest_enc, str, uni_bogus32be);
    case uni_utf16le:
      return ustri_copy_case (dest, dest_enc, str, uni_utf16le);
    case uni_utf32le:
      return ustri_copy_case (dest, dest_enc, str, uni_utf32le);
    case uni_bogus32le:
      return ustri_copy_case (dest, dest_enc, str, uni_bogus32le);
    }
}

static inline__ uni_string
ustri_copy_n_case (uni_string dest,
                   enum uni_encoding_scheme dest_enc,
                   ustr_cv_index_t dest_len,
                   uni_string str,
                   enum uni_encoding_scheme enc,
                   ustr_cv_index_t len)

{
  while (len.cv)
    {
      t_unicode c;

      c = ustri_scan_n (&str, &len, str, enc, len);

      if (   (c == UNI_SCAN_INPUT_TRUNCATED)
               && (enc != uni_bogus32)
               && (enc != uni_bogus32be)
               && (enc != uni_bogus32le))
        {
          return UNI_STRING_NULL;
        }
      else if (0 > ustri_put_n (&dest, &dest_len, dest, dest_enc, dest_len, c, ustr_over))
        {
          return UNI_STRING_NULL;
        }
    }
  return str;
}

inline__ uni_string
ustri_copy_n (uni_string dest,
              enum uni_encoding_scheme dest_enc,
              ustr_cv_index_t dest_len,
              uni_string str,
              enum uni_encoding_scheme enc,
              ustr_cv_index_t len)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_iso8859_1, len);
    case uni_utf8:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_utf8, len);
    case uni_utf16:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_utf16, len);
    case uni_utf32:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_utf32, len);
    case uni_bogus32:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_bogus32, len);
    case uni_utf16be:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_utf16be, len);
    case uni_utf32be:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_utf32be, len);
    case uni_bogus32be:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_bogus32be, len);
    case uni_utf16le:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_utf16le, len);
    case uni_utf32le:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_utf32le, len);
    case uni_bogus32le:
      return ustri_copy_n_case (dest, dest_enc, dest_len, str, uni_bogus32le, len);
    }
}




static inline__ uni_string
ustri_cat_case (uni_string dest,
                enum uni_encoding_scheme dest_enc,
                uni_string str,
                enum uni_encoding_scheme enc)
{
  ustr_cv_index_t dest_end;
  uni_string write_pos;
  uni_string answer;

  dest_end = ustri_cv_length (dest, dest_enc);
  write_pos = ustri_cv_offset (dest, dest_enc, dest_end);
  answer = ustri_copy (write_pos, dest_enc, str, enc);
  return answer;
}


inline__ uni_string
ustri_cat (uni_string dest,
              enum uni_encoding_scheme dest_enc,
              uni_string str,
              enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_cat_case (dest, dest_enc, str, uni_iso8859_1);
    case uni_utf8:
      return ustri_cat_case (dest, dest_enc, str, uni_utf8);
    case uni_utf16:
      return ustri_cat_case (dest, dest_enc, str, uni_utf16);
    case uni_utf32:
      return ustri_cat_case (dest, dest_enc, str, uni_utf32);
    case uni_bogus32:
      return ustri_cat_case (dest, dest_enc, str, uni_bogus32);
    case uni_utf16be:
      return ustri_cat_case (dest, dest_enc, str, uni_utf16be);
    case uni_utf32be:
      return ustri_cat_case (dest, dest_enc, str, uni_utf32be);
    case uni_bogus32be:
      return ustri_cat_case (dest, dest_enc, str, uni_bogus32be);
    case uni_utf16le:
      return ustri_cat_case (dest, dest_enc, str, uni_utf16le);
    case uni_utf32le:
      return ustri_cat_case (dest, dest_enc, str, uni_utf32le);
    case uni_bogus32le:
      return ustri_cat_case (dest, dest_enc, str, uni_bogus32le);
    }
}


static inline__ uni_string
ustri_cat_n_case (uni_string dest,
                  enum uni_encoding_scheme dest_enc,
                  ustr_cv_index_t dest_len,
                  uni_string str,
                  enum uni_encoding_scheme enc,
                  ustr_cv_index_t len)
{
  uni_string write_pos;
  uni_string answer;

  write_pos = ustri_cv_offset (dest, dest_enc, dest_len);
  answer = ustri_copy_n (write_pos, dest_enc, len, str, enc, len);
  return answer;
}


inline__ uni_string
ustri_cat_n (uni_string dest,
             enum uni_encoding_scheme dest_enc,
             ustr_cv_index_t dest_len,
             uni_string str,
             ustr_cv_index_t len,
             enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_iso8859_1, len);
    case uni_utf8:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_utf8, len);
    case uni_utf16:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_utf16, len);
    case uni_utf32:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_utf32, len);
    case uni_bogus32:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_bogus32, len);
    case uni_utf16be:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_utf16be, len);
    case uni_utf32be:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_utf32be, len);
    case uni_bogus32be:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_bogus32be, len);
    case uni_utf16le:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_utf16le, len);
    case uni_utf32le:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_utf32le, len);
    case uni_bogus32le:
      return ustri_cat_n_case (dest, dest_enc, dest_len, str, uni_bogus32le, len);
    }
}


static inline__ uni_string
ustri_save_case (alloc_limits limits,
                 enum uni_encoding_scheme dest_enc,
                 uni_string str,
                 enum uni_encoding_scheme enc)
{
  ustr_cv_index_t length;
  size_t alloc_length;
  uni_string answer;

  length = ustri_cv_length_in_encoding (dest_enc, str, enc);
  ++length.cv;                  /* for the final null */
  alloc_length = length.cv * uni_cv_sizeof (dest_enc);
  answer.raw = (t_uchar *)lim_malloc (limits, alloc_length);

  if (!answer.raw)
    return answer;
  else
    {
      uni_string end;

      end = ustri_copy (answer, dest_enc, str, enc);

      if (end.raw)
        return answer;
      else
        {
          lim_free (limits, answer.raw);
          return UNI_STRING_NULL;
        }
    }
}


inline__ uni_string
ustri_save (alloc_limits limits,
            enum uni_encoding_scheme dest_enc,
            uni_string str,
            enum uni_encoding_scheme enc)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_save_case (limits, dest_enc, str, uni_iso8859_1);
    case uni_utf8:
      return ustri_save_case (limits, dest_enc, str, uni_utf8);
    case uni_utf16:
      return ustri_save_case (limits, dest_enc, str, uni_utf16);
    case uni_utf32:
      return ustri_save_case (limits, dest_enc, str, uni_utf32);
    case uni_bogus32:
      return ustri_save_case (limits, dest_enc, str, uni_bogus32);
    case uni_utf16be:
      return ustri_save_case (limits, dest_enc, str, uni_utf16be);
    case uni_utf32be:
      return ustri_save_case (limits, dest_enc, str, uni_utf32be);
    case uni_bogus32be:
      return ustri_save_case (limits, dest_enc, str, uni_bogus32be);
    case uni_utf16le:
      return ustri_save_case (limits, dest_enc, str, uni_utf16le);
    case uni_utf32le:
      return ustri_save_case (limits, dest_enc, str, uni_utf32le);
    case uni_bogus32le:
      return ustri_save_case (limits, dest_enc, str, uni_bogus32le);
    }
}


inline__ uni_string
ustri_save_n_case (ustr_cv_index_t * dest_len,
                   alloc_limits limits,
                   enum uni_encoding_scheme dest_enc,
                   uni_string str,
                   enum uni_encoding_scheme enc,
                   ustr_cv_index_t len)
{
  ustr_cv_index_t length;
  size_t alloc_length;
  uni_string answer;

  length = ustri_cv_length_in_encoding_n (dest_enc, str, enc, len);
  alloc_length = (1 + length.cv) * uni_cv_sizeof (dest_enc);
  answer.raw = (t_uchar *)lim_malloc (limits, alloc_length);
  if (!answer.raw)
    {
      return answer;
    }
  else
    {
      uni_string end;

      end = ustri_copy_n (answer, dest_enc, len, str, enc, len);
      if (!end.raw)
        {
          lim_free (limits, answer.raw);
          return UNI_STRING_NULL;
        }
      else
        {
          if (dest_len)
            *dest_len = length;
          ustri_put (0, ustri_cv_offset (answer, dest_enc, length), dest_enc, (t_unicode)0, ustr_over);
          return answer;
        }
    }
}


inline__ uni_string
ustri_save_n (ustr_cv_index_t * dest_len,
            alloc_limits limits,
            enum uni_encoding_scheme dest_enc,
            uni_string str,
            enum uni_encoding_scheme enc,
            ustr_cv_index_t len)
{
  switch (enc)
    {
    default:
    case uni_iso8859_1:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_iso8859_1, len);
    case uni_utf8:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_utf8, len);
    case uni_utf16:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_utf16, len);
    case uni_utf32:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_utf32, len);
    case uni_bogus32:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_bogus32, len);
    case uni_utf16be:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_utf16be, len);
    case uni_utf32be:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_utf32be, len);
    case uni_bogus32be:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_bogus32be, len);
    case uni_utf16le:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_utf16le, len);
    case uni_utf32le:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_utf32le, len);
    case uni_bogus32le:
      return ustri_save_n_case (dest_len, limits, dest_enc, str, uni_bogus32le, len);
    }
}



#if 0
ustri_cmp ()
ustri_cmp_n ()

ustri_casecmp ()
ustri_casecmp_n ()
#endif



/* tag: Tom Lord Wed May 19 09:41:53 2004 (ustr-inline.c)
 */
