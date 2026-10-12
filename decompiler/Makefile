# Slecompiler™ build wrapper
# Requires CMake 3.20+ and a C++20 compiler.
# Works on Linux, macOS, and Windows with GNU Make + CMake.

.DEFAULT_GOAL := all

CMAKE ?= cmake
BUILD_DIR ?= build
BUILD_TYPE ?= Release
JOBS ?=
GENERATOR ?=
BUILD_EXAMPLES ?= ON
PREFIX ?= $(CURDIR)/$(BUILD_DIR)/stage
CMAKE_ARGS = -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DSLEE_LA_BUILD_API_EXAMPLES=$(BUILD_EXAMPLES) -DCMAKE_INSTALL_PREFIX="$(PREFIX)"
ifneq ($(strip $(GENERATOR)),)
CMAKE_ARGS += -G "$(GENERATOR)"
endif

.PHONY: all configure build test check stage install clean rebuild help

all: build

configure:
	$(CMAKE) -S . -B "$(BUILD_DIR)" $(CMAKE_ARGS)

build: configure
	$(CMAKE) --build "$(BUILD_DIR)" --config $(BUILD_TYPE) $(if $(JOBS),--parallel $(JOBS),--parallel)

test: build
	$(CMAKE) --build "$(BUILD_DIR)" --config $(BUILD_TYPE) --target sleela-decompiler-test
	$(CMAKE) -E chdir "$(BUILD_DIR)" ctest --output-on-failure -C $(BUILD_TYPE)

check: test

stage: build
	$(CMAKE) --install "$(BUILD_DIR)" --config $(BUILD_TYPE) --prefix "$(PREFIX)"

install: stage

rebuild: clean all

clean:
	$(CMAKE) -E rm -rf "$(BUILD_DIR)"

help:
	@echo "Slecompiler build targets:"
	@echo "  make / make all      Configure and build"
	@echo "  make configure       Generate the CMake build"
	@echo "  make build           Compile library, CLI, tests, and API examples"
	@echo "  make test            Build and run the registered tests"
	@echo "  make check           Alias for test"
	@echo "  make stage           Install to PREFIX (default: ./build/stage)"
	@echo "  make clean           Remove generated build files"
	@echo ""
	@echo "Options: BUILD_DIR=build BUILD_TYPE=Release JOBS=4 GENERATOR='Ninja'"
	@echo "         BUILD_EXAMPLES=ON|OFF PREFIX=/desired/staging/path"
