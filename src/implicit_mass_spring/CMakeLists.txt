
file(GLOB SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    )

add_executable(implicit_mass_spring ${SRC_FILES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

target_link_libraries(implicit_mass_spring PRIVATE opengl)

