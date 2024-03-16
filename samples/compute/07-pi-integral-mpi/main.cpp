//
// main.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <mpi.h>
#include "common.h"
#include "model.h"

///
/// @brief Initialize MPI and create a model for each MPI processs. Each model
/// has its own OpenCL context and its own kernel to compute the corresponding
/// partial sum. At the end, the master process receives the partial sums and
/// computes the final result.
///
void Run(int *argc, char ***argv)
{
    // Initialize MPI context.
    MPI_Init(argc, argv);
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    int nProcs;
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

    // Create a model with associated OpenCL context and run the kernel.
    Model model(procId, nProcs);
    for (cl_ulong iter = 0; iter < kNumIters; ++iter) {
        if (procId == kMasterId) {
            std::cout << "\niter " << iter << " of " << kNumIters << "\n";
        }
        model.Run();
    }

    // Finalize MPI context.
    MPI_Finalize();
}

int main(int argc, char *argv[])
{
    try {
        Run(&argc, &argv);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
