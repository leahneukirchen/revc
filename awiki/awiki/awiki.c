/* cmd-awiki.c
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "config-options.h"
#include "hackerlab/os/limits.h"
#include "hackerlab/cmd/main.h"
#include "hackerlab/identifiers/identifier.h"
#include "hackerlab/graphs/digraph.h"
#include "hackerlab/buffers/buffers.h"
#include "awiki/libawiki/awiki.h"


/* __STDC__ prototypes for static functions */



static void safe_spew_file_html (int out_fd,
                                 ssize_t bs,
                                 const t_uchar * input_file,
                                 int input_fd,
                                 const t_uchar * parse_for_type_id,
                                 const t_uchar * root_prefix);
static int one_file_to_html (ssize_t * output_buffer_ret,
                             ssize_t * errors_buffer_ret,
                             ssize_t bs,
                             const t_uchar * input_file,
                             int input_fd,
                             const t_uchar * parse_for_type_id,
                             const t_uchar * root_prefix);


/* Declarations */
Identifier_decl (outline, "outline");



static t_uchar * usage = "[options] [input-file]";
static t_uchar * version_string = (cfg__std__package " from regexps.com\n"
                                   "\n"
                                   "Copyright 2004 Tom Lord\n"
                                   "\n"
                                   "This is free software; see the source for copying conditions.\n"
                                   "There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A\n"
                                   "PARTICULAR PURPOSE.\n"
                                   "\n"
                                   "Report bugs to " cfg__awiki_bug_mail ".\n"
                                   "\n"
                                   cfg__std__release_id_string
                                   "\n");

#define OPTS(OP) \
  OP (opt_help_msg, "h", "help", 0, \
      "Display a help message and exit.") \
  OP (opt_long_help, "H", 0, 0, \
      "Display a verbose help message and exit.") \
  OP (opt_version, "V", "version", 0, \
      "Display a release identifier string\n" \
      "and exit.") \
  OP (opt_root_prefix, "r", "root DIR", 1, \
      "Use DIR as the root prefix.") \
  OP (opt_nav_txt, "n", "nav FILE", 1, \
      "nav txt.") \
  OP (opt_nav_css, "N", "nav-css FILE", 1, \
      "nav css.") \
  OP (opt_main_title, "t", "title STRING", 1, \
      "use STRING as the page title") \


t_uchar arch_cmd_awiki_help[] =
  ("parse and translate an Awiki document\n"

   "Parse the input file.  By default, convert it to HTML.\n");

enum options
{
  OPTS (OPT_ENUM)
};

static struct opt_desc opts[] =
{
  OPTS (OPT_DESC)
    {-1, 0, 0, 0, 0}
};



