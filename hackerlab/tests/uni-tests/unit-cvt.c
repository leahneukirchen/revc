/* tag: Tom Lord Tue Dec  4 14:41:20 2001 (unit-cvt.c)
 */
/* unit-cvt.c -
 *
 ****************************************************************
 * Copyright (C) 2000, 2004 Tom Lord
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/tests/uni-tests/cvtutf.h"
#include "hackerlab/tests/uni-tests/cvt_opt.h"



int
main (int argc, char * argv[])
{
  int errn;
  conversion_t fromto;

  t_uchar * file_contents;
  size_t len;

  size_t allocated;
  t_uchar * converted_text;
  t_uchar * converted_end;
  ConversionResult result;

  size_t write_amt;

  if (vu_file_to_string (&errn, &file_contents, &len, 0))
    panic ("error reading file");


  allocated = len * 6;
  converted_text = (t_uchar *)must_malloc (allocated);
  converted_end = converted_text;

  cvt_option_setup ("unit-cvt", 0, 0);
  fromto = cvt_option (argc, argv, 0);

  switch (fromto)
    {
    default:
      panic ("Unsupported conversion");
      break;
    case cvt_8_16:
      result = ConvertUTF8toUTF16 ((const UTF8 **)&file_contents, (UTF8 *)(file_contents + len),
                                   (UTF16 **)&converted_end, (UTF16 *)(converted_end + allocated),
                                   strictConversion);
      break;
    case cvt_8_32:
      result = ConvertUTF8toUTF32 ((const UTF8 **)&file_contents, (UTF8 *)(file_contents + len),
                                   (UTF32 **)&converted_end, (UTF32 *)(converted_end + allocated),
                                   strictConversion);
      break;
    case cvt_16_8:
      result = ConvertUTF16toUTF8 ((const UTF16 **)&file_contents, (UTF16 *)(file_contents + len),
                                   (UTF8 **)&converted_end, (UTF8 *)(converted_end + allocated),
                                   strictConversion);
      break;
    case cvt_16_32:
      result = ConvertUTF16toUTF32 ((const UTF16 **)&file_contents, (UTF16 *)(file_contents + len),
                                    (UTF32 **)&converted_end,
                                    (UTF32 *)(converted_end + allocated), strictConversion);
      break;
    case cvt_32_8:
      result = ConvertUTF32toUTF8 ((const UTF32 **)&file_contents, (UTF32 *)(file_contents + len),
                                   (UTF8 **)&converted_end, (UTF8 *)(converted_end + allocated),
                                   strictConversion);
      break;
    case cvt_32_16:
      result = ConvertUTF32toUTF16 ((const UTF32 **)&file_contents, (UTF32 *)(file_contents + len),
                                    (UTF16 **)&converted_end, (UTF16 *)(converted_end + allocated),
                                    strictConversion);
      break;
    }

  if (conversionOK != result) panic ("conversion failure");

  write_amt = (converted_end - converted_text);
  if (write_amt != vu_write_retry (&errn, 1, converted_text, write_amt)) panic ("write failure");
  return 0;
}
