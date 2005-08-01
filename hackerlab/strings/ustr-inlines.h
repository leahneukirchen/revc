/* ustr-inlines.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__STRINGS__USTR_INLINES_H
#define INCLUDE__STRINGS__USTR_INLINES_H


#include "hackerlab/strings/ustr.h"


/* automatically generated __STDC__ prototypes */
extern inline__ ustr_cv_index_t ustr_cv_index (ssize_t n);
extern inline__ ustr_cp_index_t ustri_cp_index (ssize_t n);
extern inline__ size_t ustri_cv_index_to_scanner_index (enum uni_encoding_scheme enc,
                                                        ustr_cv_index_t n);
extern inline__ ustr_cv_index_t ustri_scanner_index_to_cv_index (enum uni_encoding_scheme enc,
                                                                 size_t n);
extern inline__ uni_string ustri_cv_offset (uni_string str,
                                            enum uni_encoding_scheme enc,
                                            ustr_cv_index_t n);
extern inline__ uni_string ustri_cv_offset_n (ustr_cv_index_t * new_length,
                                              uni_string str,
                                              enum uni_encoding_scheme enc,
                                              ustr_cv_index_t len,
                                              ustr_cv_index_t n);
extern inline__ t_unicode ustri_scan (uni_string * str_after,
                                      uni_string str,
                                      enum uni_encoding_scheme enc);
extern ustr_cv_index_t ustr_str_subtract (uni_string a,
                                          uni_string b,
                                          enum uni_encoding_scheme enc);
extern inline__ t_unicode ustri_scan_n (uni_string * str_after,
                                        ustr_cv_index_t * len_after,
                                        uni_string str,
                                        enum uni_encoding_scheme enc,
                                        ustr_cv_index_t length);
extern inline__ int ustri_put (uni_string * str_after,
                               uni_string str,
                               enum uni_encoding_scheme enc,
                               t_unicode c,
                               enum ustr_set_type set_type);
extern inline__ int ustri_put_n (uni_string * str_after,
                                 ustr_cv_index_t * len_after,
                                 uni_string str,
                                 enum uni_encoding_scheme enc,
                                 ustr_cv_index_t len,
                                 t_unicode c,
                                 enum ustr_set_type set_type);
extern inline__ t_unicode ustri_cv_ref (ustr_cv_index_t * index_after,
                                        uni_string str,
                                        enum uni_encoding_scheme enc,
                                        ustr_cv_index_t index);
extern inline__ t_unicode ustri_cv_ref_n (ustr_cv_index_t * index_after,
                                          uni_string str,
                                          enum uni_encoding_scheme enc,
                                          ustr_cv_index_t len,
                                          ustr_cv_index_t index);
extern inline__ int ustri_cv_set (ustr_cv_index_t * index_after,
                                  uni_string str,
                                  enum uni_encoding_scheme enc,
                                  ustr_cv_index_t index,
                                  t_unicode c,
                                  enum ustr_set_type set_type);
extern inline__ int ustri_cv_set_n (ustr_cv_index_t * index_after,
                                    ustr_cv_index_t * len_after,
                                    uni_string str,
                                    enum uni_encoding_scheme enc,
                                    ustr_cv_index_t len,
                                    ustr_cv_index_t index,
                                    t_unicode c,
                                    enum ustr_set_type set_type);
extern inline__ uni_string ustr_cp_offset_case (uni_string str,
                                                enum uni_encoding_scheme enc,
                                                ustr_cp_index_t n);
extern inline__ uni_string ustr_cp_offset (uni_string str,
                                           enum uni_encoding_scheme enc,
                                           ustr_cp_index_t n);
extern inline__ uni_string ustr_cp_offset_n_case (uni_string str,
                                                  enum uni_encoding_scheme enc,
                                                  ustr_cv_index_t len,
                                                  ustr_cp_index_t n);
extern inline__ uni_string ustr_cp_offset_n (uni_string str,
                                             enum uni_encoding_scheme enc,
                                             ustr_cv_index_t len,
                                             ustr_cp_index_t n);
extern inline__ ustr_cv_index_t ustri_cv_length (uni_string str,
                                                 enum uni_encoding_scheme enc);
extern inline__ ustr_cv_index_t ustri_cv_length_in_encoding (enum uni_encoding_scheme dest_enc,
                                                             uni_string str,
                                                             enum uni_encoding_scheme enc);
extern inline__ ustr_cv_index_t ustri_cv_length_in_encoding_n (enum uni_encoding_scheme dest_enc,
                                                               uni_string str,
                                                               enum uni_encoding_scheme enc,
                                                               ustr_cv_index_t len);
extern inline__ ustr_cp_index_t ustri_cp_length (enum uni_encoding_scheme * fw_enc,
                                                 uni_string str,
                                                 enum uni_encoding_scheme enc);
extern inline__ ustr_cp_index_t ustri_cp_length_n (enum uni_encoding_scheme * fw_enc,
                                                   uni_string str,
                                                   enum uni_encoding_scheme enc,
                                                   ustr_cv_index_t len);
extern inline__ uni_string ustri_copy (uni_string dest,
                                       enum uni_encoding_scheme dest_enc,
                                       uni_string str,
                                       enum uni_encoding_scheme enc);
extern inline__ uni_string ustri_copy_n (uni_string dest,
                                         enum uni_encoding_scheme dest_enc,
                                         ustr_cv_index_t dest_len,
                                         uni_string str,
                                         enum uni_encoding_scheme enc,
                                         ustr_cv_index_t len);
extern inline__ uni_string ustri_cat (uni_string dest,
                                      enum uni_encoding_scheme dest_enc,
                                      uni_string str,
                                      enum uni_encoding_scheme enc);
extern inline__ uni_string ustri_cat_n (uni_string dest,
                                        enum uni_encoding_scheme dest_enc,
                                        ustr_cv_index_t dest_len,
                                        uni_string str,
                                        ustr_cv_index_t len,
                                        enum uni_encoding_scheme enc);
extern inline__ uni_string ustri_save (alloc_limits limits,
                                       enum uni_encoding_scheme dest_enc,
                                       uni_string str,
                                       enum uni_encoding_scheme enc);
extern inline__ uni_string ustri_save_n_case (ustr_cv_index_t * dest_len,
                                              alloc_limits limits,
                                              enum uni_encoding_scheme dest_enc,
                                              uni_string str,
                                              enum uni_encoding_scheme enc,
                                              ustr_cv_index_t len);
extern inline__ uni_string ustri_save_n (ustr_cv_index_t * dest_len,
                                         alloc_limits limits,
                                         enum uni_encoding_scheme dest_enc,
                                         uni_string str,
                                         enum uni_encoding_scheme enc,
                                         ustr_cv_index_t len);
#endif  /* INCLUDE__STRINGS__USTR_INLINES_H */


/* tag: Tom Lord Wed May 19 09:42:28 2004 (ustr-inlines.h)
 */