int
main (int argc, char ** argv)
{
  int o;
  struct opt_parsed * option;
  t_uchar * input_file;
  int input_fd;
  ssize_t bs;
  t_uchar * root_prefix = 0;
  t_uchar * nav_file = 0;
  t_uchar * nav_css_file = 0;
  t_uchar * title = 0;

  safe_buffer_fd (1, 0, O_WRONLY, 0);

  option = 0;

  while (1)
    {
      o = opt_standard (lim_use_must_malloc, &option, opts, &argc, argv, "awiki", usage, version_string, arch_cmd_awiki_help, opt_help_msg, opt_long_help, opt_version);
      if (o == opt_none)
        break;

      switch (o)
        {
        default:
          safe_printfmt (2, "unhandled option `%s'\n", option->opt_string);
          panic ("internal error parsing arguments");

        usage_error:
          opt_usage (2, argv[0], "awiki", usage, 1);
          exit (1);

          /* bogus_arg: */
          safe_printfmt (2, "ill-formed argument for `%s' (`%s')\n", option->opt_string, option->arg_string);
          goto usage_error;

        case opt_root_prefix:
          {
            root_prefix = str_save (0, option->arg_string);
            break;
          }

        case opt_nav_txt:
          {
            nav_file = str_save (0, option->arg_string);
            break;
          }

        case opt_nav_css:
          {
            nav_css_file = str_save (0, option->arg_string);
            break;
          }

        case opt_main_title:
          {
            if (title)
              lim_free (0, title);
            title = str_save (0, option->arg_string);
            break;
          }
        }
    }

  if (argc == 1)
    {
      input_file = "stdin:";
      input_fd = 0;
    }
  else if (argc == 2)
    {
      input_file = argv[1];
      input_fd = safe_open (input_file, O_RDONLY, 0);
    }
  else
    {
      goto usage_error;
    }

  if (!title)
    title = str_save (0, "An Awiki Page");


  if (0 > init_awiki ())
    exit (5);

  bs = buffer_session_open (0, 0);
  if (bs < 0)
    exit (20);


  {
    safe_printfmt (1, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n");
    safe_printfmt (1, "<html>\n");
    safe_printfmt (1, "  <head>\n");
    safe_printfmt (1, "    <title>%s</title>\n", title);
    safe_printfmt (1, "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n");
    if (nav_css_file)
      safe_printfmt (1, "    <link href=\"%s\" rel=\"stylesheet\" type=\"text/css\">\n", nav_css_file);
    safe_printfmt (1, "  </head>\n");
    safe_printfmt (1, "\n");

    safe_printfmt (1, "  <body>\n");
    safe_printfmt (1, "\n");
    safe_printfmt (1, "    <h1 class=\"page-title\"><u>%s</u></h1>\n", title);
    safe_printfmt (1, "\n");

    safe_printfmt (1, "    <div class=\"mainContent\">\n");
    safe_spew_file_html (1, bs, input_file, input_fd, Identifier (outline), root_prefix);
    safe_printfmt (1, "    </div>\n");
    safe_printfmt (1, "\n");

    safe_printfmt (1, "    <div class=\"navLeft\">\n");
    if (nav_file)
      {
        int nav_file_fd;

        nav_file_fd = safe_open (nav_file, O_RDONLY, 0);
        safe_spew_file_html (1, bs, nav_file, nav_file_fd, Identifier (outline), root_prefix);
      }

    safe_printfmt (1, "    </div>\n");
    safe_printfmt (1, "\n");

    safe_printfmt (1, "  </body>\n");
    safe_printfmt (1, "</html>\n");
  }
  exit (0);
}




static void
safe_spew_file_html (int out_fd,
                     ssize_t bs,
                     const t_uchar * input_file,
                     int input_fd,
                     const t_uchar * parse_for_type_id,
                     const t_uchar * root_prefix)
{
  ssize_t html_buffer;
  ssize_t errors_buffer;
  t_uchar * str;
  ssize_t len;

  if (0 > one_file_to_html (&html_buffer, &errors_buffer, bs, input_file, input_fd, Identifier (outline), root_prefix))
    exit (30);

  len = buf_size (bs, html_buffer);
  str = buf_range (bs, html_buffer, 0, len);
  safe_write (1, str, len);
}



static int
one_file_to_html (ssize_t * output_buffer_ret,
                  ssize_t * errors_buffer_ret,
                  ssize_t bs,
                  const t_uchar * input_file,
                  int input_fd,
                  const t_uchar * parse_for_type_id,
                  const t_uchar * root_prefix)
{
  int errn;
  ssize_t source_buffer;
  ssize_t output_buffer;
  ssize_t errors_buffer;
  ssize_t gr;

  if (output_buffer_ret)
    *output_buffer_ret = -1;
  if (errors_buffer_ret)
    *errors_buffer_ret = -1;

  source_buffer = buffer_session_index_create (bs, "*awiki-source*");
  if (source_buffer < 0)
    return -1;
  output_buffer = buffer_session_index_create (bs, "*awiki-output*");
  if (output_buffer < 0)
    return -1;
  errors_buffer = buffer_session_index_create (bs, "*awiki-errors*");
  if (errors_buffer < 0)
    return -1;

  if (0 > buf_delete_n (bs, source_buffer, 0, buf_size (bs, source_buffer)))
    return -1;
  if (0 > buf_delete_n (bs, output_buffer, 0, buf_size (bs, output_buffer)))
    return -1;
  if (0 > buf_delete_n (bs, errors_buffer, 0, buf_size (bs, errors_buffer)))
    return -1;

  if (0 > buf_insert_from_fd (&errn, 0, bs, source_buffer, input_fd, 0, -1))
    return -1;

  
  gr = open_digraph (0, 0);
  if (gr < 0)
    return -1;
  
  {
    t_buffer_point start_pt;
    t_buffer_point end_pt;
    t_buffer_point output_pt;
    t_buffer_point error_pt;
    ssize_t node;

    init_bufpt (&start_pt);
    init_bufpt (&end_pt);
    init_bufpt (&output_pt);
    init_bufpt (&error_pt);

    if (0 > bufpt_goto_abs (&output_pt, bs, output_buffer, 0))
      return -1;

    if (0 > bufpt_goto_abs (&start_pt, bs, source_buffer, 0))
      return -1;

    if (0 > bufpt_goto_abs (&end_pt, bs, source_buffer, buf_size (bs, source_buffer)))
      return -1;

    node = digraph_alloc_node (gr);
    if (node < 0)
      return -1;

    if (0 > awiki_parse_for_type (parse_for_type_id, gr, node, &error_pt, &start_pt, &end_pt))
      return -1;

    {
      t_awiki_html_state state;

      if (0 > init_awiki_init_html_state (&state, root_prefix))
        return -1;

      if (0 > awiki_to_html (&state, gr, node, &error_pt, &output_pt))
        return -1;
    }
  }

  if (output_buffer_ret)
    *output_buffer_ret = output_buffer;

  if (errors_buffer_ret)
    *errors_buffer_ret = errors_buffer;

  return 0;
}
                  




/* arch-tag: Tom Lord Sun Nov 21 10:01:35 2004 (awiki/main.c)
 */
