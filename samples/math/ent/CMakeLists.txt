project(ent)
add_executable(${PROJECT_NAME}
    src/chisq.c
    src/ent.c
    src/iso8859.c
    src/randtest.c
    src/iso8859.h
    src/randtest.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
