
file(GLOB SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    )

add_executable(pbd ${SRC_FILES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

target_link_libraries(pbd PRIVATE opengl)
target_compile_features(pbd PRIVATE cxx_std_17)
