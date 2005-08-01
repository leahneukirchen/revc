/* display.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "hackerlab/char/str.h"
#include "awiki/libawiki-nodes/display.h"


/* __STDC__ prototypes for static functions */
static int skip_to_end_of_indented_body (t_buffer_point * pos,
                                         t_buffer_point * end_pt,
                                         ssize_t ref_col);


Identifier_safe_decl (unparsed, "unparsed");

Regexp_decl (trailing_headers_lead, "^\\.[[:alpha:]][-[:alnum:]]*:", REG_EXTENDED);



int
awiki_constrained_display_scan (const t_uchar * type_name,
                                t_buffer_point * start_pt,
                                t_buffer_point * end_pt)
{
  int res;

  if (!bufpt_looking_at_str_n (start_pt, end_pt, "[[", 2))
    return 0;

  if (0 > bufpt_goto (start_pt, 2 + bufpt_pos (start_pt)))
    return -1;

  res = bufpt_looking_at_str_n (start_pt, end_pt, type_name, str_length (type_name));
  if (res <= 0)
    {
      (void)bufpt_goto (start_pt, -2 + bufpt_pos (start_pt));
      return res;
    }
  
  if (0 > bufpt_goto (start_pt, -2 + bufpt_pos (start_pt)))
    {
      return -1;
    }

  return awiki_display_scan (start_pt, end_pt);
}


int
awiki_display_scan (t_buffer_point * start_pt,
                    t_buffer_point * end_pt)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t ref_col;


  bset = bufpt_session (start_pt);
  buffer = bufpt_buffer (start_pt);
  pos = bufpt_pos (start_pt);

  if (!bufpt_looking_at_str_n (start_pt, end_pt, "[[", 2))
    return 0;

  ref_col = bufpt_col_tabifying (start_pt);

  if (0 > bufpt_next_line (start_pt, end_pt))
    return -1;

  {
    /* skip past the indented body of the display:
     */

    while (bufpt_pos (start_pt) < bufpt_pos (end_pt))
      {
        t_buffer_point lookahead;
        
        init_bufpt (&lookahead);

        if (0 > bufpt_copy_from (&lookahead, start_pt))
          return -1;
        
        /* Lookahead is at the beginning of a line.  Is this line
         * part of the display body?
         */

        if (0 > bufpt_pass_blanks (&lookahead, end_pt))
          {
            uninit_bufpt (&lookahead);
            return -1;
          }

        if (bufpt_is_eol (start_pt, end_pt))
          {
            /* blank line: yes, that's part of the dipslay.
             */
            if (0 > bufpt_next_line (&lookahead, end_pt))
              {
                uninit_bufpt (&lookahead);
                return -1;
              }
            if (0 > bufpt_goto (start_pt, bufpt_pos (&lookahead)))
              {
                uninit_bufpt (&lookahead);
                return -1;
              }
            uninit_bufpt (&lookahead);
            continue;
          }
        else if (ref_col < bufpt_col_tabifying (&lookahead))
          {
            /* non-blank line indented far enough: part of the display
             */
            if (0 > bufpt_next_line (&lookahead, end_pt))
              {
                uninit_bufpt (&lookahead);
                return -1;
              }
            if (0 > bufpt_goto (start_pt, bufpt_pos (&lookahead)))
              {
                uninit_bufpt (&lookahead);
                return -1;
              }
            uninit_bufpt (&lookahead);
            continue;
          }
        else
          {
            /* non-blank line, not indented far enough to be part
             * of the display body.  Terminate the loop.
             */
            uninit_bufpt (&lookahead);
            break;
          }
      }
  }

  /* start_pt is now at the beginning of the first line after the 
   * body of the display.
   * 
   * There /should/ be a "]]" line here, optionally followed by trailing
   * display headers.
   */

  {
    t_buffer_point lookahead;

    init_bufpt (&lookahead);
    if (0 > bufpt_copy_from (&lookahead, start_pt))
      {
        return -1;
      }
    {
      if (0 > bufpt_pass_blanks (&lookahead, end_pt))
        {
          uninit_bufpt (&lookahead);
          return -1;
        }

      if (!bufpt_looking_at_str_n (&lookahead, end_pt, "]]", 2))
        {
#undef FIXME
          /* spew warning; */
        }
      else
        {
          if (0 > bufpt_next_line (&lookahead, end_pt))
            {
              uninit_bufpt (&lookahead);
              return -1;
            }
          if (0 > bufpt_goto (start_pt, bufpt_pos (&lookahead)))
            {
              uninit_bufpt (&lookahead);
              return -1;
            }
          if (0 > bufpt_pass_blanks (&lookahead, end_pt))
            {
              uninit_bufpt (&lookahead);
              return -1;
            }
          if (bufpt_looking_at_re (&lookahead, end_pt, Regexp(trailing_headers_lead), 0))
            {
              if (0 > bufpt_forward_char (&lookahead, end_pt, 1))
                {
                  uninit_bufpt (&lookahead);
                  return -1;
                }
#undef FIXME
              /* scan headers here, setting start_pt */
            }
          
        }
    }
    uninit_bufpt (&lookahead);
  }

  return 1;
}


