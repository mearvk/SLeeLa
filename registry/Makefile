# registry/Makefile -- legacy facade + Registry 3.0 record engine.
CC       ?= cc
CXX      ?= c++
AR       ?= ar
CFLAGS   ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Wno-misleading-indentation
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
BUILD    := build
BIN      := $(BUILD)/sleela-registry
V3_BIN   := $(BUILD)/sleela-registry3
LIB      := $(BUILD)/libslregistry.a
V3_OBJ   := $(BUILD)/sleela_registry_v3.o
V3_TEST  := $(BUILD)/test_registry_v3
CPP_TEST := $(BUILD)/test_registry_v3_cpp

.PHONY: all lib run test clean
all: $(BIN) $(V3_BIN)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/sleela_registry.o: sleela_registry.c sleela_registry.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(V3_OBJ): v3/sleela_registry_v3.c v3/sleela_registry_v3.h sleela_registry.h | $(BUILD)
	$(CC) $(CFLAGS) -Iv3 -c $< -o $@

$(LIB): $(BUILD)/sleela_registry.o $(V3_OBJ)
	$(AR) rcs $@ $^

lib: $(LIB)

$(BUILD)/sleela_registry_tool.o: sleela_registry_tool.cpp sleela_registry.hpp sleela_registry.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): $(BUILD)/sleela_registry_tool.o $(LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/sleela_registry3_tool.o: v3/sleela-registry3.c v3/sleela_registry_v3.h | $(BUILD)
	$(CC) $(CFLAGS) -Iv3 -c $< -o $@

$(V3_BIN): $(BUILD)/sleela_registry3_tool.o $(LIB)
	$(CC) $(CFLAGS) $^ -o $@

$(V3_TEST): tests/test_registry_v3.c $(LIB) | $(BUILD)
	$(CC) $(CFLAGS) -Iv3 tests/test_registry_v3.c $(LIB) -o $@

$(CPP_TEST): tests/test_registry_v3_cpp.cpp v3/sleela_registry_v3.hpp $(LIB) | $(BUILD)
	$(CXX) $(CXXFLAGS) -Iv3 tests/test_registry_v3_cpp.cpp $(LIB) -o $@

test: $(V3_TEST) $(CPP_TEST)
	cd $(BUILD) && ./test_registry_v3
	cd $(BUILD) && ./test_registry_v3_cpp

run: $(BIN)
	./$(BIN) seed $(BUILD)/demo.sleelareg

clean:
	rm -rf $(BUILD)
