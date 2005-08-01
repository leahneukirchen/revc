/* ustr.c:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/strings/ustr.h"


/************************************************************************
 *(h0 "ustr_ -- Low Level Unicode Strings")
 *
 * The function in this chapter manipulate Unicode strings represented
 * simply as contiguous arrays of code units -- either 0-terminated or
 * passed to these functions with a separate `length' parameter.
 *
 * These functions operate on all supported encoding forms can be 
 * used with arguments in differing encoding forms.   For example, 
 * a UTF16 string can be appended to a UTF8 string -- the concatenation 
 * functions convert the UTF16 data to UTF8 internally.
 *
 * Some of the functions below take one or more `length' parameters
 * limiting the amount of input data to be used. These functions come
 * in two flavors: `_n' forms accept lengths that are expressed in
 * coding values, `_cn' forms accept lengths experessed in codepoints.
 *
 */
/*(menu)
 */



#include "ustr-inlines.c"



ustr_cv_index_t
ustr_cv_index (ssize_t n)
{
  return ustri_cv_index (n);
}


ustr_cp_index_t
ustr_cp_index (ssize_t n)
{
  return ustri_cp_index (n);
}


size_t
ustr_cv_index_to_scanner_index (enum uni_encoding_scheme enc,
                                ustr_cv_index_t n)
{
  return ustri_cv_index_to_scanner_index (enc, n);
}


ustr_cv_index_t
ustr_scanner_index_to_cv_index (enum uni_encoding_scheme enc,
                                size_t n)
{
  return ustri_scanner_index_to_cv_index (enc, n);
}


uni_string
ustr_cv_offset (uni_string str,
                enum uni_encoding_scheme enc,
                ustr_cv_index_t n)
{
  return ustri_cv_offset (str, enc, n);
}


uni_string
ustr_cv_offset_n (ustr_cv_index_t * new_length,
                  uni_string str,
                  enum uni_encoding_scheme enc,
                  ustr_cv_index_t len,
                  ustr_cv_index_t n)
{
  return ustri_cv_offset_n (new_length, str, enc, len, n);
}


t_unicode
ustr_scan (uni_string * str_after,
           uni_string str,
           enum uni_encoding_scheme enc)
{
  return ustri_scan (str_after, str, enc);
}


ustr_cv_index_t
ustr_str_subtract (uni_string a,
                   uni_string b,
                   enum uni_encoding_scheme enc)
{
  return ustri_str_subtract (a, b, enc);
}


t_unicode
ustr_scan_n (uni_string * str_after,
             ustr_cv_index_t * len_after,
             uni_string str,
             enum uni_encoding_scheme enc,
             ustr_cv_index_t length)
{
  return ustri_scan_n (str_after, len_after, str, enc, length);
}


int
ustr_put (uni_string * str_after,
          uni_string str,
          enum uni_encoding_scheme enc,
          t_unicode c,
          enum ustr_set_type set_type)
{
  return ustri_put (str_after, str, enc, c, set_type);
}


int
ustr_put_n (uni_string * str_after,
            ustr_cv_index_t * len_after,
            uni_string str,
            enum uni_encoding_scheme enc,
            ustr_cv_index_t len,
            t_unicode c,
            enum ustr_set_type set_type)
{
  return ustri_put_n (str_after, len_after, str, enc, len, c, set_type);
}


t_unicode
ustr_cv_ref (ustr_cv_index_t * index_after,
             uni_string str,
             enum uni_encoding_scheme enc,
             ustr_cv_index_t index)
{
  return ustri_cv_ref (index_after, str, enc, index);
}


t_unicode
ustr_cv_ref_n (ustr_cv_index_t * index_after,
               uni_string str,
               enum uni_encoding_scheme enc,
               ustr_cv_index_t len,
               ustr_cv_index_t index)
{
  return ustri_cv_ref_n (index_after, str, enc, len, index);
}


int
ustr_cv_set (ustr_cv_index_t * index_after,
             uni_string str,
             enum uni_encoding_scheme enc,
             ustr_cv_index_t index,
             t_unicode c,
             enum ustr_set_type set_type)
{
  return ustri_cv_set (index_after, str, enc, index, c, set_type);
}


int
ustr_cv_set_n (ustr_cv_index_t * index_after,
               ustr_cv_index_t * len_after,
               uni_string str,
               enum uni_encoding_scheme enc,
               ustr_cv_index_t len,
               ustr_cv_index_t index,
               t_unicode c,
               enum ustr_set_type set_type)
{
  return ustri_cv_set_n (index_after, len_after, str, enc, len, index, c, set_type);
}


