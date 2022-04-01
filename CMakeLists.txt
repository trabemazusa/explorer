cmake_minimum_required(VERSION 3.10)

project(rigid_body)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/source/*.cpp"
    )

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw GLEW::GLEW glm::glm)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)

