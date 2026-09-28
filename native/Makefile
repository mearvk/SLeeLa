CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror -fPIC
all: build/sleela-audio-native
build:
	mkdir -p build
build/sleela_audio.o: src/sleela_audio.cpp include/sleela_audio.h | build
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
build/libsleela_audio.a: build/sleela_audio.o
	ar rcs $@ $<
build/sleela-audio-native: src/sleela_audio_cli.cpp build/libsleela_audio.a
	$(CXX) $(CXXFLAGS) -Iinclude $< build/libsleela_audio.a -o $@
test: all
	./build/sleela-audio-native 2>&1 | grep -q usage
clean:
	rm -rf build
.PHONY: all test clean
