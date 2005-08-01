/* unit-md5.c: 
 *
 ****************************************************************
 * Copyright (C) 2003 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include "hackerlab/hash/md5.h"
#include "hackerlab/cmd/main.h"



static t_uchar * program_name = "unit-hash-md5";
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
    struct test_case
      {
        char * string;
        t_uchar answer[16];
      };

    struct test_case tests[] = 
      {
        {"", {0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04, 0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E}},
        {"a", {0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8, 0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61}},
        {"abc", {0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0, 0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72}},
        {"message digest", {0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D, 0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0}},
        {"Lo, this message is exactly 64 bytes long, no more and not less.",
           { 0x39, 0x10, 0x8a, 0xaa, 0xa6, 0x2c, 0x5b, 0x5b, 0x1b, 0x9f, 0x8b, 0xdb, 0xb2, 0x80, 0x79, 0x89 }},
        {"Lo, this message is exactly 64 bytes long, no more and not less.",
           { 0x39, 0x10, 0x8a, 0xaa, 0xa6, 0x2c, 0x5b, 0x5b, 0x1b, 0x9f, 0x8b, 0xdb, 0xb2, 0x80, 0x79, 0x89 }},
        {"This message has 56 bytes which is an interesting amount",
           { 0x6d, 0xe2, 0xb2, 0x26, 0x31, 0x73, 0x7a, 0x22, 0x9d, 0x1e, 0x02, 0x20, 0x63, 0x8f, 0x86, 0xc0 }},
        {"This message has 57 bytes which is an interesting amount!",
           { 0x9f, 0xed, 0x89, 0xb2, 0x9d, 0xce, 0x82, 0x94, 0x97, 0xf0, 0x6a, 0xdd, 0xaf, 0x9f, 0x15, 0xd7 }},
        {"This message has 58 bytes which is an interesting amount!!",
           { 0x94, 0x05, 0x4c, 0x5f, 0xda, 0x77, 0x7c, 0x61, 0xb4, 0x43, 0xde, 0x80, 0x80, 0x3e, 0x52, 0xca }},
        {("This message has rather more than 64 bytes\n"
          "which means that it will take more than md5 buffer\n"
          "to process.  I think that that's an interesting case\n"
          "to test for, don't you?   I could try to come up with\n"
          "more edge cases but I think this will be the last one\n"
          "for now.\n"),
           { 0x02, 0x33, 0x7f, 0x85, 0xba, 0x8e, 0x58, 0x7e, 0x65, 0x0b, 0xc7, 0x1d, 0x60, 0x91, 0xf1, 0xa2 }},
        {0, }
      };

    md5_context_t context_a;
    int x;

    context_a = make_md5_context (0);

    for (x = 0; tests[x].string; ++x)
      {
        md5_context_t context_b = 0;
        t_uchar digest_a[16];
        t_uchar digest_b[16];
        int y;

        context_b = make_md5_context (0);

        md5_scan (context_a, tests[x].string, str_length (tests[x].string));
        md5_scan (context_b, tests[x].string, str_length (tests[x].string));
        md5_final (digest_a, context_a);
        md5_final (digest_b, context_b);

        for (y = 0; y < 16; ++y)
          {
            invariant (digest_a[y] == digest_b[y]);
            invariant (digest_a[y] == tests[x].answer[y]);
          }

        free_md5_context (0, context_b);
      }

    free_md5_context (0, context_a);
  }

  return 0;
}




/* tag: Tom Lord Fri Oct 24 09:12:09 2003 (unit-md5.c)
 */

