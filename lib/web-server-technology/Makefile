# lib/web-server-technology native library:
# C11 core (slweb) + C++17 RAII facade + C/C++ smoke tests.
# Mirrors the lib/sleela-sql build layout.
CC ?= cc
CXX ?= c++
AR ?= ar
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -I native/include -I cpp/include
BUILD := build
CORE := $(BUILD)/slweb.o
CPP := $(BUILD)/slweb_cpp.o

all: $(BUILD)/libslweb.a $(BUILD)/libslweb++.a

$(BUILD):
	mkdir -p $(BUILD)

$(CORE): native/src/slweb.c native/include/slweb.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(CPP): cpp/src/slweb.cpp cpp/include/slweb.hpp native/include/slweb.h | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD)/libslweb.a: $(CORE)
	$(AR) rcs $@ $^

$(BUILD)/libslweb++.a: $(CPP) $(CORE)
	$(AR) rcs $@ $^

$(BUILD)/test-c: tests/test_c.c $(CORE) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/test-cpp: tests/test_cpp.cpp $(CPP) $(CORE) | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test: $(BUILD)/test-c $(BUILD)/test-cpp
	$(BUILD)/test-c
	$(BUILD)/test-cpp

# ---- Windows cross-build (MinGW-w64) ---------------------------------------
# Produces Windows static libs from Linux/macOS when a MinGW-w64 toolchain is
# present. Output goes to build/win/ so it never collides with the native
# build. Override the triple for 32-bit: `make windows MINGW=i686-w64-mingw32`.
# The library is dependency-free, so the C core and C++17 facade cross-compile
# directly; see PORTABILITY.md.
MINGW ?= x86_64-w64-mingw32
WINCC := $(MINGW)-gcc
WINCXX := $(MINGW)-g++
WINAR := $(MINGW)-ar
WINBUILD := $(BUILD)/win

windows:
	@command -v $(WINCC) >/dev/null 2>&1 || { \
	  echo "MinGW-w64 not found ($(WINCC)). Install e.g. 'mingw-w64' and retry,"; \
	  echo "or set MINGW=<triple> (e.g. i686-w64-mingw32 for 32-bit)."; \
	  exit 1; }
	mkdir -p $(WINBUILD)
	$(WINCC)  $(CPPFLAGS) $(CFLAGS)  -c native/src/slweb.c  -o $(WINBUILD)/slweb.o
	$(WINCXX) $(CPPFLAGS) $(CXXFLAGS) -c cpp/src/slweb.cpp   -o $(WINBUILD)/slweb_cpp.o
	$(WINAR) rcs $(WINBUILD)/libslweb.a   $(WINBUILD)/slweb.o
	$(WINAR) rcs $(WINBUILD)/libslweb++.a $(WINBUILD)/slweb_cpp.o $(WINBUILD)/slweb.o
	@echo "Windows libs: $(WINBUILD)/libslweb.a $(WINBUILD)/libslweb++.a (triple $(MINGW))"

# Build the Windows smoke tests as .exe (run under Wine or on Windows).
windows-test: windows
	$(WINCC)  $(CPPFLAGS) $(CFLAGS)  tests/test_c.c   $(WINBUILD)/slweb.o -o $(WINBUILD)/test-c.exe
	$(WINCXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_cpp.cpp $(WINBUILD)/slweb_cpp.o $(WINBUILD)/slweb.o -o $(WINBUILD)/test-cpp.exe
	@echo "Windows test exes: $(WINBUILD)/test-c.exe $(WINBUILD)/test-cpp.exe"

clean:
	rm -rf $(BUILD)

.PHONY: all test windows windows-test clean
