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
do_clean() {
    [[ -d build ]] && rm -rvf build
}

do_build() {
    mkdir build
    pushd build
    cmake .. && make -j32 all
    popd
}

do_execute() {
    dir="$1"
    target="$2"
    pushd "$dir"
    run "$target"
    popd
}

do_execute_mpi() {
    dir="$1"
    target="$2"
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
    "clean")
        DO_CLEAN=true
    ;;
    "build")
        DO_CLEAN=true
        DO_BUILD=true
    ;;
    "execute")
        DO_CLEAN=true
        DO_BUILD=true
        DO_EXECUTE=true
    ;;
    *)
        DO_CLEAN=true
        die "unknown argument: $arg"
    ;;
    esac
done

[[ $DO_CLEAN == "true" ]] && do_clean
[[ $DO_BUILD == "true" ]] && do_build
if [[ $DO_EXECUTE == "true" ]]; then
    pushd build/samples
    do_execute empty                                ./empty
    do_execute math                                 ./testmath
    do_execute compute/01-clinfo                    ./01-clinfo
    do_execute compute/02-hashmap                   ./02-hashmap
    do_execute compute/03-array-reduce-single       ./03-array-reduce-single
    do_execute compute/04-array-reduce-binary       ./04-array-reduce-binary
    do_execute compute/05-pi-integral-single        ./05-pi-integral-single
    do_execute compute/06-pi-integral-binary        ./06-pi-integral-binary
    do_execute_mpi compute/07-pi-integral-mpi       ./07-pi-integral-mpi
    do_execute graphics/00-image                    ./00-image
    do_execute graphics/01-glfw                     ./01-glfw
    do_execute graphics/02-triangle                 ./02-triangle
    do_execute graphics/03-triangle-instance        ./03-triangle-instance
    do_execute graphics/04-triangle-instance        ./04-triangle-instance
    do_execute graphics/05-triangle-instance        ./05-triangle-instance
    do_execute graphics/06-quad                     ./06-quad
    do_execute graphics/07-quad-image               ./07-quad-image
    do_execute graphics/08-sphere-image             ./08-sphere-image
    do_execute graphics/09-mesh-model               ./09-mesh-model
    do_execute graphics/10-panorama                 ./10-panorama
    do_execute graphics/11-framebuffer              ./11-framebuffer
    do_execute graphics/12-iobuffer                 ./12-iobuffer
    popd
fi
