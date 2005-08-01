/* edit-ops.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__BUFFERS__EDIT_OPS_H
#define INCLUDE__BUFFERS__EDIT_OPS_H



/* automatically generated __STDC__ prototypes */
extern int bufpt_insert_str_n (t_buffer_point * pt,
                               t_uchar * str,
                               ssize_t len);
extern int bufpt_backward_delete_n (t_buffer_point * pt_point,
                                    ssize_t n);
extern int bufpt_is_eol (t_buffer_point * m,
                         t_buffer_point * end);
extern int bufpt_is_at_end (t_buffer_point * m,
                            t_buffer_point * end);
extern int bufpt_char_at (t_buffer_point * m,
                          t_buffer_point * end);
extern int bufpt_looking_at_str_n (t_buffer_point * m,
                                   t_buffer_point * end,
                                   const t_uchar * str,
                                   ssize_t len);
extern int bufpt_looking_at_blank_line (t_buffer_point * m,
                                        t_buffer_point * end);
extern int bufpt_looking_at_ci_str_n (t_buffer_point * m,
                                      t_buffer_point * end,
                                      t_uchar * str,
                                      ssize_t len);
extern int bufpt_looking_at_re (t_buffer_point * m,
                                t_buffer_point * end_pt,
                                regex_t * preg,
                                int eflags);
extern ssize_t bufpt_col (t_buffer_point * m);
extern ssize_t bufpt_col_tabifying (t_buffer_point * m);
extern int bufpt_forward_char (t_buffer_point * m,
                               t_buffer_point * end,
                               ssize_t n);
extern int bufpt_backward_char (t_buffer_point * m,
                                t_buffer_point * end,
                                ssize_t n);
extern int bufpt_beginning_of_line (t_buffer_point * m,
                                    t_buffer_point * end);
extern int bufpt_next_line (t_buffer_point * m,
                            t_buffer_point * end);
extern int bufpt_end_of_line (t_buffer_point * m,
                              t_buffer_point * end);
extern int bufpt_pass_whitespace (t_buffer_point * m,
                                  t_buffer_point * end);
extern int bufpt_pass_graphical (t_buffer_point * m,
                                 t_buffer_point * end);
extern int bufpt_pass_blanks (t_buffer_point * m,
                              t_buffer_point * end);
extern int bufpt_pass_blank_lines (t_buffer_point * m,
                                   t_buffer_point * end);
#endif  /* INCLUDE__BUFFERS__EDIT_OPS_H */


/* tag: Tom Lord Sun Nov 21 10:46:25 2004 (edit-ops.h)
 */
