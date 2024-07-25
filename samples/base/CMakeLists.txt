project(samplesbase)
add_subdirectory(test)
file(COPY test.sh DESTINATION ${PROJECT_BINARY_DIR})
