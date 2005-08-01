/* cvt_opt.h: conversion options parsing
 *
 ****************************************************************
 * Copyright (C) 2004 by Jose Antonio Ortega Ruiz
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__UNI_TESTS__CVT_OPT_H
#define INCLUDE__UNI_TESTS__CVT_OPT_H


#include "hackerlab/cmd/main.h"




typedef enum
  {
    cvt_8_16 = 0,
    cvt_8_32,
    cvt_16_8,
    cvt_16_32,
    cvt_32_8,
    cvt_32_16
  }
conversion_t;




extern void cvt_option_setup (t_uchar * program_name,
                              t_uchar * usage,
                              t_uchar * version);

extern conversion_t cvt_option (int argc, char * argv[], int * non_native);


#endif /* INCLUDE__UNI_TESTS__CVT_OPT_H */


/* arch-tag: Jose Antonio Ortega Ruiz Sun Feb 29 2004 20:40:01 (cvt_opt.h)
*/
