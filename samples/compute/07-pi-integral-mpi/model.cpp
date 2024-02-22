//
// model.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <iomanip>
#include <mpi.h>
#include "common.h"
#include "model.h"

///
/// @brief Create a model with an OpenCL context on the specified proc id.
///
Model::Model(const int proc_id, const int n_procs)
{
    // Initialize model data.
    {
        mData.GroupSums.resize(kNumWorkGroups, 0.0);
        mData.PiCpu = 0.0;
        mData.PiGpu = 0.0;

        cl_double delta_x = 1.0 / (cl_double) n_procs;
        mData.xlo = delta_x * (cl_double) proc_id;
        mData.xhi = mData.xlo + delta_x;

        mData.proc_id = proc_id;
        mData.n_procs = n_procs;
    }

    // Initialize OpenCL data.
    {
        // Create the device object and associated context and queue.
        mDevice = Compute::CreateDevice(kDeviceIndex);

        // Create the program object and associated kernels.
        mProgram = Compute::CreateProgramWithFile(mDevice, "data/pi.cl");
        mKernels.resize(NumKernels);
        mKernels[KernelResetPi] = Compute::CreateKernel(mProgram, "reset_pi");
        mKernels[KernelComputePi] = Compute::CreateKernel(mProgram, "compute_pi");
        // std::cout << mProgram.GetSource() << "\n";
        // std::cout << mProgram.GetKernelNames() << "\n";

        // Create memory buffers.
        mBuffers.resize(NumBuffers);
        mBuffers[BufferGroupSums] = Compute::CreateBuffer(
            mDevice,
            kNumWorkGroups * sizeof(cl_double),
            CL_MEM_WRITE_ONLY);
    }
}

///
/// @brief Run the model.
///
void Model::Run()
{
    ComputeCpu();
    ComputeGpu();
    PostProcess();
}

///
/// @brief Execute the integral 4/(1+x*x) dx from 0 to 1 on the CPU.
///
void Model::ComputeCpu()
{
    cl_ulong n_steps = kIntervalSteps * kNumIntervals;
    cl_double step_size = (mData.xhi - mData.xlo) / (cl_double) n_steps;

    cl_double sum = 0.0;
    for (cl_ulong i = 0; i < n_steps; ++i) {
        cl_double x = mData.xlo + (i + 0.5) * step_size;
        sum += 4.0 / (1.0 + x*x);
    }
    mData.PiCpu = sum * step_size;
}

///
/// @brief Execute the integral 4/(1+x*x) dx from 0 to 1 on the GPU.
///
void Model::ComputeGpu()
{
    // Reset pi partial sums.
    {
        mKernels[KernelResetPi]->SetArg(0, &mBuffers[BufferGroupSums]->id);
        mKernels[KernelResetPi]->SetArg(1, &kNumIntervals);
        mKernels[KernelResetPi]->SetRanges1d({kNumWorkItems}, {kWorkGroupSize});
        mKernels[KernelResetPi]->Run();
    }

    // Compute pi.
    {
        mKernels[KernelComputePi]->SetArg(0, &mBuffers[BufferGroupSums]->id);
        mKernels[KernelComputePi]->SetArg(1, kWorkGroupSize * sizeof(cl_double), NULL);
        mKernels[KernelComputePi]->SetArg(2, &kNumIntervals);
        mKernels[KernelComputePi]->SetArg(3, &kIntervalSteps);
        mKernels[KernelComputePi]->SetArg(4, &mData.xlo);
        mKernels[KernelComputePi]->SetArg(5, &mData.xhi);
        mKernels[KernelComputePi]->SetRanges1d({kNumWorkItems}, {kWorkGroupSize});
        mKernels[KernelComputePi]->Run();
    }

    // Finish the queue and read the partial sums back to the host.
    {
        mDevice->FinishQueue();
        mBuffers[BufferGroupSums]->Read(&mData.GroupSums[0]);
    }

    // Compute final integral value from the kernel partial sums.
    {
        cl_double sum = 0.0;
        for (cl_ulong i = 0; i < kNumWorkGroups; ++i) {
            sum += mData.GroupSums[i];
        }
        mData.PiGpu = sum;
    }
}

///
/// @brief Post process analysis.
///
void Model::PostProcess()
{
    // Receive the CPU partial sums from each process and compute pi
    if (mData.proc_id == kMasterId) {
        std::cout << "CPU\n";
        cl_double pi_sum = mData.PiCpu;
        for (int src_id = 1; src_id < mData.n_procs; ++src_id) {
            cl_double pi_partial = 0.0;
            MPI_Recv(
                &pi_partial,
                sizeof(pi_partial),
                MPI_BYTE,
                src_id,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
            pi_sum += pi_partial;

            std::cout << std::setprecision(15)
                << " recv: proc " << src_id
                << " of " << mData.n_procs
                << " pi_partial " << pi_partial
                << " pi_sum " << pi_sum
                << " err " << std::fabs(M_PI - pi_sum)
                << std::endl;
        }
    } else {
        cl_double pi_partial = mData.PiCpu;
        MPI_Send(
            (void *)&pi_partial,
            sizeof(pi_partial),
            MPI_BYTE,
            kMasterId,
            0,
            MPI_COMM_WORLD);
    }

    // Receive the GPU partial sums from each process and compute pi
    if (mData.proc_id == kMasterId) {
        std::cout << "GPU\n";
        cl_double pi_sum = mData.PiGpu;
        for (int src_id = 1; src_id < mData.n_procs; ++src_id) {
            cl_double pi_partial = 0.0;
            MPI_Recv(
                &pi_partial,
                sizeof(pi_partial),
                MPI_BYTE,
                src_id,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
            pi_sum += pi_partial;

            std::cout << std::setprecision(15)
                << " recv: proc " << src_id
                << " of " << mData.n_procs
                << " pi_partial " << pi_partial
                << " pi_sum " << pi_sum
                << " err " << std::fabs(M_PI - pi_sum)
                << std::endl;
        }
    } else {
        cl_double pi_partial = mData.PiGpu;
        MPI_Send(
            (void *)&pi_partial,
            sizeof(pi_partial),
            MPI_BYTE,
            kMasterId,
            0,
            MPI_COMM_WORLD);
    }
}
