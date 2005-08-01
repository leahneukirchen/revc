/* qlinks.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/mem/mem.h"
#include "hackerlab/qlinks/qlinks.h"



void
init_qlinks (t_qlinks * links)
{
  if (links)
    {
      links->_next = links;
      links->_prev = links;
    }
}

void
uninit_qlinks (t_qlinks * links)
{
  if (links)
    {
      mem_set0 ((t_uchar *)links, sizeof (*links));
    }
}


void
qlinks_prepend (t_qlinks * head, t_qlinks * this)
{
  if (head && this)
    {
      this->_prev = head->_prev;
      this->_next = head;
      head->_prev = this;
      this->_prev->_next = this;
    }
}


void *
qlinks_next (t_qlinks * q)
{
  if (!q || (q == q->_next))
    return 0;

  return (void *)q->_next;
}


void
qlinks_unlink (t_qlinks * q)
{
  if (q)
    {
      q->_next->_prev = q->_prev;
      q->_prev->_next = q->_next;
      q->_prev = q;
      q->_next = q;
    }
}



/* tag: Tom Lord Tue Oct 26 14:20:01 2004 (qlinks.c)
 */
