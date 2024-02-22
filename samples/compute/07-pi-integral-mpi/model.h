//
// model.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include "common.h"

struct Model {
    // Model data
    struct Data {
        std::vector<cl_double> GroupSums;
        cl_double PiCpu;
        cl_double PiGpu;
        cl_double xlo;
        cl_double xhi;
        cl_int proc_id;
        cl_int n_procs;
    } mData;

    // OpenCL data
    Compute::Device mDevice;
    Compute::Program mProgram;
    enum {
        KernelResetPi = 0,
        KernelComputePi,
        NumKernels
    };
    std::vector<Compute::Kernel> mKernels;
    enum {
        BufferGroupSums = 0,
        NumBuffers
    };
    std::vector<Compute::Buffer> mBuffers;
    enum {
        NumImages = 0
    };
    std::vector<Compute::Image> mImages;

    void Run();
    void ComputeCpu();
    void ComputeGpu();
    void PostProcess();

    explicit Model(const int proc_id, const int n_procs);
    ~Model() = default;
};

#endif // MODEL_H_
