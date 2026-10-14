# idea/realtime/Makefile
# Idea System(TM) realtime canvas -- physics core (headless-verifiable).
# Idea System(TM) / Idea(TM).  From Author; distributive.
#
# This Makefile builds ONLY the graphics-independent physics core + the no-GL
# stub + the benchmark, so it compiles and runs anywhere a C++20 + OpenMP
# compiler exists (no GLFW/OpenGL/display required).
#
# For the OpenGL render layer, use CMake with -DIDEA_CANVAS_WITH_GL=ON
# (see CMakeLists.txt / README.md).  That path needs GLFW + an OpenGL loader
# and a GPU/display and is NOT built here.
#
#   make           build the static core + benchmark
#   make run       build and run the benchmark
#   make clean

CXX      ?= c++
CXXFLAGS ?= -std=c++20 -O3 -Wall -Wextra -fopenmp
AR       ?= ar
BUILD    := build

LIB := $(BUILD)/libideacanvas.a
BIN := $(BUILD)/idea-canvas-bench

.PHONY: all run clean
all: $(BIN)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/idea_canvas_sim.o: idea_canvas_sim.cpp idea_canvas.h idea_canvas_internal.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/idea_canvas_gl_stub.o: idea_canvas_gl_stub.cpp idea_canvas.h idea_canvas_internal.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# GPU backend compiled in its no-GL stub form here (GPU path needs EGL/OSMesa).
$(BUILD)/idea_canvas_gpu.o: idea_canvas_gpu.cpp idea_canvas.h idea_canvas_internal.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB): $(BUILD)/idea_canvas_sim.o $(BUILD)/idea_canvas_gl_stub.o $(BUILD)/idea_canvas_gpu.o
	$(AR) rcs $@ $^

$(BUILD)/idea_canvas_bench.o: idea_canvas_bench.cpp idea_canvas.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): $(BUILD)/idea_canvas_bench.o $(LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(BUILD)
