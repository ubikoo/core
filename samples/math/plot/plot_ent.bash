#! /bin/bash

#
# plot_ent_compute.sh
#
# Copyright (c) 2020 Carlos Braga
# This program is free software; you can redistribute it and/or modify
# it under the terms of the MIT License.
# See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
#

# Number of output runs
NUMRUNS=2048
NTHREADS=64

# -------------------------------------------------------------------------------
# Compile the ent executable
TMPDIR="/tmp"
ENT="../../external/ent/ent"

# Create the output plot files
RANDOM32_PLOT="$TMPDIR/out.random32_plot"
RANDOM64_PLOT="$TMPDIR/out.random64_plot"
echo > $RANDOM32_PLOT
echo > $RANDOM64_PLOT

#for ((i = 0; i < NUMRUNS; i++)); do
    #random32="$TMPDIR/out.random32.$i"
    #random64="$TMPDIR/out.random64.$i"

    #$ENT $random32 | awk 'NR == 7 {print $5}' >> $RANDOM32_PLOT  &
    #$ENT $random64 | awk 'NR == 7 {print $5}' >> $RANDOM64_PLOT  &

    #x0=$(tail -n 1 $RANDOM32_PLOT)
    #x1=$(tail -n 1 $RANDOM64_PLOT)
    #echo $i ": " $x0 $x1
#done

thread_fun() {
    local random32="$TMPDIR/out.random32.$1"
    local random64="$TMPDIR/out.random64.$1"

    local ix=$(($1%NTHREADS))

    $ENT $random32 | awk 'NR == 7 {print $5}' >> $RANDOM32_PLOT
    $ENT $random64 | awk 'NR == 7 {print $5}' >> $RANDOM64_PLOT

    x0=$(tail -n 1 $RANDOM32_PLOT)
    x1=$(tail -n 1 $RANDOM64_PLOT)

    echo $1 "/" $ix ": " $x0 $x1
}

for (( i = 0; i < $((NUMRUNS - NUMRUNS % NTHREADS)); i+=$NTHREADS )); do
    for (( j = 0; j < $((NTHREADS-1)); j++ )); do
        thread_fun $((i+j)) &
    done
    thread_fun $((i+NTHREADS-1)) && wait
done

for (( i = $((NUMRUNS - NUMRUNS % NTHREADS)); i < $NUMRUNS; i+=1 )); do
    thread_fun $((i))
done

echo "$TMPDIR/out.random32.long" && $ENT "$TMPDIR/out.random32.long"
echo "$TMPDIR/out.random64.long" && $ENT "$TMPDIR/out.random64.long"
