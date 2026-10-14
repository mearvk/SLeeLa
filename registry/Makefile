# registry/Makefile -- Sleela Registry (C core + C++ facade/reviewer).
# Idea System(TM) / Idea(TM).  From Author; distributive.
#
#   make        build the reviewer (build/sleela-registry)
#   make lib    build the static C core (build/libslregistry.a)
#   make run    seed + review a demo registry
#   make clean

CC       ?= cc
CXX      ?= c++
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra -Wpedantic -Wno-misleading-indentation
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
AR       ?= ar
BUILD    := build
BIN      := $(BUILD)/sleela-registry
LIB      := $(BUILD)/libslregistry.a

.PHONY: all lib run clean
all: $(BIN)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/sleela_registry.o: sleela_registry.c sleela_registry.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(BUILD)/sleela_registry.o
	$(AR) rcs $@ $^
lib: $(LIB)

$(BUILD)/sleela_registry_tool.o: sleela_registry_tool.cpp sleela_registry.hpp sleela_registry.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): $(BUILD)/sleela_registry_tool.o $(LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(BIN)
	./$(BIN) seed $(BUILD)/demo.sleelareg

clean:
	rm -rf $(BUILD)
