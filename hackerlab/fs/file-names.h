/* file-names.h - decls for file-name manipulations
 *
 ****************************************************************
 * Copyright (C) 1998, 2000 Thomas Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#ifndef INCLUDE__FS__FILE_NAMES_H
#define INCLUDE__FS__FILE_NAMES_H



#include "hackerlab/machine/types.h"
#include "hackerlab/mem/alloc-limits.h"


/* automatically generated __STDC__ prototypes */
extern t_uchar * file_name_home_directory (void);
extern int file_name_is_absolute (const t_uchar * f);
extern t_uchar * file_name_tilde_expand (alloc_limits limits, const t_uchar * fname);
extern t_uchar * file_name_tail (alloc_limits limits, const t_uchar * fname);
extern t_uchar * file_name_head (alloc_limits limits, const char * fname);
extern t_uchar * file_name_sans_head (alloc_limits limits, const char * fname);
extern t_uchar * file_name_as_directory (alloc_limits limits, const t_uchar * f);
extern t_uchar * file_name_from_directory (alloc_limits limits, const t_uchar * f);
extern t_uchar * file_name_directory (alloc_limits limits, const t_uchar * f);
extern t_uchar * file_name_directory_file (alloc_limits limits, const t_uchar * f);
extern int file_name_is_path_prefix (const t_uchar * maybe_prefix, const t_uchar * path);
extern int file_name_is_dir_prefix (const t_uchar * maybe_prefix, const t_uchar * path);
extern t_uchar * file_name_in_vicinity (alloc_limits limits,
                                        const t_uchar * dir,
                                        const t_uchar * name);
extern t_uchar ** path_parse (alloc_limits limits, const t_uchar * path);
extern void free_path (alloc_limits limits, t_uchar ** path);
extern t_uchar * path_find_executable (alloc_limits limits,
                                       const t_uchar ** path,
                                       const t_uchar * tail);
#endif  /* INCLUDE__FS__FILE_NAMES_H */
