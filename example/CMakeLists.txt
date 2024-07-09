
find_package(OpenGL REQUIRED)

add_executable(winx_example_triangle external/glad.c main.c)
target_link_libraries(winx_example_triangle PRIVATE OpenGL::OpenGL winx)
set_target_properties(winx_example_triangle PROPERTIES OUTPUT_NAME "winx-triangle")
