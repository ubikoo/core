add_library(coregraphics STATIC
    attribute.cpp
    buffer.cpp
    camera.cpp
    framebuffer.cpp
    graphics.cpp
    helpers.cpp
    image.cpp
    mesh.cpp
    pipeline.cpp
    program.cpp
    renderbuffer.cpp
    texture.cpp
    uniform.cpp
    vertexarray.cpp
    attribute.h
    buffer.h
    camera.h
    common.h
    framebuffer.h
    graphics.h
    helpers.h
    image.h
    mesh.h
    pipeline.h
    program.h
    renderbuffer.h
    texture.h
    uniform.h
    vertexarray.h)

target_include_directories(coregraphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Enable math compile options.
find_library(MATH_LIBRARY m)
mark_as_advanced(MATH_LIBRARY)
if(MATH_LIBRARY)
    target_link_libraries(coregraphics PUBLIC ${MATH_LIBRARY})
endif(MATH_LIBRARY)

if(WIN32)
    target_compile_definitions(coregraphics PUBLIC _USE_MATH_DEFINES)
endif(WIN32)

# Enable OpenGL compile options.
find_package(OpenGL REQUIRED)
target_link_libraries(coregraphics PUBLIC OpenGL::GL glad glfw ${GLFW_LIBRARIES})
target_include_directories(coregraphics PUBLIC ${OPENGL_INCLUDE_DIR})
