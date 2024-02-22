//
// modelcpu.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MODELCPU_H_
#define MODELCPU_H_

#include <vector>
#include <utility>

#include "common.h"
#include "hashmap.h"

struct ModelCpu {
    Hashmap mHashmap;
    std::vector<std::pair<uint32_t, uint32_t>> mKeys;
    const std::pair<uint32_t, uint32_t> &key(size_t i) const {
        return mKeys[i];
    }

    void Initialize();
    void Cleanup();
    void Run(std::vector<Point> &points);
};

#endif // MODELCPU_H_
