#!/bin/sh 
# tag: Christian Thaeter (char-tests/unit-pika-escaping.sh)
#

set -e

arg0="$0"
srcdir=`dirname "$arg0"`

echo ================ unit-pika-escaping tests ================
./unit-pika-escaping

echo ...passed