int
awiki_parse_for_display (ssize_t graph,
                         ssize_t node,
                         t_buffer_point * error_pt,
                         t_buffer_point * start_pt,
                         t_buffer_point * end_pt)
{
  t_buffer_point pos;
  t_buffer_point display_start;
  t_buffer_point display_end;
  int found_display;
  int answer = -1;


  init_bufpt (&pos);
  init_bufpt (&display_start);
  init_bufpt (&display_end);

  if (0 > bufpt_copy_from (&pos, start_pt))
    {
    garked:

      (void)uninit_bufpt (&pos);
      (void)uninit_bufpt (&display_start);
      (void)uninit_bufpt (&display_end);
      return -1;
    }

  if (0 > bufpt_pass_whitespace (&pos, end_pt))
    goto garked;

  if (0 > bufpt_copy_from (&display_start, &pos))
    goto garked;

  found_display = awiki_display_scan (&pos, end_pt);
  if (found_display < 0)
    goto garked;
  else if (!found_display)
    {
      answer = 0;
      goto return_normal_answer;
    }
  else
    {
      const t_uchar * type_id;
      ssize_t ref_col;

      if (0 > bufpt_copy_from (&display_end, &pos))
        goto garked;

      if (0 > awiki_display_parse_headers (&type_id, &ref_col, graph, node, error_pt, &display_start, &display_end))
        goto garked;

      if (!type_id)
        type_id = Identifier (unparsed);

      if (0 > awiki_parse_for_type (type_id, graph, node, error_pt, &display_start, &display_end))
        goto garked;

      if (0 > bufpt_goto (start_pt, bufpt_pos (&pos)))
        goto garked;

      answer = 1;

    return_normal_answer:
      if (0 > uninit_bufpt (&display_start))
        {
          (void)uninit_bufpt (&display_end);
          (void)uninit_bufpt (&pos);
          return -1;
        }
      if (0 > uninit_bufpt (&display_end))
        {
          (void)uninit_bufpt (&pos);
          return -1;
        }
      if (0 > uninit_bufpt (&pos))
        {
          return -1;
        }
      return answer;
    }
}