uni_string
ustr_cp_offset (uni_string str,
                enum uni_encoding_scheme enc,
                ustr_cp_index_t n)
{
  return ustri_cp_offset (str, enc, n);
}


uni_string
ustr_cp_offset_n (uni_string str,
                  enum uni_encoding_scheme enc,
                  ustr_cv_index_t len,
                  ustr_cp_index_t n)
{
  return ustri_cp_offset_n (str, enc, len, n);
}


ustr_cv_index_t
ustr_lengths (ustr_cp_index_t * cp_length,
              enum uni_encoding_scheme * fw_enc,
              uni_string str,
              enum uni_encoding_scheme enc)
{
  return ustri_lengths (cp_length, fw_enc, str, enc);
}


ustr_cv_index_t
ustr_lengths_n (ustr_cp_index_t * cp_length,
                enum uni_encoding_scheme * fw_enc,
                uni_string str,
                enum uni_encoding_scheme enc,
                ustr_cv_index_t len)
{
  return ustri_lengths_n (cp_length, fw_enc, str, enc, len);
}


ustr_cv_index_t
ustr_cv_length (uni_string str,
                enum uni_encoding_scheme enc)
{
  return ustri_cv_length (str, enc);
}


ustr_cv_index_t
ustr_cv_length_in_encoding (enum uni_encoding_scheme dest_enc,
                            uni_string str,
                            enum uni_encoding_scheme enc)
{
  return ustri_cv_length_in_encoding (dest_enc, str, enc);
}


ustr_cv_index_t
ustr_cv_length_in_encoding_n (enum uni_encoding_scheme dest_enc,
                              uni_string str,
                              enum uni_encoding_scheme enc,
                              ustr_cv_index_t len)
{
  return ustri_cv_length_in_encoding_n (dest_enc, str, enc, len);
}


ustr_cp_index_t
ustr_cp_length (enum uni_encoding_scheme * fw_enc,
                uni_string str,
                enum uni_encoding_scheme enc)
{
  return ustri_cp_length (fw_enc, str, enc);
}


ustr_cp_index_t
ustr_cp_length_n (enum uni_encoding_scheme * fw_enc,
                  uni_string str,
                  enum uni_encoding_scheme enc,
                  ustr_cv_index_t len)
{
  return ustri_cp_length_n (fw_enc, str, enc, len);
}


uni_string
ustr_copy (uni_string dest,
           enum uni_encoding_scheme dest_enc,
           uni_string str,
           enum uni_encoding_scheme enc)
{
  return ustri_copy (dest, dest_enc, str, enc);
}


uni_string
ustr_copy_n (uni_string dest,
             enum uni_encoding_scheme dest_enc,
             ustr_cv_index_t dest_len,
             uni_string str,
             enum uni_encoding_scheme enc,
             ustr_cv_index_t len)
{
  return ustri_copy_n (dest, dest_enc, dest_len, str, enc, len);
}


uni_string
ustr_cat (uni_string dest,
          enum uni_encoding_scheme dest_enc,
          uni_string str,
          enum uni_encoding_scheme enc)
{
  return ustri_cat (dest, dest_enc, str, enc);
}


uni_string
ustr_cat_n (uni_string dest,
            enum uni_encoding_scheme dest_enc,
            ustr_cv_index_t dest_len,
            uni_string str,
            ustr_cv_index_t len,
            enum uni_encoding_scheme enc)
{
  return ustri_cat_n (dest, dest_enc, dest_len, str, len, enc);
}


uni_string
ustr_save (alloc_limits limits,
           enum uni_encoding_scheme dest_enc,
           uni_string str,
           enum uni_encoding_scheme enc)
{
  return ustri_save (limits, dest_enc, str, enc);
}


uni_string
ustr_save_n_case (ustr_cv_index_t * dest_len,
                  alloc_limits limits,
                  enum uni_encoding_scheme dest_enc,
                  uni_string str,
                  enum uni_encoding_scheme enc,
                  ustr_cv_index_t len)
{
  return ustri_save_n_case (dest_len, limits, dest_enc, str, enc, len);
}


uni_string
ustr_save_n (ustr_cv_index_t * dest_len,
             alloc_limits limits,
             enum uni_encoding_scheme dest_enc,
             uni_string str,
             enum uni_encoding_scheme enc,
             ustr_cv_index_t len)
{
  return ustri_save_n (dest_len, limits, dest_enc, str, enc, len);
}









/* tag: Tom Lord Thu Jan  1 13:54:53 2004 (ustr.c)
 */
