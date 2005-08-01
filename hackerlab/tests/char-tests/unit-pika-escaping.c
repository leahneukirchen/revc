/* unit-pika-escaping.c - test pika-escaping.c
 *
 ****************************************************************
 * Copyright (C) 2004, Christian Thaeter
 * Copyright (C) 2000, 2003 Thomas Lord
 * Copyright (C) 2002 Scott Parish
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include "hackerlab/bugs/panic.h"
#include "hackerlab/char/str.h"
#include "hackerlab/fmt/cvt.h"
#include "hackerlab/char/pika-escaping.h"
#include "hackerlab/char/pika-escaping-utils.h"
#include "hackerlab/vu/safe-printfmt.h"
#include "hackerlab/cmd/main.h"



static t_uchar * program_name = "unit-escapeing";
static t_uchar * usage = "[options]";
static t_uchar * version_string = "1.0";

#define OPTS(OP, OP2) \
  OP (opt_help_msg, "h", "help", 0, \
      "Display a help message and exit.") \
  OP (opt_version, "V", "version", 0, \
      "Display a release identifier string") \
  OP2 (opt_version, 0, 0, 0, "and exit.")

enum options
{
  OPTS (OPT_ENUM, OPT_IGN)
};

struct opt_desc opts[] =
{
  OPTS (OPT_DESC, OPT_DESC)
    {-1, 0, 0, 0, 0}
};




int
main (int argc, char * argv[])
{
  int o;
  struct opt_parsed * option;

  option = 0;

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
	}
    }
  {
    t_uchar   out[256];

    size_t result_needed;
    size_t str_used;

    t_uchar * test;
    t_uchar * test_back;


    invariant (pika_escape_iso8859_1_n (0, (size_t)0, 0, "foobar", (size_t)6) == 6);
    invariant (pika_escape_iso8859_1_n (0, (size_t)0, 0, "foo\tbar", (size_t)7) == 7);
    invariant (pika_escape_iso8859_1_n (0, (size_t)0, char_class_space, "foo\tbar", (size_t)7) == 12);
    invariant (pika_escape_iso8859_1_n (0, (size_t)0, char_class_control, "foo\tbar", (size_t)7) == 12);
    invariant (pika_escape_iso8859_1_n (0, (size_t)0, 0, "foo\020bar", (size_t)7) == 7);
    invariant (pika_escape_iso8859_1_n (0, (size_t)0, char_class_control, "foo\020bar", (size_t)7) == 13);
    invariant (pika_escape_iso8859_1_n (0, (size_t)0, char_class_space, "foo\020bar", (size_t)7) == 7);

    invariant ((pika_escape_iso8859_1_n (out, (size_t)255, 0, "foo bar", (size_t)7) == 7)
               && !str_cmp (out, "foo bar"));
    invariant ((pika_escape_iso8859_1_n (out, (size_t)255, 0, "foo\tbar", (size_t)7) == 7)
               && !str_cmp (out, "foo\tbar"));
    invariant ((pika_escape_iso8859_1_n (out, (size_t)255, char_class_control, "foo\tbar", (size_t)7) == 12)
               && !str_cmp (out, "foo\\(tab)bar"));
    invariant ((pika_escape_iso8859_1_n (out, (size_t)255, char_class_space, "foo\tbar", (size_t)7) == 12)
               && !str_cmp (out, "foo\\(tab)bar"));
    invariant ((pika_escape_iso8859_1_n (out, (size_t)255, 0, "foo\020bar", (size_t)7) == 7)
               && !str_cmp (out, "foo\020bar"));
    invariant ((pika_escape_iso8859_1_n (out, (size_t)255, char_class_control, "foo\020bar", (size_t)7) == 13)
               && !str_cmp (out, "foo\\(U+10)bar"));
    invariant ((pika_escape_iso8859_1_n (out, (size_t)255, char_class_space, "foo\020bar", (size_t)7) == 7)
               && !str_cmp (out, "foo\020bar"));


    invariant (pika_unescape_iso8859_1_n (&result_needed, &str_used, out, (size_t)3,
                                          "foo", (size_t)3) == -1);

    invariant (pika_unescape_iso8859_1_n (&result_needed, &str_used, out, (size_t)256,
                                          "foo\\(sp)bar", (size_t)11) == 0
               && str_cmp(out, "foo bar") == 0
               && result_needed == 7
               && str_used == 11);

    invariant (pika_unescape_iso8859_1_n (&result_needed, &str_used, out, (size_t)256,
                                          "foo\\(U+20)bar", (size_t)13) == 0
               && str_cmp(out, "foo bar") == 0
               && result_needed == 7
               && str_used == 13);

    invariant (pika_unescape_iso8859_1_n (&result_needed, &str_used, out, (size_t)3,
                                          "foo\\(sp)bar", (size_t)11) == -1
               && result_needed == 7
               && str_used == 3);

    invariant (pika_unescape_iso8859_1_n (&result_needed, &str_used, out, (size_t)4,
                                          "foo\\(sp)bar", (size_t)11) == -1
               && result_needed == 7
               && str_used == 8);

    invariant ( (test = pika_save_escape_iso8859_1 (0, 0, char_class_control, "foo\abar"))
                && str_cmp (test, "foo\\(U+7)bar") == 0);

    invariant ( (test_back = pika_save_unescape_iso8859_1 (0, 0, test))
                && !str_cmp (test_back, "foo\abar"));

    invariant ( (test = pika_save_escape_iso8859_1 (0, 0, char_class_control | char_class_space | char_class_non_ascii, "foo\tbar"))
                && !str_cmp (test, "foo\\(tab)bar"));

    lim_free (0,test);
    lim_free (0,test_back);


  }
  return 0;
}



