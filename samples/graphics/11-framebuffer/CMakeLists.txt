project(11-framebuffer)

add_executable(${PROJECT_NAME}
    main.cpp
    drawable.cpp
    drawable.h)

target_link_libraries(${PROJECT_NAME} PRIVATE coremath coregraphics)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
