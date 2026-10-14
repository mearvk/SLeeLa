# idea/realtime/CMakeLists.txt
# Idea System(TM) realtime canvas.
# Idea System(TM) / Idea(TM).  From Author; distributive.
#
# Builds the realtime canvas module.  By default only the graphics-independent
# physics core (OpenMP + SIMD) is built -- that part compiles anywhere.  Turn
# on the OpenGL render layer with:
#
#     cmake -S . -B build -DIDEA_CANVAS_WITH_GL=ON
#     cmake --build build
#
# The GL layer requires GLFW and an OpenGL loader (glad or GLEW) and a GPU +
# display.  It is NOT built or verified in the headless Idea System sandbox.

cmake_minimum_required(VERSION 3.16)
project(idea_canvas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(IDEA_CANVAS_WITH_GL "Build the GLFW + OpenGL render layer" OFF)

find_package(OpenMP REQUIRED)

# ---- the always-built physics core ----
set(CORE_SRC idea_canvas_sim.cpp)

if(IDEA_CANVAS_WITH_GL)
    list(APPEND CORE_SRC idea_canvas_gl.cpp)
else()
    list(APPEND CORE_SRC idea_canvas_gl_stub.cpp)
endif()

add_library(ideacanvas STATIC ${CORE_SRC})
target_include_directories(ideacanvas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ideacanvas PUBLIC OpenMP::OpenMP_CXX)

if(IDEA_CANVAS_WITH_GL)
    target_compile_definitions(ideacanvas PUBLIC IDEA_CANVAS_WITH_GL)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(OpenGL REQUIRED)
    # Provide your GL loader (glad/GLEW). Example assumes a `glad` target:
    #   add_subdirectory(third_party/glad) ; then link `glad` below.
    target_link_libraries(ideacanvas PUBLIC glfw OpenGL::GL)
    # target_link_libraries(ideacanvas PUBLIC glad)   # uncomment with your loader
endif()

# ---- the physics-core benchmark (headless) ----
add_executable(idea-canvas-bench idea_canvas_bench.cpp)
target_link_libraries(idea-canvas-bench PRIVATE ideacanvas OpenMP::OpenMP_CXX)
