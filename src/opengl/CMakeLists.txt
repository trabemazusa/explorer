
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
target_link_libraries(opengl PUBLIC glfw glad glm::glm)
target_compile_features(opengl PRIVATE cxx_std_17)
