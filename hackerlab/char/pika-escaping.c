/* pika-escaping.c - decls for escaping/unescaping pika scheme strings
 *
 ****************************************************************
 * Copyright (C) 2004, Christian Thaeter
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include "hackerlab/fmt/cvt.h"
#include "hackerlab/char/char-class.h"
#include "hackerlab/char/pika-escaping.h"


/************************************************************************
 *(h1 "Pika Escaping")
 * 
 * ``Escaping'' a string means substituting a symbolic representation 
 * for some characters.   For example, the C character constant:
 * 
 *       "column 1\tcolumn 2\n"
 * 
 * contains an escaped tab character and an escaped newline.
 *
 * There are many different escaping syntaxes of which C's is only one
 * example.  As a coding standard, hackerlab programs should use the
 * syntax of Pika Scheme for all exchange formats.  (The Pika format
 * is simple, extensible, and handles Unicode well.  Additionally,
 * Pika Scheme is [will be] the preferred scripting and extension language
 * for Hackerlab programs.)
 * 
 * The functions in this section implement Pika Scheme escaping for
 * ordinary C strings (strings over the `char' type).
 */


/* __STDC__ prototypes for static functions */
static int str_print (size_t * needed,
                      t_uchar * to,
                      size_t to_length,
                      const t_uchar * from);


/************************************************************************
 *(h2 "Basic Pika Escaping Functions")
 * 
 * 
 * 
 */


/*(c pika_escape_iso8859_1_n)
 * size_t pika_escape_iso8859_1_n (t_uchar * result,
 *                                 size_t result_length,
 *                                 int escape_classes,
 *                                 const t_uchar * str,
 *                                 size_t length);
 * 
 * Fill up to `result_length' characters of `result'
 * with a Pika-escaped form of the `length' characters
 * beginning at `str' (including a final 0 character).
 *
 * Return the number of characters required to escape the input
 * string, not counting the final 0 character.  (This may be greater
 * than `result_length' if insufficient space was provided to contain
 * the result.)
 *
 * Note, therefore, that if the return value is greater than or 
 * equal to `result_length', then the output has been truncated
 * and no final 0 character appended.
 *
 * The parameter `escape_classes' is a bitwise or of 
 * values of type `enum char_class' (see xref:"Character
 * Classes").   Characters in the indicated classes
 * are replaced by hexadecimal or symbolic escapes
 * in the result -- other characters are unmodified.
 *
 * Backslashes and doublequotes are always escaped.
 *
 */
size_t
pika_escape_iso8859_1_n (t_uchar * result,
                         size_t result_length,
                         int escape_classes,
                         const t_uchar * str,
                         size_t length)
{
  size_t conv_out;

  conv_out = 0;

  while (length)
    {
      if (*str == '\\' || *str == '"' || char_is_any_of (*str, escape_classes))
        {
          size_t needed;
          int success;

          switch (*str)
            {
            case '\t':
              success = str_print (&needed, result, result_length, "\\(tab)");
              break;
            case '\n':
              success = str_print (&needed, result, result_length, "\\(nl)");
              break;
            case '\r':
              success = str_print (&needed, result, result_length, "\\(cr)");
              break;
            case '\f':
              success = str_print (&needed, result, result_length, "\\(np)");
              break;
            case ' ':
              success = str_print (&needed, result, result_length, "\\(sp)");
              break;
            case '\\':
              success = str_print (&needed, result, result_length, "\\\\");
              break;
            case '"':
              success = str_print (&needed, result, result_length, "\\\"");
              break;
            default:
              success = str_print (&needed, result, result_length, "\\(U+");
              conv_out += needed;

              if (success == 0)
                {
                  result += needed;
                  result_length -= needed;
                }

              success = cvt_ulong_to_HEX_n (result, result_length, (unsigned long) *str, &needed);
              conv_out += needed;

              if (success == 0)
                {
                  result += needed;
                  result_length -= needed;
                }

              success = str_print (&needed, result, result_length, ")");
              break;
            }
          conv_out += needed;
          if (success == 0)
            {
              result += needed;
              result_length -= needed;
            }
        }
      else
        {
          if (result_length)
            {
              *result = *str;
              --result_length;
              ++result;
            }
          ++conv_out;
        }
      --length;
      ++str;
    }

  if (result_length)
    *result = '\0';

  return conv_out;
}


