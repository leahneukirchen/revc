/* ustr.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__CHAR__USTR_H
#define INCLUDE__CHAR__USTR_H


#include "hackerlab/unicode/unicode.h"



struct ustr_codevalue_index
{
  ssize_t cv;
};

typedef struct ustr_codevalue_index ustr_cv_index_t;

struct ustr_codepoint_index
{
  ssize_t cp;
};

typedef struct ustr_codepoint_index ustr_cp_index_t;

enum ustr_length_type
{
  ustr_null_terminated,
  ustr_coding_value_length,
  ustr_codepoint_length
};

enum ustr_set_type
{
  ustr_over,
  ustr_fit,
  ustr_shift
};



/* automatically generated __STDC__ prototypes */
extern ustr_cv_index_t ustr_cv_index (ssize_t n);
extern ustr_cp_index_t ustr_cp_index (ssize_t n);
extern size_t ustr_cv_index_to_scanner_index (enum uni_encoding_scheme enc,
                                              ustr_cv_index_t n);
extern ustr_cv_index_t ustr_scanner_index_to_cv_index (enum uni_encoding_scheme enc,
                                                       size_t n);
extern uni_string ustr_cv_offset (uni_string str,
                                  enum uni_encoding_scheme enc,
                                  ustr_cv_index_t n);
extern uni_string ustr_cv_offset_n (ustr_cv_index_t * new_length,
                                    uni_string str,
                                    enum uni_encoding_scheme enc,
                                    ustr_cv_index_t len,
                                    ustr_cv_index_t n);
extern t_unicode ustr_scan (uni_string * str_after,
                            uni_string str,
                            enum uni_encoding_scheme enc);
extern ustr_cv_index_t ustr_str_subtract (uni_string a,
                                          uni_string b,
                                          enum uni_encoding_scheme enc);
extern t_unicode ustr_scan_n (uni_string * str_after,
                              ustr_cv_index_t * len_after,
                              uni_string str,
                              enum uni_encoding_scheme enc,
                              ustr_cv_index_t length);
extern int ustr_put (uni_string * str_after,
                     uni_string str,
                     enum uni_encoding_scheme enc,
                     t_unicode c,
                     enum ustr_set_type set_type);
extern int ustr_put_n (uni_string * str_after,
                       ustr_cv_index_t * len_after,
                       uni_string str,
                       enum uni_encoding_scheme enc,
                       ustr_cv_index_t len,
                       t_unicode c,
                       enum ustr_set_type set_type);
extern t_unicode ustr_cv_ref (ustr_cv_index_t * index_after,
                              uni_string str,
                              enum uni_encoding_scheme enc,
                              ustr_cv_index_t index);
extern t_unicode ustr_cv_ref_n (ustr_cv_index_t * index_after,
                                uni_string str,
                                enum uni_encoding_scheme enc,
                                ustr_cv_index_t len,
                                ustr_cv_index_t index);
extern int ustr_cv_set (ustr_cv_index_t * index_after,
                        uni_string str,
                        enum uni_encoding_scheme enc,
                        ustr_cv_index_t index,
                        t_unicode c,
                        enum ustr_set_type set_type);
extern int ustr_cv_set_n (ustr_cv_index_t * index_after,
                          ustr_cv_index_t * len_after,
                          uni_string str,
                          enum uni_encoding_scheme enc,
                          ustr_cv_index_t len,
                          ustr_cv_index_t index,
                          t_unicode c,
                          enum ustr_set_type set_type);
extern uni_string ustr_cp_offset (uni_string str,
                                  enum uni_encoding_scheme enc,
                                  ustr_cp_index_t n);
extern uni_string ustr_cp_offset_n (uni_string str,
                                    enum uni_encoding_scheme enc,
                                    ustr_cv_index_t len,
                                    ustr_cp_index_t n);
extern ustr_cv_index_t ustr_lengths (ustr_cp_index_t * cp_length,
                                     enum uni_encoding_scheme * fw_enc,
                                     uni_string str,
                                     enum uni_encoding_scheme enc);
extern ustr_cv_index_t ustr_lengths_n (ustr_cp_index_t * cp_length,
                                       enum uni_encoding_scheme * fw_enc,
                                       uni_string str,
                                       enum uni_encoding_scheme enc,
                                       ustr_cv_index_t len);
extern ustr_cv_index_t ustr_cv_length (uni_string str,
                                       enum uni_encoding_scheme enc);
extern ustr_cv_index_t ustr_cv_length_in_encoding (enum uni_encoding_scheme dest_enc,
                                                   uni_string str,
                                                   enum uni_encoding_scheme enc);
extern ustr_cv_index_t ustr_cv_length_in_encoding_n (enum uni_encoding_scheme dest_enc,
                                                     uni_string str,
                                                     enum uni_encoding_scheme enc,
                                                     ustr_cv_index_t len);
extern ustr_cp_index_t ustr_cp_length (enum uni_encoding_scheme * fw_enc,
                                       uni_string str,
                                       enum uni_encoding_scheme enc);
extern ustr_cp_index_t ustr_cp_length_n (enum uni_encoding_scheme * fw_enc,
                                         uni_string str,
                                         enum uni_encoding_scheme enc,
                                         ustr_cv_index_t len);
extern uni_string ustr_copy (uni_string dest,
                             enum uni_encoding_scheme dest_enc,
                             uni_string str,
                             enum uni_encoding_scheme enc);
extern uni_string ustr_copy_n (uni_string dest,
                               enum uni_encoding_scheme dest_enc,
                               ustr_cv_index_t dest_len,
                               uni_string str,
                               enum uni_encoding_scheme enc,
                               ustr_cv_index_t len);
extern uni_string ustr_cat (uni_string dest,
                            enum uni_encoding_scheme dest_enc,
                            uni_string str,
                            enum uni_encoding_scheme enc);
extern uni_string ustr_cat_n (uni_string dest,
                              enum uni_encoding_scheme dest_enc,
                              ustr_cv_index_t dest_len,
                              uni_string str,
                              ustr_cv_index_t len,
                              enum uni_encoding_scheme enc);
extern uni_string ustr_save (alloc_limits limits,
                             enum uni_encoding_scheme dest_enc,
                             uni_string str,
                             enum uni_encoding_scheme enc);
extern uni_string ustr_save_n_case (ustr_cv_index_t * dest_len,
                                    alloc_limits limits,
                                    enum uni_encoding_scheme dest_enc,
                                    uni_string str,
                                    enum uni_encoding_scheme enc,
                                    ustr_cv_index_t len);
extern uni_string ustr_save_n (ustr_cv_index_t * dest_len,
                               alloc_limits limits,
                               enum uni_encoding_scheme dest_enc,
                               uni_string str,
                               enum uni_encoding_scheme enc,
                               ustr_cv_index_t len);
#endif  /* INCLUDE__CHAR__USTR_H */


/* tag: Tom Lord Thu Jan  1 13:54:58 2004 (ustr.h)
 */
