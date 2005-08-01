#!/bin/sh 


set -e

arg0="$0"
srcdir=`dirname "$arg0"`

echo ================ unit-md5 tests ================
./unit-md5

echo ...passed

# tag: Tom Lord Fri Oct 24 09:25:04 2003 (hash-tests/unit-md5.sh)
#
