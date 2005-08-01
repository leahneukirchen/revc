/* file-buffers.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__FILE_BUFFERS_H
#define INCLUDE__LIBAWIKI__FILE_BUFFERS_H


#include "hackerlab/buffers/buffer-set.h"


/* automatically generated __STDC__ prototypes */
extern t_uchar * buf_file_name (ssize_t bset, ssize_t buffer);
extern ssize_t buf_find_existing_file (int * errn,
                                       ssize_t bset,
                                       t_uchar * file);
extern ssize_t buf_find_file (int * errn,
                              ssize_t bset,
                              t_uchar * file);
extern int buf_insert_from_fd (int * errn,
                               ssize_t * amt_inserted_return,
                               ssize_t bset,
                               ssize_t buffer,
                               int fd,
                               ssize_t insert_pos,
                               ssize_t insert_amt);
#endif  /* INCLUDE__LIBAWIKI__FILE_BUFFERS_H */


/* tag: Tom Lord Wed Oct 27 19:35:09 2004 (file-buffers.h)
 */
