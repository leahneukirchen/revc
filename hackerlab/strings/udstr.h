/* udstr.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__STRINGS__UDSTR_H
#define INCLUDE__STRINGS__UDSTR_H


#include "hackerlab/strings/ustr.h"




struct udstr_handle;
typedef struct udstr_handle * t_udstr;



/* automatically generated __STDC__ prototypes */
extern t_udstr udstr_save (alloc_limits limits,
                           uni_string str,
                           enum uni_encoding_scheme enc);
extern t_udstr udstr_save_n (alloc_limits limits,
                             uni_string str,
                             enum uni_encoding_scheme enc,
                             ustr_cv_index_t len);
extern t_udstr udstr_save_fw (alloc_limits limits,
                              uni_string str,
                              enum uni_encoding_scheme enc);
extern t_udstr udstr_save_fw_n (alloc_limits limits,
                                uni_string str,
                                enum uni_encoding_scheme enc,
                                ustr_cv_index_t len);
extern t_udstr udstr_save_generic (alloc_limits limits,
                                   enum uni_encoding_scheme dest_enc,
                                   ustr_cv_index_t dest_len,
                                   uni_string str,
                                   enum uni_encoding_scheme enc,
                                   ustr_cv_index_t len,
                                   ustr_cp_index_t cp_len);
extern void udstr_ref (t_udstr d);
extern void udstr_unref (t_udstr d);
extern ustr_cv_index_t udstr_cv_length (t_udstr dstr);
extern ustr_cp_index_t udstr_cp_length (t_udstr dstr);
extern enum uni_encoding_scheme udstr_encoding (t_udstr dstr);
extern uni_string udstr_str (t_udstr dstr);
extern t_unicode udstr_cv_ref (ustr_cv_index_t * pos_after,
                               t_udstr dstr,
                               ustr_cv_index_t pos);
extern t_unicode udstr_cp_ref (ustr_cv_index_t * pos_after,
                               t_udstr dstr,
                               ustr_cp_index_t pos);
extern t_udstr udstr_cv_set (alloc_limits limits,
                             t_udstr s,
                             ustr_cv_index_t x,
                             t_unicode c);
extern t_udstr udstr_cv_set_fw (alloc_limits limits,
                                t_udstr s,
                                ustr_cv_index_t x,
                                t_unicode c);
extern t_udstr udstr_cp_set (alloc_limits limits,
                             t_udstr s,
                             ustr_cp_index_t x,
                             t_unicode c);
extern t_udstr udstr_cp_set_fw (alloc_limits limits,
                                t_udstr s,
                                ustr_cp_index_t x,
                                t_unicode c);
extern t_udstr udstr_fw_x (t_udstr d);
extern t_udstr udstr_copy (alloc_limits limits, t_udstr dstr);
extern t_udstr udstr_copy_fw (alloc_limits limits, t_udstr dstr);
extern t_udstr udstr_cv_substr (alloc_limits limits,
                                t_udstr dstr,
                                ustr_cv_index_t from,
                                ustr_cv_index_t to);
extern t_udstr udstr_cv_substr_x (t_udstr dstr,
                                  ustr_cv_index_t from,
                                  ustr_cv_index_t to);
extern t_udstr udstr_cv_substr_fw (alloc_limits limits,
                                   t_udstr dstr,
                                   ustr_cv_index_t from,
                                   ustr_cv_index_t to);
extern t_udstr udstr_cv_substr_fw_x (t_udstr dstr,
                                     ustr_cv_index_t from,
                                     ustr_cv_index_t to);
extern t_udstr udstr_cp_substr (alloc_limits limits,
                                t_udstr dstr,
                                ustr_cp_index_t from,
                                ustr_cp_index_t to);
extern t_udstr udstr_cp_substr_x (t_udstr dstr,
                                  ustr_cp_index_t from,
                                  ustr_cp_index_t to);
extern t_udstr udstr_cp_substr_fw (alloc_limits limits,
                                   t_udstr dstr,
                                   ustr_cp_index_t from,
                                   ustr_cp_index_t to);
extern t_udstr udstr_cp_substr_fw_x (t_udstr dstr,
                                     ustr_cp_index_t from,
                                     ustr_cp_index_t to);
extern t_udstr udstr_append (alloc_limits limits,
                             t_udstr a_dstr,
                             t_udstr b_dstr);
extern t_udstr udstr_append_x (t_udstr a_dstr,
                               t_udstr b_dstr);
extern t_udstr udstr_append_fw (alloc_limits limits,
                                t_udstr a_dstr,
                                t_udstr b_dstr);
extern t_udstr udstr_append_fw_x (t_udstr a_dstr,
                                  t_udstr b_dstr);
