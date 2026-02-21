#!/bin/sh

set -e

BASEDIR=$(dirname "$0")

cd $BASEDIR/..

find src -type f -regex ".*\(\.h\|\.cpp\)$" -exec clang-format -i {} \;
