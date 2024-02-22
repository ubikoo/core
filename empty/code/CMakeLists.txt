add_executable(empty main.cpp)
target_link_libraries(empty PRIVATE coremath coregraphics corecompute)
target_include_directories(empty PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external)

file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
