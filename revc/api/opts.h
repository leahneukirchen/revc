/* opts.h:
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBREVC__OPTS_H
#define INCLUDE__LIBREVC__OPTS_H


#include "hackerlab/machine/types.h"

#define REVC_END_OPTS 0
#define REVC_FLAG_OPT 1
#define REVC_ARG_OPT  2


struct revc_opts
{
  int opt_type;
  const t_uchar * short_name;
  const t_uchar * long_name;
  int * flag_slot;
  t_uchar ** arg_slot;
};


/* automatically generated __STDC__ prototypes */
extern int revc_opts (t_uchar ** const errname,
                      int * const argx_r,
                      struct revc_opts * opts,
                      int const argc,
                      char ** const argv,
                      const t_uchar * const usage_string);
#endif  /* INCLUDE__LIBREVC__OPTS_H */


/* arch-tag: Tom Lord Thu Jun 23 09:42:01 2005 (opts.h)
 */
