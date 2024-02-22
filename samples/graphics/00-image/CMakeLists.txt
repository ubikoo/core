project(00-image)

add_executable(00-image main.cpp)

target_link_libraries(00-image PRIVATE coremath coregraphics)
target_include_directories(00-image PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external)
