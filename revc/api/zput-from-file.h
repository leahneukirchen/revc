/* zput-from-file.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__ZPUT_FROM_FILE_H
#define INCLUDE__API__ZPUT_FROM_FILE_H


#include "hackerlab/machine/types.h"


/* automatically generated __STDC__ prototypes */
extern int revc_zput_from_file (const t_uchar * const errname,
                                size_t * file_size_ret,
                                size_t * zip_size_ret,
                                const t_uchar * const path_out,
                                const t_uchar * const path_in);
#endif  /* INCLUDE__API__ZPUT_FROM_FILE_H */


/* arch-tag: Tom Lord Sun Jul  3 09:54:10 2005 (api/zput-from-file.h)
 */
