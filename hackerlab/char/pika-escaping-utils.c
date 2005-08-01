/* pika-escaping-utils.c - saving escaped/unescaped pika scheme strings
 *
 ****************************************************************
 * Copyright (C) 2004, Christian Thaeter
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include "hackerlab/char/pika-escaping.h"
#include "hackerlab/char/pika-escaping-utils.h"


/* __STDC__ prototypes for static functions */


/************************************************************************
 *(h2 "Allocating Convenience Functions")
 * 
 * 
 * 
 */




/*(c pika_save_escape_iso8859_1_n)
 * t_uchar * pika__save_escape_iso8859_1_n (size_t * length_return,
 *                                          alloc_limits limits,
 *                                          int escape_classes,
 *                                          const t_uchar * str,
 *                                          size_t length);
 *
 * Return a freshly allocated, 0-terminated copy of `str' with
 * Pika escaping according to `escape_classes'.
 *
 * `str' is `length' characters long.
 *
 * Return 0 upon allocation failure.
 */
t_uchar *
pika_save_escape_iso8859_1_n (size_t * length_return,
                              alloc_limits limits,
                              int escape_classes,
                              const t_uchar * str,
                              size_t length)
{
  t_uchar * answer;
  t_uchar * realloced;
  size_t result_length;
  size_t needed;

  result_length = length * 2;

  answer = lim_malloc(limits, result_length);
  if (!answer)
    return 0;

 retry:
  needed = pika_escape_iso8859_1_n (answer, result_length, escape_classes, str, length);

  if (needed >= result_length)
    {
      result_length = needed + 1;
      realloced = lim_realloc (limits, answer, result_length);
      if (!realloced)
        goto enomem;
      answer = realloced;
      goto retry;
    }
  else
    {
      realloced = lim_realloc (limits, answer, needed + 1);
      if (!realloced)
        goto enomem;
      answer = realloced;
    }

  if (length_return)
    *length_return = needed;

  return answer;

 enomem:
  lim_free (limits, answer);
  return 0;
}



/*(c pika_save_escape_iso8859_1)
 * t_uchar * pika_save_escape_iso8859_1 (size_t * length_return,
 *                                       alloc_limits limits,
 *                                       int escape_classes,
 *                                       const t_uchar * str);
 *
 * Return a freshly allocated, 0-terminated copy of `str' with 
 * Pika escaping according to `escape_classes'.
 * 
 * Return 0 upon allocation failure.
 */
t_uchar *
pika_save_escape_iso8859_1 (size_t * length_return,
                            alloc_limits limits,
                            int escape_classes,
                            const t_uchar * str)
{
  size_t length;

  length = str_length (str);

  return pika_save_escape_iso8859_1_n (length_return, limits, escape_classes, str, length);
}



/*(c pika_save_unescape_iso8859_1_n)
 * t_uchar * pika_save_unescape_iso8859_1_n (size_t * length_return,
 *                                           alloc_limits limits,
 *                                           const t_uchar * str,
 *                                           size_t length);
 *
 * Return a freshly-allocated, 0-terminated copy of `str' with Pika
 * escaping removed.
 * 
 * `str' is `length' characters long.
 *
 * Return 0 upon allocation failure.
 */
t_uchar *
pika_save_unescape_iso8859_1_n (size_t * length_return,
                                alloc_limits limits,
                                const t_uchar * str,
                                size_t length)
{
  t_uchar * answer;
  t_uchar * realloced;
  size_t result_length;
  size_t result_needed;

  result_length = length;

  answer = lim_malloc (limits, result_length);
  if (!answer)
    return 0;

 retry:
  if ( pika_unescape_iso8859_1_n (&result_needed, 0, answer, result_length, str, length) == -1
       || result_needed >= result_length)
    {
      if (result_needed >= result_length)
        {
          result_length = result_needed + 1;
          realloced = lim_realloc (limits, answer, result_length);
          if (!realloced)
            goto error;
          answer = realloced;
          goto retry;
        }
      else
        goto error;
    }
  else
    {
      realloced = lim_realloc (limits, answer, result_needed + 1);
      if (!realloced)
        goto error;
      answer = realloced;
    }

  if (length_return)
    *length_return = result_needed;

  return answer;

 error:
  lim_free (limits, answer);
  return 0;
}


/*(c pika_save_unescape_iso8859_1)
 * t_uchar * pika_save_unescape_iso8859_1 (size_t * length_return,
 *                                         alloc_limits limits,
 *                                         const t_uchar * str);
 * 
 * Return a freshly-allocated, 0-terminated copy of `str' with Pika
 * escaping removed.
 *
 * Return 0 upon allocation failure.
 */
t_uchar *
pika_save_unescape_iso8859_1 (size_t * length_return,
                              alloc_limits limits,
                              const t_uchar * str)
{
  size_t length;

  length = str_length (str);

  return pika_save_unescape_iso8859_1_n (length_return, limits, str, length);
}

