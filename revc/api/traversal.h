/* traversal.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__API__TRAVERSAL_H
#define INCLUDE__API__TRAVERSAL_H


#include "hackerlab/machine/types.h"
#include "revc/api/constants.h"

struct revc_traversal
{
  int eot;
  t_uchar * node;
  t_uchar blob[REVC_MAX_BLOB_ADDR_SIZE + 1];
  t_uchar meta[REVC_MAX_BLOB_ADDR_SIZE + 1];

  int details_known;

  int (*next) (const t_uchar * const errname,
               struct revc_traversal * trav,
               int skip_subtree);
  int (*end) (const t_uchar * const errname,
              struct revc_traversal ** trav);
};


#define revc_traversal_eot(T) ((T)->eot)
#define revc_traversal_node(T) ((T)->node)
#define revc_traversal_next(E,T,S) ((T)->next ((E), (T), (S)))
#define revc_traversal_end(E,T,S) ((T)->next ((E), (T)))


/* automatically generated __STDC__ prototypes */
#endif  /* INCLUDE__API__TRAVERSAL_H */


/* arch-tag: Tom Lord Sat Jul  2 18:16:09 2005 (traversal.h)
 */
