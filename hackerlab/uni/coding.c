/* tag: Tom Lord Tue Dec  4 14:41:36 2001 (coding.c)
 */
/* coding.c -
 *
 ****************************************************************
 * Copyright (C) 2000, 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#define UNI_CODING_INLINES

#include <stddef.h>
#include "hackerlab/machine/types.h"
#include "hackerlab/machine/endian.h"
#include "hackerlab/bugs/panic.h"
#include "hackerlab/uni/coding.h"



uni_iscan_fn
uni_encoding_iscan_fn (enum uni_encoding_scheme encoding)
{
  switch (encoding)
    {
    case uni_iso8859_1:
      return uni_iso8859_1_iscan;

    case uni_utf8:
      return uni_utf8_iscan;

    case uni_utf16:
      return uni_utf16_iscan;

    case uni_utf16be:
      return uni_utf16be_iscan;

    case uni_utf16le:
      return uni_utf16le_iscan;

    case uni_utf32:
      return uni_utf32_iscan;

    case uni_utf32le:
      return uni_utf32le_iscan;

    case uni_utf32be:
      return uni_utf32be_iscan;

    case uni_bogus32:
      return uni_bogus32_iscan;

    case uni_bogus32le:
      return uni_bogus32le_iscan;

    case uni_bogus32be:
      return uni_bogus32be_iscan;
    }

  while (1)
    panic ("not reached in uni_encoding_iscan_fn");
}


uni_iput_fn
uni_encoding_iput_fn (enum uni_encoding_scheme encoding)
{
  switch (encoding)
    {
    case uni_iso8859_1:
      return uni_iso8859_1_iput;

    case uni_utf8:
      return uni_utf8_iput;

    case uni_utf16:
      return uni_utf16_iput;

    case uni_utf16be:
      return uni_utf16be_iput;

    case uni_utf16le:
      return uni_utf16le_iput;

    case uni_utf32:
      return uni_utf32_iput;

     case uni_utf32le:
      return uni_utf32le_iput;

    case uni_utf32be:
      return uni_utf32be_iput;

    case uni_bogus32:
      return uni_bogus32_iput;

    case uni_bogus32be:
      return uni_bogus32be_iput;

    case uni_bogus32le:
      return uni_bogus32le_iput;
    }

  while (1)
    panic ("not reached in uni_encoding_iput_fn");
}

