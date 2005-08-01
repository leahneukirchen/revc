/* constants.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBREVC__CONSTANTS_H
#define INCLUDE__LIBREVC__CONSTANTS_H


#define REVC_FAKED			0

#define REVC_MAX_FNAME_SIZE		256
#define REVC_MAX_BLOB_ADDR_SIZE		128
#define REVC_MAX_METADATA_SIZE		128
#define REVC_MAX_REVNAME_SIZE		128

#define REVC_DIR_ENTRY_SIZE		(  REVC_MAX_FNAME_SIZE \
                                         + REVC_MAX_BLOB_ADDR_SIZE \
                                         + REVC_MAX_METADATA_SIZE)

#define REVC_DIR_ENTRY_FNAME(E)		(E)
#define REVC_DIR_ENTRY_BLOB_ADDR(E)	((E) + REVC_MAX_FNAME_SIZE)
#define REVC_DIR_ENTRY_METADATA(E)	((E) + REVC_MAX_FNAME_SIZE + REVC_MAX_BLOB_ADDR_SIZE)


#define REVC_INITIAL_MANIFEST		(".\0")

#define REVC_NIL_REVNAME		("nil")

#define REVC_EMPTY_FILE_REVNAME		("empty")

#define REVC_DEFAULT_DIR_METADATA	("d---rwxrwxrwx.500.500")
#define REVC_DEFAULT_FILE_METADATA	("d---rw-rw-rw-.500.500")

#define REVC_MAX_SIMULTANEOUS_COMMITS   (256)
#define REVC_MAX_SIMULTANEOUS_GETS      (256)

#define REVC_BLOB_HINT_SET_BITS		(2048) /* do not lower without changing blob-hint.c */
#define REVC_BLOB_HINT_SET_SIZE		(REVC_BLOB_HINT_SET_BITS >> 3)




/* automatically generated __STDC__ prototypes */
#endif  /* INCLUDE__LIBREVC__CONSTANTS_H */


/* arch-tag: Tom Lord Sun Jun 19 10:54:36 2005 (constants.h)
 */
