add_library(corecompute STATIC
    device.cpp
    event.cpp
    helpers.cpp
    interop.cpp
    kernel.cpp
    memory.cpp
    ndrange.cpp
    program.cpp
    sampler.cpp
    common.h
    compute.h
    device.h
    event.h
    helpers.h
    interop.h
    kernel.h
    math.h
    memory.h
    ndrange.h
    program.h
    sampler.h)

target_include_directories(corecompute PRIVATE ${CORE_ROOT_DIR})

# Enable math compile options.
find_library(MATH_LIBRARY m)
mark_as_advanced(MATH_LIBRARY)
if(MATH_LIBRARY)
    target_link_libraries(corecompute PUBLIC ${MATH_LIBRARY})
endif(MATH_LIBRARY)

if(WIN32)
    target_compile_definitions(corecompute PUBLIC _USE_MATH_DEFINES)
endif(WIN32)

# Enable OpenCL compile options.
find_package(OpenCL REQUIRED)
target_link_libraries(corecompute PUBLIC OpenCL::OpenCL)
target_include_directories(corecompute PUBLIC ${OpenCL_INCLUDE_DIRS})

# Enable OpenCL/OpenGL interop compile options.
option(ENABLE_OPENCL_INTEROP "Enable OpenCL/OpenGL interoperability" ON)
if(ENABLE_OPENCL_INTEROP)
    find_package(OpenGL REQUIRED)
    target_link_libraries(corecompute PUBLIC OpenGL::GL glad glfw ${GLFW_LIBRARIES})
    target_include_directories(corecompute PUBLIC ${OPENGL_INCLUDE_DIR})
    target_compile_definitions(corecompute PUBLIC ENABLE_OPENCL_INTEROP)
endif(ENABLE_OPENCL_INTEROP)
