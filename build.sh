#!/bin/bash

PRJDIR="$(dirname "$(realpath "$0")")"
mkdir "$PRJDIR/build"
(cd "$PRJDIR/build" ; cmake .. ; cmake --build .)
