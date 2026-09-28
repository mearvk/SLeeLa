CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror

all: build/sleela-audio-native

build:
	mkdir -p build

build/sleela_audio.o: ../cpp/src/sleela_audio.cpp ../cpp/include/sleela_audio.hpp | build
	$(CXX) $(CXXFLAGS) -I../cpp/include -c $< -o $@

build/sleela_audio_native.o: src/sleela_audio_native.cpp ../cpp/include/sleela_audio.hpp | build
	$(CXX) $(CXXFLAGS) -I../cpp/include -c $< -o $@

build/sleela-audio-native: build/sleela_audio.o build/sleela_audio_native.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test: all
	./build/sleela-audio-native --help

clean:
	rm -rf build

.PHONY: all test clean
