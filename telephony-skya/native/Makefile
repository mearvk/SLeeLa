# Skya native engine build.
#
# Builds two executables and a static library:
#   skya         -- combined client/server/both bridge (main.cpp)
#   skya-server  -- standalone server entry point (skya_server_main.cpp)
#   libskya.a    -- the engine as a static archive for embedding
#
# Platform linkage:
#   * POSIX (Linux/macOS): std::thread needs -pthread.
#   * Windows (MinGW):     Winsock (WSAStartup/closesocket) needs -lws2_32.
# GNU Make on Windows/MinGW sets OS=Windows_NT; otherwise detect via uname.
CXX?=c++
CXXFLAGS?=-std=c++17 -O2 -Wall -Wextra -Wpedantic

ifeq ($(OS),Windows_NT)
  PLATFORM_LIBS:=-lws2_32
  THREADFLAGS:=
  EXE:=.exe
else
  UNAME_S:=$(shell uname -s)
  PLATFORM_LIBS:=
  THREADFLAGS:=-pthread
  EXE:=
endif

LDLIBS:=$(THREADFLAGS) $(PLATFORM_LIBS)

all: skya$(EXE) skya-server$(EXE) libskya.a

skya$(EXE): main.o skya_engine.o
	$(CXX) $(CXXFLAGS) $(THREADFLAGS) $^ -o $@ $(PLATFORM_LIBS)

skya-server$(EXE): skya_server_main.o skya_engine.o
	$(CXX) $(CXXFLAGS) $(THREADFLAGS) $^ -o $@ $(PLATFORM_LIBS)

libskya.a: skya_engine.o
	ar rcs $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(THREADFLAGS) -c $< -o $@

clean:
	rm -f *.o skya skya.exe skya-server skya-server.exe libskya.a

.PHONY: all clean
