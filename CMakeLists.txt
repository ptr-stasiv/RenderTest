cmake_minimum_required(VERSION 3.0)

project (RenderTest)

set(headers 
	"src/scene/mesh.h"
	"src/scene/render-object.h"
	"src/scene/texture.h"
	"src/scene/scene.h"
	"src/math/vectors/vector3.h"
	"src/math/vectors/vector4.h"
	"src/math/matrices/matrix4x4.h"
	"src/scene/renders/forward-render.h"
	"src/debug/gassert.h"
	"src/bgl/shader.h")

set(sources 
	"src/main.c"
	"src/scene/mesh.c"
	"src/scene/render-object.c"
	"src/scene/texture.c"
	"src/scene/scene.c"
	"src/math/vectors/vector3.c"
	"src/math/vectors/vector4.c"
	"src/math/matrices/matrix4x4.c"
	"src/scene/renders/forward-render.c"
	"src/bgl/shader.c")

set(libsInclude "extern/glew/include" "extern/glfw/include")
set(libsLink "glew32s" "glfw3" "opengl32")
set(libs "extern/glew/lib" "extern/glfw/lib")

add_executable(RenderTest ${sources} ${headers})

target_include_directories(RenderTest PRIVATE ${libsInclude})

target_link_directories(RenderTest PRIVATE ${libs})
target_link_libraries(RenderTest PRIVATE ${libsLink})

target_compile_definitions(RenderTest PRIVATE "GLEW_STATIC")

file(COPY "res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})