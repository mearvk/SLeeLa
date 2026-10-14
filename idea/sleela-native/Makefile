# idea/sleela-native/Makefile
# Sleela-Native -- original real-time renderer + physics model + audio.
# Idea System(TM) / Idea(TM).  From Author; distributive.
# SPDX-License-Identifier: MIT (see PROVENANCE.md)
#
# This Makefile builds the GPU-free, verifiable parts: the scene model (math,
# camera, moving light, vignette params) and the audio mixer/WAV writer, via the
# headless demo. The OpenGL renderer (sln_render_gl.cpp) + loader + GLSL need a
# GL context/driver and are built on a GPU/display host (see README / the
# idea/realtime context layer).
#
#   make         build the headless demo (model + audio)
#   make run     build and run it (writes a .wav)
#   make clean

CXX      ?= c++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra
BUILD    := build
BIN      := $(BUILD)/sln-demo

.PHONY: all run clean
all: $(BIN)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/sln_audio.o: sln_audio.cpp sln_audio.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/sln_demo.o: sln_demo.cpp sln_scene.h sln_math.h sln_audio.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): $(BUILD)/sln_demo.o $(BUILD)/sln_audio.o
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(BIN)
	./$(BIN) $(BUILD)/sleela-native-demo.wav

clean:
	rm -rf $(BUILD)
