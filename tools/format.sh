#!/bin/sh

set -e

BASEDIR=$(dirname "$0")

cd $BASEDIR/..

find src -type f -regex ".*\(\.h\|\.cpp\)$" -not -wholename "*src/ui/libs/*" -exec clang-format -i {} \;
