/* safe.h - decls for error-free I/O routines (panic on error)
 *
 ****************************************************************
 * Copyright (C) 1999, 2000 Thomas Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#ifndef INCLUDE__VU__SAFE_H
#define INCLUDE__VU__SAFE_H

#include "hackerlab/os/sys/stat.h"
#include "hackerlab/os/fcntl.h"
#include "hackerlab/os/unistd.h"
#include "hackerlab/os/stdlib.h"
#include "hackerlab/os/sys/time.h"
#include "hackerlab/os/utime.h"
#include "hackerlab/os/dirent.h"
#include "hackerlab/machine/types.h"
#include "hackerlab/vu/safe-vu-utils.h"
#include "hackerlab/vu/safe-vfdbuf.h"
#include "hackerlab/vu/safe-printfmt.h"



/* automatically generated __STDC__ prototypes */
extern int safe_access (const char * path, int mode);
extern int safe_chdir (const char * path);
extern int safe_chmod (const char * path, int mode);
extern int safe_chown (const char * path, int owner, int group);
extern int safe_chroot (const char * path);
extern int safe_close (int fd);
extern int safe_closedir (DIR * dir);
extern int safe_fchdir (int fd);
extern int safe_fchmod (int fd, int mode);
extern int safe_fchown (int fd, int owner, int group);
extern int safe_fstat (int fd, struct stat * buf);
extern int safe_fsync (int fd);
extern int safe_ftruncate (int fd, long where);
extern int safe_link (const char * from, const char * to);
extern long safe_lseek (int fd, long offset, int whence);
extern int safe_lstat (const char * path, struct stat * buf);
extern int safe_mkdir (const char * path, int mode);
extern int safe_open (const char * path, int flags, int mode);
extern int safe_opendir (DIR ** retv, const char * path);
extern long safe_read (int fd, char * buf, long count);
extern long safe_read_retry (int fd, char * buf, long count);
extern int safe_readdir (char ** file_ret, DIR * dir);
extern int safe_readlink (const char * path, char * buf, int bufsize);
extern int safe_rename (const char * from, const char * to);
extern int safe_rmdir (const char * path);
extern int safe_stat (const char * path, struct stat * buf);
extern int safe_symlink (const char * from, const char * to);
extern int safe_truncate (const char * path, long where);
extern int safe_unlink (const char * path);
extern int safe_utime (const char * path, struct utimbuf * times);
extern long safe_write (int fd, const char * buf, long count);
extern long safe_write_retry (int fd, const t_uchar * buf, int amt);
extern int safe_fcntl (int fd, int cmd, long arg);
extern int safe_dup (int fd);
extern int safe_dup2 (int fd, int newfd);
extern int safe_move_state (int fd, int newfd);
#endif  /* INCLUDE__VU__SAFE_H */
