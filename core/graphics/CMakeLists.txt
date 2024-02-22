add_library(coregraphics STATIC
    gl/buffer.cpp
    gl/error.cpp
    gl/framebuffer.cpp
    gl/renderbuffer.cpp
    gl/texture.cpp
    gl/vertexarray.cpp
    gl/buffer.h
    gl/error.h
    gl/framebuffer.h
    gl/renderbuffer.h
    gl/texture.h
    gl/vertexarray.h
    glsl/attribute.cpp
    glsl/datatype.cpp
    glsl/program.cpp
    glsl/shader.cpp
    glsl/uniform.cpp
    glsl/attribute.h
    glsl/datatype.h
    glsl/program.h
    glsl/shader.h
    glsl/uniform.h
    camera.cpp
    image.cpp
    mesh.cpp
    render.cpp
    camera.h
    image.h
    mesh.h
    render.h
    common.h
    graphics.h)

target_include_directories(coregraphics PRIVATE ${CORE_ROOT_DIR})

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
