/* opts.c: 
 *
 ****************************************************************
 * Copyright (C) 2005 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "revc/deps/error.h"
#include "revc/deps/usage.h"
#include "revc/api/opts.h"



int
revc_opts (t_uchar ** const errname,
           int * const argx_r,
           struct revc_opts * opts,
           int const argc,
           char ** const argv,
           const t_uchar * const usage_string)
{
  int argx = 0;

  if (argc >= 0)
    {
      *errname = str_save (0, argv[0]);
      argx = 1;
    }
  else
    {
      *errname = str_save (0, "revc");
      argx = 0;
    }

  while ((argx < argc) && (argv[argx][0] == '-'))
    {
      int o;
      int matched;

      matched = 0;

      for (o = 0; opts[o].opt_type != REVC_END_OPTS; ++o)
        {
          if (   (opts[o].short_name && !str_cmp (argv[argx], opts[o].short_name))
              || (opts[o].long_name && !str_cmp (argv[argx], opts[o].long_name)))
            {
              matched = 1;
              switch (opts[o].opt_type)
                {
                case REVC_FLAG_OPT:
                  {
                    *opts[o].flag_slot = 1;
                    ++argx;
                    break;
                  }
                case REVC_ARG_OPT:
                  {
                    if ((argx + 1) == argc)
                      goto usage;
                    *opts[o].arg_slot = str_save (0, argv[argx + 1]);
                    argx += 2;
                    break;
                  }
                default:
                  {
                    revc_error (*errname, "internal error parsing arguments\n");
                    return -1;
                  }
                }
            }
        }

      if (!matched)
        {
          if (!str_cmp (argv[argx], "--"))
            break;
          else 
            {
            usage:
              revc_usage (*errname, usage_string);
              return -1;
            }
        }
    }

  *argx_r = argx;
  return 0;
}






/* arch-tag: Tom Lord Thu Jun 23 10:18:14 2005 (opts.c)
 */
