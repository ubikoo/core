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
# Execute project
execute() {
    dir="$1"
    target="$2"
    pushd "$dir"
    run "$target"
    popd
}

execute_mpi() {
    dir="$1"
    target="$2"
    pushd "$dir"
    run mpirun --mca shmem posix --use-hwthread-cpus -np 16 "$target"
    popd
}

# -------------------------------------------------------------------------------
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
        DO_CLEAN=true
        die "unknown argument: $arg"
    ;;
    esac
done

if [[ $DO_CLEAN == "true" ]]; then
    [[ -d build ]] && rm -rvf build
fi

if [[ $DO_BUILD == "true" ]]; then
    mkdir build
    pushd build
    cmake .. && make -j16 all
    popd
fi

if [[ $DO_EXECUTE == "true" ]]; then
    pushd build
    execute samples/math                                ./testmath

    execute samples/compute/01-clinfo                   ./01-clinfo
    execute samples/compute/02-hashmap                  ./02-hashmap
    execute samples/compute/03-array-reduce-single      ./03-array-reduce-single
    execute samples/compute/04-array-reduce-binary      ./04-array-reduce-binary
    execute samples/compute/05-pi-integral-single       ./05-pi-integral-single
    execute samples/compute/06-pi-integral-binary       ./06-pi-integral-binary
    execute_mpi samples/compute/07-pi-integral-mpi      ./07-pi-integral-mpi

    execute samples/graphics/00-image                    ./00-image
    execute samples/graphics/01-glfw                     ./01-glfw
    execute samples/graphics/02-triangle                 ./02-triangle
    execute samples/graphics/03-triangle-instance-a      ./03-triangle-instance-a
    execute samples/graphics/04-triangle-instance-b      ./04-triangle-instance-b
    execute samples/graphics/05-triangle-instance-c      ./05-triangle-instance-c
    execute samples/graphics/06-quad                     ./06-quad
    execute samples/graphics/07-quad-image               ./07-quad-image
    execute samples/graphics/08-sphere-image             ./08-sphere-image
    execute samples/graphics/09-mesh-model               ./09-mesh-model
    execute samples/graphics/10-panorama                 ./10-panorama
    execute samples/graphics/11-framebuffer              ./11-framebuffer
    execute samples/graphics/12-iobuffer                 ./12-iobuffer
    popd
fi
