# This is required to be defined by external callers!
message(STATUS "CORE_ROOT_DIR - ${CORE_ROOT_DIR}")
if(NOT DEFINED CORE_ROOT_DIR)
	message(FATAL_ERROR "CORE_ROOT_DIR variable wasn't set.")
endif()

add_subdirectory(math)
add_subdirectory(compute)
add_subdirectory(graphics)
