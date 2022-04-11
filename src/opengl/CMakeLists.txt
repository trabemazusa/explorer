
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

set(OPENGL_SRCS
   window.cpp
   renderer.cpp
   shader.cpp
   shape.cpp
)

set(OPENGL_HDRS
   window.h
   renderer.h
   shader.h
   shape.h
)

add_library(opengl ${OPENGL_SRCS} ${OPENGL_HDRS})
target_link_libraries(opengl PUBLIC glfw GLEW::GLEW glm::glm)
target_compile_features(opengl PRIVATE cxx_std_17)
