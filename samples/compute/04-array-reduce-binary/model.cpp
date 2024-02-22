//
// model.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cmath>
#include <iostream>
#include <iomanip>

#include "common.h"
#include "model.h"

///
/// @brief Create a Hashmap model executing on the GPU.
///
void Model::Initialize()
{
    // Initialize model data.
    {
        mData.GroupSums.resize(kNumWorkGroups, 0.0);
        mData.Points.resize(kNumPoints, Point{0.0, 0.0});
        double step = 1.0 / static_cast<double>(kNumPoints);
        for (size_t i = 0; i < kNumPoints; ++i) {
            double x = i * step;
            mData.Points[i].x = x;
            mData.Points[i].y = 4.0 / (1.0 + x*x);
        }
        mData.PiCpu = 0.0;
        mData.PiGpu = 0.0;
    }

    // Initialize OpenCL data.
    {
        // Create the device object and associated context and queue.
        mDevice = Compute::CreateDevice(kDeviceIndex);

        // Create the program object and associated kernels.
        mProgram = Compute::CreateProgramWithFile(mDevice, "data/pi.cl");
        mKernels.resize(NumKernels);
        mKernels[KernelPi] = Compute::CreateKernel(mProgram, "pi");
        // std::cout << mProgram.GetSource() << "\n";
        // std::cout << mProgram.GetKernelNames() << "\n";

        // Create memory buffers.
        mBuffers.resize(NumBuffers);
        mBuffers[BufferGroupSums] = Compute::CreateBuffer(
            mDevice,
            mData.GroupSums.size() * sizeof(mData.GroupSums[0]),
            CL_MEM_WRITE_ONLY);
        mBuffers[BufferPoints] = Compute::CreateBuffer(
            mDevice,
            mData.Points.size() * sizeof(mData.Points[0]),
            CL_MEM_READ_ONLY);
    }
}

///
/// @brief Cleanup the gpu model.
///
void Model::Cleanup()
{}

///
/// @brief Execute the model.
///
void Model::Run()
{
    Initialize();
    ComputeCpu();
    ComputeGpu();
    Cleanup();
}

///
/// @brief Compute the integral 4/(1+x*x) dx from 0 to 1 on the CPU.
///
void Model::ComputeCpu()
{
    // Compute the pi value.
    {
        cl_double sum = 0.0;
        for (size_t i = 0; i < mData.Points.size() - 1; ++i) {
            double del_x = mData.Points[i + 1].x - mData.Points[i].x;
            double mid_y = mData.Points[i + 1].y + mData.Points[i].y;
            sum += 0.5 * mid_y * del_x;
        }
        mData.PiCpu = sum;
    }

    // Print the computed pi value.
    {
        std::cout << std::setprecision(15)
            << " CPU " << mData.PiCpu
            << " err " << std::fabs(M_PI - mData.PiCpu)
            << "\n";
    }
}

///
/// @brief Compute the integral 4/(1+x*x) dx from 0 to 1 on the GPU.
///
void Model::ComputeGpu()
{
    // Run the pi kernel.
    {
        mBuffers[BufferPoints]->Write(&mData.Points[0]);
        mKernels[KernelPi]->SetArg(0, &mBuffers[BufferGroupSums]->id);
        mKernels[KernelPi]->SetArg(1, kWorkGroupSize * sizeof(cl_double), NULL);
        mKernels[KernelPi]->SetArg(2, &mBuffers[BufferPoints]->id);
        mKernels[KernelPi]->SetArg(3, &kNumPoints);
        mKernels[KernelPi]->SetRanges1d({kNumWorkItems}, {kWorkGroupSize});
        mKernels[KernelPi]->Run();
        mDevice->FinishQueue();
        mBuffers[BufferGroupSums]->Read(&mData.GroupSums[0]);
    }

    // Compute final integral value from the kernel partial sums.
    {
        cl_double sum = 0.0;
        for (size_t i = 0; i < kNumWorkGroups; ++i) {
            sum += mData.GroupSums[i];
        }
        mData.PiGpu = sum;
    }

    // Print the computed pi value.
    {
        std::cout << std::setprecision(15)
            << " GPU " << mData.PiGpu
            << " err " << std::fabs(M_PI - mData.PiGpu)
            << "\n";
    }
}
