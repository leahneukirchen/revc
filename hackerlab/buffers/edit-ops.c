/* edit-ops.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/char-class.h"
#include "hackerlab/buffers/buffers.h"


/* typedefs */


/* __STDC__ prototypes for static functions */


/* declarations */


/* code */

int
bufpt_insert_str_n (t_buffer_point * pt,
                    t_uchar * str,
                    ssize_t len)
{
  if (0 > buf_insert_str_n (bufpt_session (pt), bufpt_buffer (pt), bufpt_pos (pt), str, len))
    return -1;

  return 0;
}

int
bufpt_backward_delete_n (t_buffer_point * pt_point,
                         ssize_t n)
{
  ssize_t bset;
  ssize_t buf;
  ssize_t pos;

  bset = bufpt_session (pt_point);
  buf = bufpt_buffer (pt_point);
  pos = bufpt_pos (pt_point);

  if ((bset < 0) || (buf < 0) || (pos < 0) || (n < 0) || (n > pos))
    return -1;

  return buf_delete_n (bset, buf, pos - n, n);
}



int
bufpt_is_eol (t_buffer_point * m,
              t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;
  t_uchar * burst;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 1;

  burst = buf_burst (0, bset, buffer, pos, 1);
  if (!burst)
    return -1;

  if (*burst == '\n')
    return 1;

  return 0;
}


int
bufpt_is_at_end (t_buffer_point * m,
                 t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 1;
  else
    return 0;
}

int
bufpt_char_at (t_buffer_point * m,
               t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return -1;

  return buf_n (bset, buffer, pos);
}


int
bufpt_looking_at_str_n (t_buffer_point * m,
                        t_buffer_point * end,
                        const t_uchar * str,
                        ssize_t len)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 0;

  if (len > (end_pos - pos))
    return 0;

  if (!len)
    return 1;

  while (len && (pos < end_pos))
    {
      t_uchar * burst;
      ssize_t burst_len;
      ssize_t x;

      burst = buf_burst (&burst_len, bset, buffer, pos, len);
      if (!burst)
        return -1;

      for (x = 0; x < burst_len; ++x)
        {
          if (burst[x] != *str)
            return 0;
          ++str;
          --len;
        }

      pos += x;
    }

  if (len)
    return 0;
  else
    return 1;
}


int
bufpt_looking_at_blank_line (t_buffer_point * m,
                             t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 0;

  while (pos < end_pos)
    {
      t_uchar * burst;
      ssize_t burst_len;
      ssize_t x;

      burst = buf_burst (&burst_len, bset, buffer, pos, end_pos - pos);
      if (!burst)
        return -1;

      for (x = 0; x < burst_len; ++x)
        {
          if (burst[x] == '\n')
            return 1;

          if (!char_is_blank (burst[x]))
            return 0;
        }
      pos += x;
    }
  return 1;
}


int
bufpt_looking_at_ci_str_n (t_buffer_point * m,
                           t_buffer_point * end,
                           t_uchar * str,
                           ssize_t len)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 0;

  if (len > (end_pos - pos))
    return 0;

  if (!len)
    return 1;

  while (len && (pos < end_pos))
    {
      t_uchar * burst;
      ssize_t burst_len;
      ssize_t x;

      burst = buf_burst (&burst_len, bset, buffer, pos, len);
      if (!burst)
        return -1;

      for (x = 0; x < burst_len; ++x)
        {
          if (char_to_lower (burst[x]) != char_to_lower (*str))
            return 0;
          ++str;
          --len;
        }

      pos += x;
    }

  if (len)
    return 0;
  else
    return 1;
}


