#!/bin/bash

CXX="${CXX:-gcc}"
SRC="*.c"
FLAG="-Werror -Wall -Wextra -pedantic -ggdb -lm"
LIBS="sdl2"
OUT="sopa"

set -xe

$CXX $SRC $FLAG `pkg-config --cflags $LIBS` -o $OUT `pkg-config --libs $LIBS`

if [[ $? -ne 0 ]]
then
    echo "Compile failed"
    exit 1
else
    ./$OUT $@
    exit 0
fi
