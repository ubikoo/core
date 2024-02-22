project(02-hashmap)

add_executable(${PROJECT_NAME}
    hashmap.cpp
    main.cpp
    modelcpu.cpp
    modelgpu.cpp
    common.h
    hashmap.h
    modelcpu.h
    modelgpu.h)

target_link_libraries(${PROJECT_NAME} PRIVATE coremath corecompute)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external)

file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
