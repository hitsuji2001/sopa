#!/bin/bash

CXX="${CXX:-gcc}"
SRC="src/*.c"
DEBUG_FLAG="-Werror -Wall -Wextra -pedantic -ggdb -lm -lSDL2_image"
RELEASE_FLAG="-Werror -Wall -Wextra -pedantic -lm -lSDL2_image -O3"
LIBS="sdl2"
OUT="sopa"

set -xe

if [[ $# -lt 1 ]]
then
    echo "Usage: $(basename "$0") [flag]"
    exit 0
else
    if [[ "$1" == "--release" ]]
    then
        $CXX $SRC $RELEASE_FLAG -o $OUT $(pkg-config --cflags --libs $LIBS)
    elif [[ "$1" == "--debug" ]]
    then
        $CXX $SRC $DEBUG_FLAG -o $OUT $(pkg-config --cflags --libs $LIBS)
    else
        echo "There are only 2 flags: '--release' and '--debug'"
        exit 1
    fi
fi

if [[ $? -ne 0 ]]
then
    echo "Compile failed"
    exit 1
else
    echo "Compile succesfully"
    exit 0
fi
