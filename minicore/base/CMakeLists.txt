add_library(corebase STATIC
    parallel.cpp
    base.h
    error.h
    memory.h
    parallel.h)

target_include_directories(corebase PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Enable posix threads.
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
