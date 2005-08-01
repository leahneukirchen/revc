/* cvt_opt.c: cvt_opt implementation
 *
 ****************************************************************
 * Copyright (C) 2004 by Jose Antonio Ortega Ruiz
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/tests/uni-tests/cvt_opt.h"



static t_uchar * program_name = "test program";
static t_uchar * usage = "[options] < input-file";
static t_uchar * version_string = "1.0";

#define OPTS(OP, OP2) \
  OP (opt_help_msg, "h", "help", 0, \
      "Display a help message and exit.") \
  OP (opt_version, "V", "version", 0, \
      "Display a release identifier string") \
  OP2 (opt_version, 0, 0, 0, "and exit.") \
  OP (opt_non_native, "n", "non_native", 0, \
      "Use non-native byte order.") \
  OP (opt_cvt_8_16, 0, "8-to-16", 0, \
      "Convert UTF-8 to UTF-16.") \
  OP (opt_cvt_8_32, 0, "8-to-32", 0, \
      "Convert UTF-8 to UTF-32.") \
  OP (opt_cvt_16_8, 0, "16-to-8", 0, \
      "Convert UTF-16 to UTF-8.") \
  OP (opt_cvt_16_32, 0, "16-to-32", 0, \
      "Convert UTF-16 to UTF-32.") \
  OP (opt_cvt_32_8, 0, "32-to-8", 0, \
      "Convert UTF-32 to UTF-8.") \
  OP (opt_cvt_32_16, 0, "32-to-16", 0, \
      "Convert UTF-32 to UTF-16.")

enum options
  {
    OPTS (OPT_ENUM, OPT_IGN)
  };

struct opt_desc opts[] =
  {
    OPTS (OPT_DESC, OPT_DESC)
    {-1, 0, 0, 0, 0}
  };

void
cvt_option_setup (t_uchar * pn,
                  t_uchar * usg,
                  t_uchar * ver)
{
  if (pn) program_name = pn;
  if (usg) usage = usg;
  if (ver) version_string = ver;
}

conversion_t
cvt_option (int argc, char * argv[], int * non_native)
{
  conversion_t fromto;
  int o;
  struct opt_parsed * option;

  fromto = -1;
  option = 0;
  if (non_native) *non_native = 0;

  while (1)
    {
      o = opt_standard (lim_use_must_malloc, &option, opts, &argc, argv, program_name, usage, version_string, 0, opt_help_msg, opt_none, opt_version);
      if (o == opt_none)
	break;
      switch (o)
	{
	default:
	  safe_printfmt (2, "unhandled option `%s'\n", option->opt_string);
	  panic ("internal error parsing arguments");

	usage_error:
	  opt_usage (2, argv[0], program_name, usage, 1);
	  panic_exit ();

#if 0
	bogus_arg:
	  safe_printfmt (2, "ill-formed argument for `%s' (`%s')\n", option->opt_string, option->arg_string);
	  goto usage_error;
#endif
        case opt_non_native:
          if (non_native) *non_native = 1;
          break;

	case opt_cvt_8_16:
          fromto =  cvt_8_16;
          break;

        case opt_cvt_8_32:
          fromto =  cvt_8_32;
          break;

        case opt_cvt_16_8:
          fromto =  cvt_16_8;
          break;

        case opt_cvt_16_32:
          fromto =  cvt_16_32;
          break;

        case opt_cvt_32_8:
          fromto =  cvt_32_8;
          break;

        case opt_cvt_32_16:
          fromto =  cvt_32_16;
          break;

	}
    }

  if ((argc != 1) || (fromto == -1))
    goto usage_error;

  return fromto;
}

  
/* arch-tag: Jose Antonio Ortega Ruiz Sun Feb 29 2004 21:13:10 (cvt_opt.c)
*/
