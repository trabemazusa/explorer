
file(GLOB SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    )

add_executable(rigid_body_simulation ${SRC_FILES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

target_link_libraries(rigid_body_simulation PRIVATE opengl)
target_compile_features(rigid_body_simulation PRIVATE cxx_std_17)

