# Skya native engine build.
#
# Builds two executables and a static library:
#   skya         -- combined client/server/both bridge (main.cpp); links the
#                   drivers archive so `skya --drivers` can enumerate hardware
#                   drivers from the per-vendor tree
#   skya-server  -- standalone server entry point (skya_server_main.cpp)
#   libskya.a    -- the engine as a static archive for embedding
#
# Platform linkage:
#   * POSIX (Linux/macOS): std::thread needs -pthread.
#   * Windows (MinGW):     Winsock (WSAStartup/closesocket) needs -lws2_32.
#   * macOS:               the drivers platform layer links CoreAudio.
# GNU Make on Windows/MinGW sets OS=Windows_NT; otherwise detect via uname.
CXX?=c++
CXXFLAGS?=-std=c++17 -O2 -Wall -Wextra -Wpedantic

DRIVERS_DIR:=../drivers
DRIVERS_LIB:=$(DRIVERS_DIR)/libskya-drivers.a

ifeq ($(OS),Windows_NT)
  PLATFORM_LIBS:=-lws2_32
  THREADFLAGS:=
  DRIVER_FRAMEWORKS:=
  EXE:=.exe
else
  UNAME_S:=$(shell uname -s)
  THREADFLAGS:=-pthread
  EXE:=
  ifeq ($(UNAME_S),Darwin)
    PLATFORM_LIBS:=
    DRIVER_FRAMEWORKS:=-framework CoreAudio
  else
    PLATFORM_LIBS:=
    DRIVER_FRAMEWORKS:=
  endif
endif

all: skya$(EXE) skya-server$(EXE) libskya.a

# Build the drivers archive via its own Makefile so the per-vendor driver tree
# is actually compiled and consumed by the engine.
$(DRIVERS_LIB):
	$(MAKE) -C $(DRIVERS_DIR)

skya$(EXE): main.o skya_engine.o $(DRIVERS_LIB)
	$(CXX) $(CXXFLAGS) $(THREADFLAGS) main.o skya_engine.o $(DRIVERS_LIB) -o $@ $(PLATFORM_LIBS) $(DRIVER_FRAMEWORKS)

skya-server$(EXE): skya_server_main.o skya_engine.o
	$(CXX) $(CXXFLAGS) $(THREADFLAGS) $^ -o $@ $(PLATFORM_LIBS)

libskya.a: skya_engine.o
	ar rcs $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(THREADFLAGS) -c $< -o $@

clean:
	rm -f *.o skya skya.exe skya-server skya-server.exe libskya.a
	$(MAKE) -C $(DRIVERS_DIR) clean

.PHONY: all clean
