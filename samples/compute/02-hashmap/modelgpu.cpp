//
// modelgpu.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#include "common.h"
#include "modelgpu.h"

///
/// @brief Static constants.
///
static const KeyValue kEmptySlot = {kEmpty, kEmpty};
static const std::pair<uint32_t, uint32_t> kEmptyPair = {0, 0};

///
/// @brief Initialize the gpu model.
///
void ModelGpu::Initialize()
{
    // Initialize model data.
    mHashmap.resize(kCapacity, kEmptySlot);
    mKeys.resize(kNumPoints, kEmptyPair);

    // Create the device object and associated context and queue.
    mDevice = Compute::CreateDevice(kDeviceIndex);

    // Create the program object and associated kernels.
    mProgram = Compute::CreateProgramWithFile(mDevice, "data/hashmap.cl");
    mKernels.resize(NumKernels);
    mKernels[KernelHashmap] = Compute::CreateKernel(mProgram, "hashmap_insert");
    // std::cout << mProgram.GetSource() << "\n";
    // std::cout << mProgram.GetKernelNames() << "\n";

    // Create memory buffers.
    mBuffers.resize(NumBuffers);
    mBuffers[BufferHashmap] = Compute::CreateBuffer(
        mDevice,
        kCapacity * sizeof(KeyValue),
        CL_MEM_READ_WRITE);
    mBuffers[BufferPoints] = Compute::CreateBuffer(
        mDevice,
        kNumPoints * sizeof(Point),
        CL_MEM_READ_WRITE);
}

///
/// @brief Cleanup the gpu model.
///
void ModelGpu::Cleanup()
{}

///
/// @brief Run the gpu model.
///
void ModelGpu::Run(std::vector<Point> &points)
{
    // Update the hashmap data on the gpu.
    {
        std::fill(mHashmap.begin(), mHashmap.end(), kEmptySlot);
        mBuffers[BufferHashmap]->Write(&mHashmap[0]);
        mBuffers[BufferPoints]->Write(&points[0]);
    }

    // Run hashmap insert kernel.
    {
        mKernels[KernelHashmap]->SetArg(0, &mBuffers[BufferHashmap]->mId);
        mKernels[KernelHashmap]->SetArg(1, &mBuffers[BufferPoints]->mId);
        mKernels[KernelHashmap]->SetArg(2, &kCapacity);
        mKernels[KernelHashmap]->SetArg(3, &kNumPoints);
        mKernels[KernelHashmap]->SetArg(4, &kNumCells);
        mKernels[KernelHashmap]->SetArg(5, &kDomainLo);
        mKernels[KernelHashmap]->SetArg(6, &kDomainHi);
        mKernels[KernelHashmap]->SetRanges1d({kNumPoints}, {kWorkGroupSize});
        mKernels[KernelHashmap]->Run();
        mDevice->FinishQueue();
    }

    // Read the hashmap data back to the host.
    {
        mBuffers[BufferHashmap]->Read(&mHashmap[0]);
    }

    // Query the hashmap for all valid key-value pairs.
    {
        std::fill(mKeys.begin(), mKeys.end(), kEmptyPair);
        for (auto &slot : mHashmap) {
            if (slot.key != kEmpty) {
                mKeys[slot.value] = std::make_pair(
                    slot.key, slot.key % kCapacity);
            }
        }
    }
}
