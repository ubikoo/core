project(testmath)
add_executable(${PROJECT_NAME}
    main.cpp
    test-algebra.cpp
    test-arithmetic.cpp
    test-matrix.cpp
    test-memory.cpp
    test-ortho.cpp
    test-random.cpp
    test-vector.cpp
    common.h
    test-algebra-matrix2.h
    test-algebra-matrix3.h
    test-algebra-matrix4.h
    test-algebra-vector2.h
    test-algebra-vector3.h
    test-algebra-vector4.h
    test-arithmetic2.h
    test-arithmetic3.h
    test-arithmetic4.h
    test-matrix2.h
    test-matrix3.h
    test-matrix4.h
    test-memory.h
    test-ortho.h
    test-vector2.h
    test-vector3.h
    test-vector4.h)

target_link_libraries(${PROJECT_NAME} PRIVATE coremath)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external)

file(COPY plot DESTINATION ${PROJECT_BINARY_DIR})
file(COPY testmath.sh DESTINATION ${PROJECT_BINARY_DIR})