int
bufpt_looking_at_re (t_buffer_point * m,
                     t_buffer_point * end_pt,
                     regex_t * preg,
                     int eflags)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;
  ssize_t size;
  ssize_t len;
  t_uchar * str;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end_pt);
  end_buffer = bufpt_buffer (end_pt);
  end_pos = bufpt_pos (end_pt);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if ((bset < 0) || (buffer < 0) || (pos < 0) || (end_pos < 0) || (end_pos < pos))
    return -1;

  size = buf_size (bset, buffer);

  if (end_pos > size)
    return -1;

  len = end_pos - pos;
  if (!len)
    str = 0;
  else
    {
      str = buf_range (bset, buffer, pos, len);
      if (!str)
        return -1;
    }

  {
    int r;

    r = regexec (preg, str, 0, 0, eflags);
    if (!r)
      return 1;
    else if (r == REG_NOMATCH)
      return 0;
    else
      return -1;
  }
}


ssize_t
bufpt_col (t_buffer_point * m)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  if ((bset < 0) || (buffer < 0) || (pos < 0))
    return -1;

  while (pos > 0)
    {
      t_uchar * s;

      s = buf_burst (0, bset, buffer, pos, 1);
      if (!s)
        return -1;

      if (*s == '\n')
        {
          ++pos;
          break;
        }
      else
        {
          --pos;
        }
    }

  return (bufpt_pos (m) - pos);
}


ssize_t
bufpt_col_tabifying (t_buffer_point * m)
{
  t_buffer_point pos;
  ssize_t answer;

  init_bufpt (&pos);

  if (0 > bufpt_copy_from (&pos, m))
    {
    bail:
      (void)uninit_bufpt (&pos);
      return -1;
    }
  
  if (0 > bufpt_beginning_of_line (&pos, m))
    goto bail;


  answer = 0;

  while (bufpt_pos (m) > bufpt_pos (&pos))
    {
      switch (bufpt_char_at (&pos, m))
        {
        default:
          answer += 1;
          break;
        case '\t':
          answer += 8;
          answer /= 8;
          answer *= 8;
          break;
        }
      if (0 > bufpt_goto (&pos, 1 + bufpt_pos (&pos)))
        goto bail;
    }

  if (0 > uninit_bufpt (&pos))
    goto bail;

  return answer;
}



int
bufpt_forward_char (t_buffer_point * m,
                    t_buffer_point * end,
                    ssize_t n)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return -1;

  if ((end_pos < 0) || (pos < 0))
    return -1;

  if ((pos + n) > end_pos)
    return -1;

  if ((pos + n) < 0)
    return -1;

  pos += n;

  if (0 > bufpt_goto (m, pos))
    return -1;

  return 0;
}

int 
bufpt_backward_char (t_buffer_point * m,
                     t_buffer_point * end,
                     ssize_t n)
{
  return bufpt_forward_char (m, end, -n);
}


int
bufpt_beginning_of_line (t_buffer_point * m,
                         t_buffer_point * end)
{
 tail:
  if (0 > bufpt_pos (m))
    return -1;

  if (0 == bufpt_pos (m))
    return 0;

  if (0 > bufpt_goto (m, -1 + bufpt_pos (m)))
    return -1;

  if ('\n' == bufpt_char_at (m, end))
    {
      if (0 > bufpt_forward_char (m, end, 1))
        return -1;
      return 0;
    }
  else
    goto tail;
}


#undef MIN
#define MIN(A,B) ((A) <= (B)  ? (A) : (B))

int
bufpt_next_line (t_buffer_point * m,
                 t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 0;

  while (pos < end_pos)
    {
      t_uchar * burst;
      ssize_t burst_len;
      ssize_t x;

      burst = buf_burst (&burst_len, bset, buffer, pos, MIN(16, end_pos - pos));
      if (!burst)
        return -1;

      for (x = 0; x < burst_len; ++x)
        {
          if (burst[x] == '\n')
            break;
        }

      if (x == burst_len)
        {
          pos += x;
        }
      else
        {
          pos += x + 1;
          break;
        }
    }

  if (0 > bufpt_goto (m, pos))
    return -1;

  return 0;
}


