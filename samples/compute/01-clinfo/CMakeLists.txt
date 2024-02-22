project(01-clinfo)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE coremath corecompute)
target_include_directories(${PROJECT_NAME} PRIVATE ${CORE_ROOT_DIR})

# file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