int
awiki_display_parse_headers (const t_uchar ** type_id,
                             ssize_t * ref_col,
                             ssize_t graph,
                             ssize_t node,
                             t_buffer_point * error_pt,
                             t_buffer_point * start_pt,
                             t_buffer_point * end_pt)
{
  /* Input should be a string
   * 
   * with 
   * 
   *    [[<type>  <optional frobs>
   *			<optional headers>
   *	   <indented body>
   *    <optional trailer, frobs, headers>
   *
   * where the optional trailer is:
   *
   *	]] <optional frobs>
   *		<optional headers>
   * 
   * 
   * This function should adjust start_pt and end_pt should surround
   * only the indented body.   The header field values provided as
   * headers or frobs should be stored as properties of the node.
   * We return the user's declared type for this display.
   */

  t_buffer_point pos;
  t_buffer_point type_name_start;
  t_buffer_point type_name_end;
  ssize_t reference_column;

  init_bufpt (&pos);
  init_bufpt (&type_name_start);
  init_bufpt (&type_name_end);
  
  if (   (0 > bufpt_copy_from (&pos, start_pt))
      || (0 > bufpt_copy_from (&type_name_start, start_pt))
      || (0 > bufpt_copy_from (&type_name_end, start_pt)))
    {
    bail:
      uninit_bufpt (&pos);
      uninit_bufpt (&type_name_start);
      uninit_bufpt (&type_name_end);
      return -1;
    }

  if (0 > bufpt_pass_whitespace (&pos, end_pt))
    goto bail;

  if (0 >= bufpt_looking_at_str_n (&pos, end_pt, "[[", 2))
    {
    /* bogus_parse_failsafe: */
      /* Leave start_pt and end_pt alone -- we can't parse any
       * headers here.
       */
      if (type_id)
        *type_id = Identifier(unparsed);

      if (ref_col)
        *ref_col = 0;

    normal_exit:
      /* Assume start_pt and end_pt and *type_id *ref_col are all set.
       */
      {
        int s;
        int t;
        int u;

        s = uninit_bufpt (&pos);
        t = uninit_bufpt (&type_name_start);
        u = uninit_bufpt (&type_name_end);

        if ((s < 0) || (t < 0) || (u < 0))
          return -1;
        return 0;
      }
    }

  reference_column = bufpt_col_tabifying (&pos);
  if (ref_col)
    *ref_col = reference_column;

  if (0 > bufpt_goto (&type_name_start, 2 + bufpt_pos (&pos)))
    goto bail;

  if (0 > bufpt_pass_graphical (&pos, end_pt))
    goto bail;

  if (0 > bufpt_goto (&type_name_end, bufpt_pos (&pos)))
    goto bail;

  /* For now, skip frobs and optional headers.
   * We have the type....
   * Just find the extent of the body.
   */

  if (type_id)
    {
      ssize_t id_name_len;
      t_uchar * id_name_data;
      t_uchar * id_name = 0;

      id_name_len =  bufpt_pos (&type_name_end) - bufpt_pos (&type_name_start);
      id_name_data = bufpt_range (&type_name_start, id_name_len);
      if (!id_name_data)
        goto bail;

      id_name = str_save_n (lim_use_malloc, id_name_data, id_name_len);
      if (!id_name)
        goto bail;

      *type_id = intern_identifier (id_name);
      lim_free (lim_use_malloc, id_name);
      id_name = 0;

      if (!*type_id)
        goto bail;
    }

  if (0 > bufpt_next_line (&pos, end_pt))
    goto bail;

  if (0 > bufpt_goto (start_pt, bufpt_pos (&pos)))
    goto bail;

  if (0 > skip_to_end_of_indented_body (&pos, end_pt, reference_column))
    goto bail;

  if (0 > bufpt_goto (end_pt, bufpt_pos (&pos)))
    goto bail;

  goto normal_exit;
}

static int
skip_to_end_of_indented_body (t_buffer_point * pos,
                              t_buffer_point * end_pt,
                              ssize_t ref_col)
{
  t_buffer_point lookahead;

  init_bufpt (&lookahead);

  if (0 > bufpt_copy_from (&lookahead, pos))
    return -1;

  while (1)
    {
      if (0 > bufpt_pass_blanks (&lookahead, end_pt))
        {
        bail:
          (void)uninit_bufpt (&lookahead);
          return -1;
        }

      if (bufpt_is_eol (&lookahead, end_pt))
        {
        this_line_in_display:
          if (0 > bufpt_next_line (&lookahead, end_pt))
            goto bail;

          if (0 > bufpt_goto (pos, bufpt_pos (&lookahead)))
            goto bail;

          if (0 < bufpt_is_at_end (&lookahead, end_pt))
            {
              if (0 > uninit_bufpt (&lookahead))
                return -1;
              return 0;
            }

          continue;
        }

      if (ref_col < bufpt_col_tabifying (&lookahead))
        goto this_line_in_display;

      if (0 > uninit_bufpt (&lookahead))
        return -1;
      return 0;                 /* this line (lookahead's) is not in the indented body */
    }
}



/* arch-tag: Tom Lord Sat Oct 30 09:39:19 2004 (display.c)
 */
