CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror -fPIC
all: build/sleela-audio-native build/sleela-opcode-native build/sleela-gov-native build/sleela-sldocument-native
build:
	mkdir -p build
build/sleela_audio.o: src/sleela_audio.cpp include/sleela_audio.h | build
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
build/libsleela_audio.a: build/sleela_audio.o
	ar rcs $@ $<
build/sleela-audio-native: src/sleela_audio_cli.cpp build/libsleela_audio.a
	$(CXX) $(CXXFLAGS) -Iinclude $< build/libsleela_audio.a -o $@
build/sleela_opcode.o: src/sleela_opcode.cpp include/sleela_opcode.h | build
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
build/libsleela_opcode.a: build/sleela_opcode.o
	ar rcs $@ $<
build/sleela-opcode-native: src/sleela_opcode_cli.cpp build/libsleela_opcode.a
	$(CXX) $(CXXFLAGS) -Iinclude $< build/libsleela_opcode.a -o $@
build/sleela_gov.o: src/sleela_gov.cpp include/sleela_gov.h | build
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
build/libsleela_gov.a: build/sleela_gov.o
	ar rcs $@ $<
build/sleela-gov-native: src/sleela_gov_cli.cpp build/libsleela_gov.a
	$(CXX) $(CXXFLAGS) -Iinclude $< build/libsleela_gov.a -o $@
build/sleela_sldocument.o: src/sleela_sldocument.cpp include/sleela_sldocument.h | build
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
build/libsleela_sldocument.a: build/sleela_sldocument.o
	ar rcs $@ $<
build/sleela-sldocument-native: src/sleela_sldocument_cli.cpp build/libsleela_sldocument.a
	$(CXX) $(CXXFLAGS) -Iinclude $< build/libsleela_sldocument.a -o $@
test: all
	./build/sleela-audio-native 2>&1 | grep -q usage
	./build/sleela-opcode-native
	./build/sleela-gov-native
	./build/sleela-sldocument-native
clean:
	rm -rf build
.PHONY: all test clean
