project(08-sphere-image)

add_executable(${PROJECT_NAME}
    main.cpp
    sphere.cpp
    sphere.h)

target_link_libraries(${PROJECT_NAME} PRIVATE coremath coregraphics)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
