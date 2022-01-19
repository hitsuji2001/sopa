#!/bin/bash

CXX="${CXX:-gcc}"
SRC_APP="src/app/*.c"
SRC_IMAGE="src/image_process/embeded_png.c"
DEBUG_FLAG="-Werror -Wall -Wextra -pedantic -ggdb -lm -lSDL2_image"
RELEASE_FLAG="-Werror -Wall -Wextra -pedantic -lm -lSDL2_image -O3"
LIBS="sdl2"
PNG_OUT="png2c"
OUT="sopa"

set -xe

process_image() {
    $CXX $SRC_IMAGE $DEBUG_FLAG -o $PNG_OUT
    if [[ $? -ne 0 ]]
    then
        echo "Can not parse image to c file"
        exit 1
    else
        $(./"$PNG_OUT" > "./header/digits.h")
        return 0
    fi
}

if [[ $# -lt 1 ]]
then
    echo "Usage: $(basename "$0") [flag]"
    exit 0
else
    if [[ "$1" == "--release" ]]
    then
        process_image
        $CXX $SRC_APP $RELEASE_FLAG -o $OUT $(pkg-config --cflags --libs $LIBS)
    elif [[ "$1" == "--debug" ]]
    then
        process_image
        $CXX $SRC_APP $DEBUG_FLAG -o $OUT $(pkg-config --cflags --libs $LIBS)
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
