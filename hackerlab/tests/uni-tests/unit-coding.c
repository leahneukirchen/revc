/* tag: Tom Lord Tue Dec  4 14:41:21 2001 (unit-coding.c)
 */
/* unit-coding.c -
 *
 ****************************************************************
 * Copyright (C) 2000, 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/uni/coding.h"
#include "hackerlab/tests/uni-tests/cvt_opt.h"


#if 0

typedef int (* conv_fun_t) (t_uchar *, size_t *, size_t, t_uchar *, size_t *, size_t);



static int
identity (t_uchar * to, size_t * tpos, size_t allocated, t_uchar * from, size_t * fpos, size_t len)
{
  to = memcpy ((void *) (to + (*tpos)) , (void *) (from + (*fpos)), len);
  *tpos += len;
  *fpos += len;
  return 0;
}

static const char *
name (conversion_t t)
{
  switch (t)
    {
    case cvt_8_16:
        return "8 -> 16";
    case cvt_8_32:
      return "8 -> 32";
    case cvt_16_8:
      return "16 -> 8";
    case cvt_16_32:
      return "16 -> 32";
    case cvt_32_8:
      return "32 -> 8";
    case cvt_32_16:
      return "32 -> 16";
    default:
      panic ("Unsupported conversion");
      return 0;
    }
}

static conv_fun_t
conv_fun (conversion_t t)
{
  switch (t)
    {
    case cvt_8_16:
      return uni_utf8_to_utf16;
    case cvt_8_32:
      return uni_utf8_to_utf32;
    case cvt_16_8:
      return uni_utf16_to_utf8;
    case cvt_16_32:
      return uni_utf16_to_utf32;
    case cvt_32_8:
      return uni_utf32_to_utf8;
    case cvt_32_16:
      return uni_utf32_to_utf16;
    default:
      panic ("Unsupported conversion");
      return 0;
    }
}

#if MACHINE_IS_BIGENDIAN
#define IF_BIGENDIAN(A, B) A
#else
#define IF_BIGENDIAN(A, B) A
#endif

static conv_fun_t
non_native_conv_fun (conversion_t t)
{
  switch (t)
    {
    case cvt_8_16:
      return IF_BIGENDIAN (uni_utf8_to_utf16le, uni_utf8_to_utf16be);
    case cvt_8_32:
      return IF_BIGENDIAN (uni_utf8_to_utf32le, uni_utf8_to_utf32be);
    case cvt_16_8:
      return IF_BIGENDIAN (uni_utf16le_to_utf8, uni_utf16be_to_utf8);
    case cvt_16_32:
      return IF_BIGENDIAN (uni_utf16le_to_utf32le, uni_utf16be_to_utf32be);
    case cvt_32_8:
      return IF_BIGENDIAN (uni_utf32le_to_utf8, uni_utf32be_to_utf8);
    case cvt_32_16:
      return IF_BIGENDIAN (uni_utf32le_to_utf16le, uni_utf32be_to_utf16be);
    default:
      panic ("Unsupported conversion");
      return 0;
    }
}

static conv_fun_t
to_non_native_fun (conversion_t t)
{
  switch (t)
    {
    case cvt_8_16: case cvt_8_32:
      return identity;
    case cvt_16_8: case cvt_16_32:
      return IF_BIGENDIAN (uni_utf16_to_utf16le, uni_utf16_to_utf16be);
    case cvt_32_8: case cvt_32_16:
      return IF_BIGENDIAN (uni_utf32_to_utf32le, uni_utf32_to_utf32be);
    default:
      panic ("Unsupported conversion");
      return 0;
    }
}

static conv_fun_t
to_native_fun (conversion_t t)
{
  switch (t)
    {
    case cvt_16_8: case cvt_32_8:
      return identity;
    case cvt_8_16: case cvt_32_16:
      return IF_BIGENDIAN (uni_utf16le_to_utf16, uni_utf16be_to_utf16);
    case cvt_8_32: case cvt_16_32:
      return IF_BIGENDIAN (uni_utf32le_to_utf32, uni_utf32be_to_utf32);
    default:
      panic ("Unsupported conversion");
      return 0;
    }
}

static void
cvt_panic (const char * fmt, conversion_t t)
{
  safe_printfmt (2, (char *)fmt, name(t));
  panic_exit();
}

#endif 



int
main (int argc, char * argv[])
{
#if 0
  int errn;
  int non_native;
  conversion_t fromto;

  t_uchar * file_contents;
  size_t len;
  size_t file_pos;

  t_uchar * converted_text;
  size_t allocated;
  size_t converted_pos;

  if (vu_file_to_string (&errn, &file_contents, &len, 0))
    panic ("error reading file");


  allocated = len * 6;
  converted_text = (t_uchar *)must_malloc (allocated);

  converted_pos = 0;
  file_pos = 0;

  cvt_option_setup ("unit-coding", 0, 0);
  fromto = cvt_option (argc, argv, &non_native);

  if (!non_native)
    {
      if (conv_fun(fromto) (converted_text, &converted_pos, allocated, file_contents, &file_pos, len))
        cvt_panic ("%s conversion failed", fromto);
    }
  else
    {
      if (to_non_native_fun(fromto) (converted_text, &converted_pos,
                                     allocated, file_contents, &file_pos, len))
          cvt_panic ("%s conversion: initial translation to non-native failed", fromto);
      else
        {
          t_uchar * nn_converted_text;
          size_t nn_converted_pos;
          size_t nn_file_pos;

          nn_converted_text = (t_uchar *)must_malloc (allocated);
          nn_converted_pos = 0;
          nn_file_pos = 0;

          if (non_native_conv_fun(fromto) (nn_converted_text, &nn_converted_pos, allocated,
                                           converted_text, &nn_file_pos, converted_pos))
              cvt_panic ("%s non-native conversion failed", fromto);

          converted_pos = 0;
          file_pos = 0;

          if (to_native_fun(fromto) (converted_text, &converted_pos, allocated,
                                     nn_converted_text, &file_pos, nn_converted_pos))
              cvt_panic ("%s conversion: final translation to native failed", fromto);
        }

    }

  if (converted_pos != vu_write_retry (&errn, 1, converted_text, converted_pos))
    cvt_panic ("%s conversion: write failure", fromto);

#endif
  return 0;
}