/*(c pika_unescape_iso8859_1_n)
 * int pika_unescape_iso8859_1_n (size_t * result_needed,
 *                                size_t * str_used,
 *                                t_uchar * result,
 *                                size_t result_length,
 *                                const t_uchar * str,
 *                                size_t length);
 * 
 * Fill up to `result_length' characters of `result'
 * with the Pika-unescaped form of the `length' characters
 * beginning at `str' plus a final 0.
 *
 * Return in `*result_needed' the number of characters required to
 * unescape the input string, not counting the final 0.  (This may be
 * greater than `result_length' if insufficient space was provided to
 * contain the result.)
 *
 * Note, therefore, that the function succeeds only if the value
 * to be returned in `*result_needed' is less than `result_length'.
 *
 * If `str_used' is not 0, return in `*str_used' the number
 * of characters consumed from `str' (normally `length').
 *
 * Return 0 on success, -1 if either `result_length' is not large
 * enough to hold the unescaped result plus the final 0 or if the
 * input string contains a syntax error.
 *
 * If `*result_needed >= result_length' then there was
 * not enough room to unescape the input;  otherwise,
 * the input contains a syntax error.
 *
 * In either case, `*str_used' indicates the first character
 * within `str' that could not be unescaped in the result.  
 */
int
pika_unescape_iso8859_1_n (size_t * result_needed,
                           size_t * str_used,
                           t_uchar * result,
                           size_t result_length,
                           const t_uchar * str,
                           size_t length)
{
  int ret;
  size_t needed;
  size_t remain;
  const t_uchar * progress;
  const t_uchar * str_itr;

  ret = 0;
  needed = 0;
  remain = length;
  progress = str;
  str_itr = str;

  while (remain)
    {
      if (*str_itr == '\\')
        {
          static struct pika_escape_symbolic_names
          {
            char * name;
            size_t name_len;
            char translation;
          } symbolic_names[] =
          {
            { "(tab)", 5, '\t' },
            { "(nl)", 4, '\n' },
            { "(cr)", 4, '\r' },
            { "(np)", 4, '\f' },
            { "(sp)", 4, ' ' },
            { "\\", 1, '\\' },
            { "\"", 1, '"' },
            { 0, 0, 0 }
          };

          int x;

          ++str_itr;
          --remain;

          for (x = 0; symbolic_names[x].name; ++x)
            {
              if (!str_casecmp_prefix_n (symbolic_names[x].name, symbolic_names[x].name_len, str_itr, remain))
                {
                  str_itr += symbolic_names[x].name_len;
                  remain -= symbolic_names[x].name_len;
                  ++needed;
                  if (result_length)
                    {
                      *result = symbolic_names[x].translation;
                      ++result;
                      --result_length;
                    }
                  else
                    ret = -1;
                  break;
                }
            }

          if (!symbolic_names[x].name)
            {
              if (!str_casecmp_prefix_n ("(U+", (size_t)3, str_itr, remain))
                {
                  int err;
                  unsigned n;
                  size_t consumed;

                  str_itr += 3;
                  remain -= 3;

                  if (cvt_scan_hex_to_uint (&err, &n, &consumed, str_itr, remain))
                    goto einval;

                  if (n > 0xff)
                    goto einval;

                  str_itr += consumed;
                  remain -= consumed;

                  if (*str_itr != ')')
                    goto einval;

                  ++str_itr;
                  --remain;

                  ++needed;

                  if (result_length)
                    {
                      *result = (t_uchar)n;
                      ++result;
                      --result_length;
                    }
                  else
                    ret = -1;
                }
              else
                {
                einval:
                  ret = -1;
                  goto exit;
                }
            }
        }
      else
        {
          ++needed;
          --remain;
          if (result_length)
            {
              *result = *str_itr;
              ++result;
              --result_length;
              ++str_itr;
            }
          else
            ret = -1;
        }
      if (ret == 0)
        progress = str_itr;
    }

  if (result_length)
    *result = '\0';
  else
    ret = -1;

 exit:
  if (result_needed)
    *result_needed = needed;
  if (str_used)
    *str_used = progress - str;

  return ret;
}




static int
str_print (size_t * needed,
           t_uchar * to,
           size_t to_length,
           const t_uchar * from)
{
  size_t used;

  used = 0;

  while ((used < to_length) && *from)
    {
      *to++ = *from++;
      ++used;
    }

  if (*from)
    {
      while (*from)
        {
          ++used;
          ++from;
        }
      if (needed)
        *needed = used;
      return -1;
    }

  if (needed)
    *needed = used;

  return 0;
}


/*(include-documentation "pika-escaping-utils.c")
 */

