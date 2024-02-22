#! /bin/bash

#
# run.sh
#
# Copyright (c) 2020 Carlos Braga
# This program is free software; you can redistribute it and/or modify it under
# the terms of the MIT License. See accompanying LICENSE.md or
# https://opensource.org/licenses/MIT.
#

# -------------------------------------------------------------------------------
# Die with message
die() {
    echo >&2 "$@"
    exit 1
}

# Run command and check exit code
run() {
    echo "$@" && "$@"
    code=$?
    [[ $code -ne 0 ]] && die "[$@] failed with error code $code"
    return 0
}

# Ask for input query
ask() {
    echo -n "$@ (y/n [n]): "
    local ans
    read ans
    [[ "$ans" != "y" ]] && return 1
    return 0
}

# -------------------------------------------------------------------------------
[[ $# == 0 ]] && die "usage: $0 clean | build"

DO_CLEAN=false
DO_BUILD=false
for arg in "$@"; do
    case $arg in
    clean)
        DO_CLEAN=true
    ;;
    build)
        DO_CLEAN=true
        DO_BUILD=true
    ;;
    *)
        die "unknown argument: $arg"
    ;;
    esac
done

if [[ $DO_CLEAN == "true" ]]; then
    [[ -d build ]] && rm -rvf build
fi

if [[ $DO_BUILD == "true" ]]; then
    mkdir build
    cd build
    cmake .. && make -j16 all
fi
