#! /bin/bash

# run.sh
#
# Copyright (c) 2020 Carlos Braga
# This program is free software; you can redistribute it and/or modify it under
# the terms of the MIT License. See accompanying LICENSE.md or
# https://opensource.org/licenses/MIT.

# -----------------------------------------------------------------------------
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

# -----------------------------------------------------------------------------
do_clean() {
    [[ -d build ]] && rm -rvf build
}

do_build() {
    mkdir build
    pushd build
    cmake -DCMAKE_BUILD_TYPE="Release" .. && make -j16 all
    popd
}

do_execute() {
    dir="$1"
    target="./$1"
    pushd "$dir"
    run "$target"
    popd
}

do_execute_mpi() {
    dir="$1"
    target="./$1"
    pushd "$dir"
    run mpirun --mca shmem posix --use-hwthread-cpus -np 16 "$target"
    popd
}

[[ $# == 0 ]] && die "usage: $0 clean | build | execute"

DO_CLEAN=false
DO_BUILD=false
DO_EXECUTE=false
for arg in "$@"; do
    case $arg in
    clean)
        DO_CLEAN=true
    ;;
    build)
        DO_CLEAN=true
        DO_BUILD=true
    ;;
    execute)
        DO_CLEAN=true
        DO_BUILD=true
        DO_EXECUTE=true
    ;;
    *)
        die "unknown argument: $arg"
    ;;
    esac
done

[[ $DO_CLEAN == "true" ]] && do_clean
[[ $DO_BUILD == "true" ]] && do_build
if [[ $DO_EXECUTE == "true" ]]; then
    pushd build/samples
    do_execute empty
    popd

    pushd build/samples/math
    ./testmath.sh
    popd

    pushd build/samples/compute
    do_execute      01-clinfo
    do_execute      02-hashmap
    do_execute      03-array-reduce-single
    do_execute      04-array-reduce-binary
    do_execute      05-pi-integral-single
    do_execute      06-pi-integral-binary
    do_execute_mpi  07-pi-integral-mpi
    popd

    pushd build/samples/graphics
    do_execute      00-image
    do_execute      01-glfw
    do_execute      02-triangle
    do_execute      03-triangle-instance
    do_execute      04-triangle-instance
    do_execute      05-triangle-instance
    do_execute      06-quad
    do_execute      07-quad-image
    do_execute      08-sphere-image
    do_execute      09-mesh-model
    do_execute      10-panorama
    do_execute      11-framebuffer
    do_execute      12-iobuffer
    popd
fi
