project(testbase)
add_executable(${PROJECT_NAME}
    main.cpp
    test-memory.cpp
    test-parallel.cpp
    test-memory.h
    test-parallel.h)

target_link_libraries(${PROJECT_NAME} PRIVATE corebase)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

# file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
