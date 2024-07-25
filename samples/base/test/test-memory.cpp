//
// test-memory.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "external/catch2/catch.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include "test-memory.h"

static const size_t NumArrays = 16;
static const size_t ArraySize = 1024;

/// -----------------------------------------------------------------------------
struct X {
    size_t m_num = 0;
    size_t m_value = 0;
    size_t *m_arr;

    int get() const { return m_num; }
    void set(int num) { m_num = num; }

    bool check(void) const {
        bool ok = true;
        for (size_t i = 0; i < m_num; ++i) {
            ok &= m_arr[i] == m_value;
        }
        return ok;
    }

    // Constructor and destructor.
    X(size_t num, size_t value) {
        m_num = num;
        m_value = value;
        m_arr = Base::AlignArrayAlloc<size_t>(m_num, m_value);
    }
    ~X() {
        Base::AlignArrayFree<size_t>(m_arr, m_num);
    }

    // Disable copy constructor/assignment operators
    X(const X &other) = delete;
    X &operator=(const X &other) = delete;
};

struct XPtrDeleter {
    void operator()(X *ptr) const {
        if (ptr) {
            Base::AlignFree(ptr);
        }
    }
};

using XUniquePtr = std::unique_ptr<X, XPtrDeleter>;

/// -----------------------------------------------------------------------------
void test_base_memory()
{
    // Test align_unique_ptr
    {
        XPtrDeleter del;
        std::vector<XUniquePtr> data;
        for (size_t i = 0; i < NumArrays; ++i) {
            XUniquePtr ptr(Base::AlignAlloc<X>(ArraySize, 1), del);
            data.push_back(std::move(ptr));
        }

        for (auto &x : data) {
            REQUIRE(x->check());
        }

        std::cout << "data size before clear: " << data.size() << "\n";
        REQUIRE(data.size() == NumArrays);
        data.clear();
        std::cout << "data size after clear: " << data.size() << "\n";
        REQUIRE(data.size() == 0);
    }
}

/// -----------------------------------------------------------------------------
TEST_CASE("BaseMemory") {
    test_base_memory();
}
