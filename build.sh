#!/bin/bash

CXX="${CXX:-gcc}"
SRC="src/*.c"
FLAG="-Werror -Wall -Wextra -pedantic -ggdb -lm"
LIBS="sdl2"
OUT="sopa"

set -xe

$CXX $SRC $FLAG -o $OUT $(pkg-config --cflags --libs $LIBS)

if [[ $? -ne 0 ]]
then
    echo "Compile failed"
    exit 1
else
    ./$OUT $@
    exit 0
fi
