#!/bin/sh
# tag: Tom Lord Tue Dec  4 14:54:36 2001 (uni-tests/unit-coding.sh)
#


set -e

arg0="$0"
srcdir=`dirname "$arg0"`

echo "================ unit-coding: UTF-8/UTF-16/UTF-32 conversion tests ================"

echo "Native byte order tests..."

./unit-cvt --8-to-16 < $srcdir/demo.utf8 > ,ref-utf16
./unit-cvt --16-to-8 < ,ref-utf16 > ,ref-utf8
./unit-cvt --8-to-32 < $srcdir/demo.utf8 > ,ref-utf32
./unit-cvt --32-to-8 < ,ref-utf32 > ,,ref-utf8
./unit-cvt --16-to-32 < ,ref-utf16 > ,,ref-utf32
./unit-cvt --32-to-16 < ,ref-utf32 > ,,ref-utf16

cmp $srcdir/demo.utf8 ,ref-utf8
cmp $srcdir/demo.utf8 ,,ref-utf8
cmp ,ref-utf32 ,,ref-utf32
cmp ,ref-utf16 ,,ref-utf16


# 
# ./unit-coding --8-to-16 < $srcdir/demo.utf8 > ,uni-utf16
# ./unit-coding --16-to-8 < ,uni-utf16 > ,uni-utf8
# ./unit-coding --8-to-32 < $srcdir/demo.utf8 > ,uni-utf32
# ./unit-coding --32-to-8 < ,ref-utf32 > ,,uni-utf8
# ./unit-coding --16-to-32 < ,ref-utf16 > ,,uni-utf32
# ./unit-coding --32-to-16 < ,ref-utf32 > ,,uni-utf16
# 
# cmp ,ref-utf16 ,uni-utf16
# cmp ,ref-utf8 ,uni-utf8
# cmp ,ref-utf32 ,uni-utf32
# cmp ,ref-utf8 ,,uni-utf8
# cmp ,ref-utf16 ,,uni-utf16
# cmp ,ref-utf32 ,,uni-utf32
# 
# ./unit-coding --32-to-8 < ,uni-utf32 > ,,uni-utf8
# ./unit-coding --16-to-32 < ,uni-utf16 > ,,uni-utf32
# ./unit-coding --32-to-16 < ,uni-utf32 > ,,uni-utf16
# 
# cmp ,ref-utf8 ,,uni-utf8
# cmp ,ref-utf16 ,,uni-utf16
# cmp ,ref-utf32 ,,uni-utf32
# 
# echo "...passed"
# 
# echo "Non-native byte order tests..."
# 
# ./unit-coding -n --8-to-16 < $srcdir/demo.utf8 > ,uni-utf16
# ./unit-coding -n --16-to-8 < ,uni-utf16 > ,uni-utf8
# ./unit-coding -n --8-to-32 < $srcdir/demo.utf8 > ,uni-utf32
# ./unit-coding -n --32-to-8 < ,ref-utf32 > ,,uni-utf8
# ./unit-coding -n --16-to-32 < ,ref-utf16 > ,,uni-utf32
# ./unit-coding -n --32-to-16 < ,ref-utf32 > ,,uni-utf16
# 
# cmp ,ref-utf16 ,uni-utf16
# cmp ,ref-utf8 ,uni-utf8
# cmp ,ref-utf32 ,uni-utf32
# cmp ,ref-utf8 ,,uni-utf8
# cmp ,ref-utf32 ,,uni-utf32
# cmp ,ref-utf16 ,,uni-utf16
# 
# ./unit-coding -n --32-to-8 < ,uni-utf32 > ,,uni-utf8
# ./unit-coding -n --16-to-32 < ,uni-utf16 > ,,uni-utf32
# ./unit-coding -n --32-to-16 < ,uni-utf32 > ,,uni-utf16
# 
# cmp ,ref-utf8 ,,uni-utf8
# cmp ,ref-utf16 ,,uni-utf16
# cmp ,ref-utf32 ,,uni-utf32
# 

echo "...passed"
 