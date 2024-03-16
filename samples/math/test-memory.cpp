//
// test-memory.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cassert>
#include <stddef.h>
#include "test-memory.h"

static const size_t NumArrays = 16;
static const size_t ArraySize = 1024;

/// ---- Memory test structure ------------------------------------------------
///
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
        m_arr = math::align_array_alloc<size_t>(m_num, m_value);
    }
    ~X() {
        math::align_array_free<size_t>(m_arr, m_num);
    }

    // Disable copy constructor/assignment operators
    X(const X &other) = delete;
    X &operator=(const X &other) = delete;
};

struct XPtrDeleter {
    void operator()(X *ptr) const {
        if (ptr) {
            math::align_free(ptr);
        }
    }
};

using XUniquePtr = std::unique_ptr<X, XPtrDeleter>;

// ---- Memory ----------------------------------------------------------------
///
void test_core_memory()
{
    // Test align_unique_ptr
    {
        XPtrDeleter del;
        std::vector<XUniquePtr> vector_data;
        for (size_t i = 0; i < NumArrays; ++i) {
            XUniquePtr ptr(math::align_alloc<X>(ArraySize, 1), del);
            vector_data.push_back(std::move(ptr));
        }

        for (auto &x : vector_data) {
            assert(x->check());
        }

        std::cout << "vector_data size before clear: "
            << vector_data.size() << "\n";
        assert(vector_data.size() == NumArrays);
        vector_data.clear();
        std::cout << "vector_data size after clear: "
            << vector_data.size() << "\n";
        assert(vector_data.size() == 0);
    }
}