int
bufpt_end_of_line (t_buffer_point * m,
                   t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 0;

  while (pos < end_pos)
    {
      t_uchar * burst;
      ssize_t burst_len;
      ssize_t x;

      burst = buf_burst (&burst_len, bset, buffer, pos, MIN(16, end_pos - pos));
      if (!burst)
        return -1;

      for (x = 0; x < burst_len; ++x)
        {
          if (burst[x] == '\n')
            break;
        }

      pos += x;

      if (x != burst_len)
        {
          /* we found a newline.
           */
          break;
        }
    }

  if (0 > bufpt_goto (m, pos))
    return -1;

  return 0;
}


int
bufpt_pass_whitespace (t_buffer_point * m,
                       t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 0;

  while (pos < end_pos)
    {
      t_uchar * burst;
      ssize_t burst_len;
      ssize_t x;

      burst = buf_burst (&burst_len, bset, buffer, pos, MIN(16, end_pos - pos));
      if (!burst)
        return -1;

      for (x = 0; x < burst_len; ++x)
        {
          if (!char_is_space (burst[x]))
            break;
        }

      pos += x;
      if (x != burst_len)
        break;
    }

  if (0 > bufpt_goto (m, pos))
    return -1;

  return 0;
}


int
bufpt_pass_graphical (t_buffer_point * m,
                      t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 0;

  while (pos < end_pos)
    {
      t_uchar * burst;
      ssize_t burst_len;
      ssize_t x;

      burst = buf_burst (&burst_len, bset, buffer, pos, MIN(16, end_pos - pos));
      if (!burst)
        return -1;

      for (x = 0; x < burst_len; ++x)
        {
          if (!char_is_graph (burst[x]))
            break;
        }

      pos += x;
      if (x != burst_len)
        break;
    }

  if (0 > bufpt_goto (m, pos))
    return -1;

  return 0;
}


int
bufpt_pass_blanks (t_buffer_point * m,
                   t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 0;

  while (pos < end_pos)
    {
      t_uchar * burst;
      ssize_t burst_len;
      ssize_t x;

      burst = buf_burst (&burst_len, bset, buffer, pos, MIN(16, end_pos - pos));
      if (!burst)
        return -1;

      for (x = 0; x < burst_len; ++x)
        {
          if (!char_is_blank (burst[x]))
            break;
        }

      pos += x;
      if (x != burst_len)
        break;
    }

  if (0 > bufpt_goto (m, pos))
    return -1;

  return 0;
}



int
bufpt_pass_blank_lines (t_buffer_point * m,
                        t_buffer_point * end)
{
  ssize_t bset;
  ssize_t buffer;
  ssize_t pos;
  ssize_t end_bset;
  ssize_t end_buffer;
  ssize_t end_pos;

  ssize_t best_guess_end_pos;

  bset = bufpt_session (m);
  buffer = bufpt_buffer (m);
  pos = bufpt_pos (m);

  end_bset = bufpt_session (end);
  end_buffer = bufpt_buffer (end);
  end_pos = bufpt_pos (end);

  if ((end_bset != bset) || (buffer != end_buffer))
    return -1;

  if (end_pos <= pos)
    return 0;

  best_guess_end_pos = pos;

  while (pos < end_pos)
    {
      t_uchar * burst;
      ssize_t burst_len;
      ssize_t x;

      burst = buf_burst (&burst_len, bset, buffer, pos, MIN(16, end_pos - pos));
      if (!burst)
        return -1;

      for (x = 0; x < burst_len; ++x)
        {
          if (!char_is_space (burst[x]))
            break;
          else if (burst[x] == '\n')
            best_guess_end_pos = pos + x + 1;
        }

      pos += x;
      if (x != burst_len)
        break;
    }

  if (0 > bufpt_goto (m, best_guess_end_pos))
    return -1;

  return 0;
}




/* tag: Tom Lord Sun Nov 21 10:46:28 2004 (edit-ops.c)
 */
