/* qlinks.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__QLINKS_H
#define INCLUDE__LIBAWIKI__QLINKS_H


typedef struct qlinks t_qlinks;


struct qlinks
{
  struct qlinks * _next;
  struct qlinks * _prev;
};


/* automatically generated __STDC__ prototypes */
extern void init_qlinks (t_qlinks * links);
extern void uninit_qlinks (t_qlinks * links);
extern void qlinks_prepend (t_qlinks * head, t_qlinks * this);
extern void * qlinks_next (t_qlinks * q);
extern void qlinks_unlink (t_qlinks * q);
#endif  /* INCLUDE__LIBAWIKI__QLINKS_H */


/* tag: Tom Lord Tue Oct 26 14:19:32 2004 (qlinks.h)
 */
