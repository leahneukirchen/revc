#!/bin/sh
# tag: Tom Lord Tue Dec  4 14:54:36 2001 (unidata-tests/unit-unidata.sh)
#


set -e

echo "================ unit-unidata test ================"

rm -f ,tmp ,tmp2
echo "pruning unidata.txt"

sed -e "s/\([^;]*;\)[^;]*;\([^;]*;[^;]*;[^;]*;[^;]*;[^;]*;\)[^;]*;[^;]*;\([^;]*;\)[^;]*;[^;]*;\([^;]*;[^;]*;[^;]*\)/\1\2\3\4/" < $srcroot/hackerlab/unidata-scaffolding/unidata.txt > ,tmp

# Note:
# 
# The apparent noop invocation of `sed' here is *not*
# a noop under Cygwin -- it causes a needed line-ending 
# translation. The translation is reported to be
# caused by the `|' implentation, not `sed', but
# I'm not certain.   Perhaps `| cat' would do as well. 
#

echo "printing hackerlab db"
./unit-unidata | sed -e '' > ,tmp2

echo "comparing results"
cmp ,tmp ,tmp2

echo ...passed



