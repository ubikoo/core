//
// parallel.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <queue>
#include <vector>
#include <unordered_map>
#include <pthread.h>
#include "parallel.h"

namespace Base {

///
/// @brief Parallel chunk objects.
///
static bool gTerminate;
static size_t gWorkCount;
static pthread_mutex_t gWorkLock;
static pthread_mutex_t gQueueLock;
static pthread_cond_t gQueueHasWork;
static pthread_cond_t gWorkFinished;
static std::vector<pthread_t> gWorkThreads;
static std::unordered_map<pthread_t, size_t> gWorkThreadId;
static std::queue<ThreadPool::Work> gWorkQueue;

///
/// @brief Initialize the thread pool with a specified number of threads using
/// their default attributes.
///
void ThreadPool::Initialize(const uint32_t numThreads)
{
    gTerminate = false;
    gWorkCount = 0;
    pthread_mutex_init(&gWorkLock, NULL);
    pthread_mutex_init(&gQueueLock, NULL);
    pthread_cond_init(&gQueueHasWork, NULL);
    pthread_cond_init(&gWorkFinished, NULL);
    gWorkThreads.resize(numThreads);
    for (size_t id = 0; id < numThreads; ++id) {
        pthread_create(&gWorkThreads[id], NULL, Execute, NULL);
        gWorkThreadId[gWorkThreads[id]] = id;
    }
}

///
/// @brief Destroy the thread pool and terminate all threads. Set terminate flag
/// and wake up any threads so they can terminate.
///
void ThreadPool::Terminate()
{
    pthread_mutex_lock(&gQueueLock);
    gTerminate = true;
    pthread_cond_broadcast(&gQueueHasWork);
    pthread_mutex_unlock(&gQueueLock);
    for (auto &thread : gWorkThreads) {
        pthread_join(thread, NULL);
    }
}

///
/// @brief Thread main loop. If queue is empty, sleep and wait for the condition
/// signalling there is a new work item in the queue. Check first if terminate
/// flag is set and exit if needed. Otherwise, pop the work item from the queue
/// and run it.
/// After work item finishes, update the work count. If it reaches zero, then all
/// running jobs have finished. Send a signal to wake the thread waiting on the
/// work finished condition in ThreadPool::Wait so it can continue.
///
void *ThreadPool::Execute(void *arg)
{
    while (true) {
        Work work;

        // Sleep until the condition there is a new work item in the queue.
        pthread_mutex_lock(&gQueueLock);
        while (!gTerminate && gWorkQueue.empty()) {
            pthread_cond_wait(&gQueueHasWork, &gQueueLock);
        }

        if (gTerminate) {
            pthread_mutex_unlock(&gQueueLock);
            pthread_exit(NULL);
        }

        work = gWorkQueue.front();
        gWorkQueue.pop();
        pthread_mutex_unlock(&gQueueLock);

        // Run the work item and signal the waiting thread if finished.
        work.run(work.data);
        pthread_mutex_lock(&gWorkLock);
        gWorkCount--;
        if (gWorkCount == 0) {
            pthread_cond_signal(&gWorkFinished);
        }
        pthread_mutex_unlock(&gWorkLock);
    }
}

///
/// @brief Add a new item to the work queue and signal the condition queue has
/// work to awake a thread and execute the work. Call pthread_cond_broadcast
/// nstead of pthread_cond_signal because there is more than one thread in a
/// blocking wait state.
///
void ThreadPool::Enqueue(void (*run) (void *), void *data)
{
    pthread_mutex_lock(&gWorkLock);
    gWorkCount++;
    pthread_mutex_unlock(&gWorkLock);

    pthread_mutex_lock(&gQueueLock);
    gWorkQueue.push({run, data});
    pthread_cond_broadcast(&gQueueHasWork);
    pthread_mutex_unlock(&gQueueLock);
}

///
/// @brief Wait until all currently work items finish, ie while work count is
/// larger than zero.
///
void ThreadPool::Wait()
{
    pthread_mutex_lock(&gWorkLock);
    while (gWorkCount > 0) {
        pthread_cond_wait(&gWorkFinished, &gWorkLock);
    }
    pthread_mutex_unlock(&gWorkLock);
}

///
/// @brief Return the number of threads in the pool.
///
size_t ThreadPool::GetNumThreads()
{
    return gWorkThreads.size();
}

///
/// @brief Return the number of threads in the pool.
///
size_t ThreadPool::GetThreadId()
{
    pthread_t thread = pthread_self();
    return gWorkThreadId[thread];
}

///
/// @brief Return the number of threads in the pool.
///
size_t ThreadPool::RoundUp(size_t count)
{
    size_t multiple = GetNumThreads();
    return ((count + multiple - 1) / multiple) * multiple;
}

///
/// @brief Parallel for loop over an array of items.
/// @param run is a pointer to a function of each item in the array.
/// @param count is the number of work items.
/// @param data is the data for each work item.
///
struct ParallelChunk {
    size_t begin;
    size_t end;
    void (*run) (size_t, void *);
    void *data;
};

void ParallelFor(void (*run) (size_t, void *), const size_t count, void *data)
{
    // Enqueue each chunk to the thread pool and wait for all threads to finish.
    size_t numThreads = ThreadPool::GetNumThreads();
    size_t chunkSize = (count + numThreads - 1) / numThreads;

    std::vector<ParallelChunk> chunks(numThreads);
    for (size_t i = 0; i < numThreads; ++i) {
        chunks[i].begin = std::min(count, i * chunkSize);
        chunks[i].end = std::min(count, (i + 1) * chunkSize);
        chunks[i].run = run;
        chunks[i].data = data;
    }

    for (size_t i = 0; i < numThreads; ++i) {
        ThreadPool::Enqueue(
            [](void *data) {
                ParallelChunk *chunk = static_cast<ParallelChunk *>(data);
                for (size_t id = chunk->begin; id < chunk->end; ++id) {
                    chunk->run(id, chunk->data);
                }
            },
            (void *) &chunks[i]
        );
    }
    ThreadPool::Wait();
}

} // namespace Base
