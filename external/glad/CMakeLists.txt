cmake_minimum_required(VERSION 3.2)

project(glad)

find_package(OpenGL REQUIRED)

add_library(glad STATIC src/glad.c)

target_include_directories(glad PUBLIC include)
target_link_libraries(glad PUBLIC ${OPENGL_gl_LIBRARY})

