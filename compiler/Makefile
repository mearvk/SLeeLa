# SLeeLa Native Compiler — portable CMake-backed build wrapper
# Max Rupplin - MEARVK LLC - 2026
#
# Requirements: CMake 3.20+ and a C++20 compiler.
# Works with GNU Make on Linux, macOS, and Windows environments that provide Make.
# Native Visual Studio users can use the equivalent CMake commands in README.md.

CMAKE ?= cmake
CTEST ?= ctest
BUILD_DIR ?= build
BUILD_TYPE ?= Release
PREFIX ?= $(CURDIR)/build/stage
GENERATOR ?=
JOBS ?=

CMAKE_ARGS = -S . -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
ifneq ($(strip $(GENERATOR)),)
CMAKE_ARGS += -G "$(GENERATOR)"
endif

.PHONY: all configure build test check run-example emit-ir install stage clean help

all: build

configure:
	$(CMAKE) $(CMAKE_ARGS)

build: configure
	$(CMAKE) --build "$(BUILD_DIR)" --config $(BUILD_TYPE) $(if $(JOBS),--parallel $(JOBS),)

test: build
	$(CTEST) --test-dir "$(BUILD_DIR)" -C $(BUILD_TYPE) --output-on-failure

check: test

run-example: build
	"$(BUILD_DIR)/sleelac$(if $(filter Windows_NT,$(OS)),.exe,)" --check examples/arithmetic.sleela

emit-ir: build
	"$(BUILD_DIR)/sleelac$(if $(filter Windows_NT,$(OS)),.exe,)" --emit-ir examples/arithmetic.sleela

stage: build
	$(CMAKE) --install "$(BUILD_DIR)" --config $(BUILD_TYPE) --prefix "$(PREFIX)"

install: stage

clean:
	$(CMAKE) -E rm -rf "$(BUILD_DIR)"

help:
	@echo "SLeeLa native compiler build targets:"
	@echo "  make [all]       Configure and build the compiler"
	@echo "  make test        Build and run compiler tests"
	@echo "  make check       Alias for test"
	@echo "  make run-example Check the arithmetic example"
	@echo "  make emit-ir     Print intermediate representation for the example"
	@echo "  make stage       Install under PREFIX (default: build/stage)"
	@echo "  make clean       Remove BUILD_DIR (default: build)"
	@echo "Variables: BUILD_DIR, BUILD_TYPE, PREFIX, GENERATOR, JOBS"