extern t_udstr udstr_cv_delete (alloc_limits limits,
                                t_udstr d,
                                ustr_cv_index_t from,
                                ustr_cv_index_t to);
extern t_udstr udstr_cp_delete (alloc_limits limits,
                                t_udstr d,
                                ustr_cp_index_t from,
                                ustr_cp_index_t to);
extern t_udstr udstr_cv_delete_x (t_udstr d,
                                  ustr_cv_index_t from,
                                  ustr_cv_index_t to);
extern t_udstr udstr_cp_delete_x (t_udstr d,
                                  ustr_cp_index_t from,
                                  ustr_cp_index_t to);
extern t_udstr udstr_cv_delete_fw (alloc_limits limits,
                                   t_udstr d,
                                   ustr_cv_index_t from,
                                   ustr_cv_index_t to);
extern t_udstr udstr_cp_delete_fw (alloc_limits limits,
                                   t_udstr d,
                                   ustr_cp_index_t from,
                                   ustr_cp_index_t to);
extern t_udstr udstr_cv_delete_fw_x (t_udstr d,
                                     ustr_cv_index_t from,
                                     ustr_cv_index_t to);
extern t_udstr udstr_cp_delete_fw_x (t_udstr d,
                                     ustr_cp_index_t from,
                                     ustr_cp_index_t to);
extern t_udstr udstr_cv_replace (alloc_limits limits,
                                 t_udstr into,
                                 ustr_cv_index_t i_from,
                                 ustr_cv_index_t i_to,
                                 t_udstr from,
                                 ustr_cv_index_t f_from,
                                 ustr_cv_index_t f_to);
extern t_udstr udstr_cp_replace (alloc_limits limits,
                                 t_udstr into,
                                 ustr_cp_index_t i_from,
                                 ustr_cp_index_t i_to,
                                 t_udstr from,
                                 ustr_cp_index_t f_from,
                                 ustr_cp_index_t f_to);
extern t_udstr udstr_cv_replace_x (t_udstr into,
                                   ustr_cv_index_t i_from,
                                   ustr_cv_index_t i_to,
                                   t_udstr from,
                                   ustr_cv_index_t f_from,
                                   ustr_cv_index_t f_to);
extern t_udstr udstr_cp_replace_x (t_udstr into,
                                   ustr_cp_index_t i_from,
                                   ustr_cp_index_t i_to,
                                   t_udstr from,
                                   ustr_cp_index_t f_from,
                                   ustr_cp_index_t f_to);
extern t_udstr udstr_cv_replace_fw (alloc_limits limits,
                                    t_udstr into,
                                    ustr_cv_index_t i_from,
                                    ustr_cv_index_t i_to,
                                    t_udstr from,
                                    ustr_cv_index_t f_from,
                                    ustr_cv_index_t f_to);
extern t_udstr udstr_cp_replace_fw (alloc_limits limits,
                                    t_udstr into,
                                    ustr_cp_index_t i_from,
                                    ustr_cp_index_t i_to,
                                    t_udstr from,
                                    ustr_cp_index_t f_from,
                                    ustr_cp_index_t f_to);
extern t_udstr udstr_cv_replace_fw_x (t_udstr into,
                                      ustr_cv_index_t i_from,
                                      ustr_cv_index_t i_to,
                                      t_udstr from,
                                      ustr_cv_index_t f_from,
                                      ustr_cv_index_t f_to);
extern t_udstr udstr_cp_replace_fw_x (t_udstr into,
                                      ustr_cp_index_t i_from,
                                      ustr_cp_index_t i_to,
                                      t_udstr from,
                                      ustr_cp_index_t f_from,
                                      ustr_cp_index_t f_to);
extern ustr_cv_index_t udstr_cv_normalize (t_udstr dstr,
                                           ustr_cv_index_t orig_index);
extern ustr_cv_index_t udstr_cv_inc (t_udstr dstr,
                                     ustr_cv_index_t orig_index);
extern ustr_cv_index_t udstr_cv_dec (t_udstr dstr,
                                     ustr_cv_index_t orig_index);
extern ustr_cv_index_t udstr_cp_to_cv (t_udstr dstr,
                                       ustr_cp_index_t cpi);
extern void udstr_cp_to_cv_range (ustr_cv_index_t * from_v,
                                  ustr_cv_index_t * to_v,
                                  t_udstr d,
                                  ustr_cp_index_t from,
                                  ustr_cp_index_t to);
#endif  /* INCLUDE__STRINGS__UDSTR_H */


/* tag: Tom Lord Fri Jan  2 08:46:35 2004 (udstr.h)
 */
