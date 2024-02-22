//
// modelgpu.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MODELGPU_H_
#define MODELGPU_H_

#include <vector>
#include <utility>

#include "common.h"

struct ModelGpu {
    std::vector<KeyValue> mHashmap;
    std::vector<std::pair<uint32_t, uint32_t>> mKeys;
    const std::pair<uint32_t, uint32_t> &key(size_t i) const {
        return mKeys[i];
    }

    Compute::Device mDevice;
    Compute::Program mProgram;
    enum {
        KernelHashmap = 0,
        NumKernels
    };
    std::vector<Compute::Kernel> mKernels;
    enum {
        BufferHashmap = 0,
        BufferPoints,
        NumBuffers,
    };
    std::vector<Compute::Buffer> mBuffers;
    enum {
        NumImages = 0
    };
    std::vector<Compute::Image> mImages;

    void Initialize();
    void Cleanup();
    void Run(std::vector<Point> &points);
};

#endif // MODELGPU_H_
