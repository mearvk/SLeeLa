# Makefile -- Idea System(TM) native core (C) + C++ facade/driver.
# Idea System(TM) / Idea(TM).  From Author; distributive.
#
# Targets:
#   make            build the demo driver (bin/idea-system)
#   make lib        build the static C core (lib/libideasystem.a)
#   make run        build and run the demo driver
#   make clean      remove build artifacts

CC       ?= cc
CXX      ?= c++
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
AR       ?= ar

BUILD    := build
BIN      := $(BUILD)/idea-system
CORE_A   := $(BUILD)/libideasystem.a

.PHONY: all lib run clean
all: $(BIN)

$(BUILD):
	mkdir -p $(BUILD)

# C core -> object
$(BUILD)/idea_system.o: idea_system.c idea_system.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Static library of the C core (what the VM / embedders link against)
$(CORE_A): $(BUILD)/idea_system.o
	$(AR) rcs $@ $^

lib: $(CORE_A)

# C++ driver -> object
$(BUILD)/idea_main.o: idea_main.cpp idea_system.hpp idea_system.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link the demo driver against the C core
$(BIN): $(BUILD)/idea_main.o $(CORE_A)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(BUILD)
