project(03-triangle-instance-a)

add_executable(${PROJECT_NAME}
    main.cpp
    triangle.cpp
    triangle.h)

target_link_libraries(${PROJECT_NAME} PRIVATE coremath coregraphics)
target_include_directories(${PROJECT_NAME} PRIVATE ${CORE_ROOT_DIR})

file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
