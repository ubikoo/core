//
// parallel.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef BASE_PARALLEL_H_
#define BASE_PARALLEL_H_

namespace Base {

///
/// Threadpool maintains a group of threads and a work queue. If the work queue
/// is empty, the threads sleep until a new work item is added to it.
/// https://stackoverflow.com/questions/6954489/how-to-utilize-a-thread-pool-with-pthreads
///
struct ThreadPool {
    struct Work {
        void (*run) (void *);
        void *data;
    };
    static size_t NumThreads();
    static void Initialize(const uint32_t numThreads);
    static void Terminate();
    static void *Execute(void *arg);
    static void Enqueue(void (*func) (void *), void *data);
    static void Wait();
};

/// @brief Parallel for loop over an array of items.
void ParallelFor(void (*run) (size_t, void *), const size_t count, void *data);

} // namespace Base

#endif // BASE_PARALLEL_H_
