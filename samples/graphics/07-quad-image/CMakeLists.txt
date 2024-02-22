project(07-quad-image)

add_executable(${PROJECT_NAME}
    main.cpp
    quad.cpp
    quad.h)

target_link_libraries(${PROJECT_NAME} PRIVATE coremath coregraphics)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external)

file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
