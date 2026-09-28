CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror
all: build/libsleela_audio_cpp.a
build:; mkdir -p build
build/sleela_audio.o: src/sleela_audio.cpp include/sleela_audio.hpp | build
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
build/libsleela_audio_cpp.a: build/sleela_audio.o
	ar rcs $@ $<
test: all
	@echo "C++ audio library built"
clean:; rm -rf build
.PHONY: all test clean
