# Use glfw library
option(CORE_USE_GLFW "Use glfw library" ON)
if(CORE_USE_GLFW)
  set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)

  mark_as_advanced(
      GLFW_INSTALL
      GLFW_BUILD_DOCS
      GLFW_BUILD_TESTS
      GLFW_BUILD_EXAMPLESz
      GLFW_BUILD_COCOA
      GLFW_LIBRARY_TYPE)

  add_subdirectory(glfw)
  add_subdirectory(glad)
endif(CORE_USE_GLFW)

# Use glm library
option(CORE_USE_GLM "Use glm library" OFF)
if(CORE_USE_GLM)
    add_subdirectory(glm/glm)
endif(CORE_USE_GLM)
