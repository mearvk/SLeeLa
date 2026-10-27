# Author: Max Rupplin — MEARVK LLC — 2026
CC       ?= cc
CXX      ?= c++
AR       ?= ar
CFLAGS   ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
BUILD    ?= build
INCLUDES := -Iinclude
LIB      := $(BUILD)/libsleela_signal_processing.a
COBJS    := $(BUILD)/slep_core.o $(BUILD)/slep_filter.o $(BUILD)/slep_fft.o
CPPOBJ   := $(BUILD)/slep_cpp.o

.PHONY: all lib test examples clean
all: lib examples
lib: $(LIB)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/slep_core.o: src/slep_core.c include/sleela_signal_processing.h | $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD)/slep_filter.o: src/slep_filter.c include/sleela_signal_processing.h | $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD)/slep_fft.o: src/slep_fft.c include/sleela_signal_processing.h | $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(CPPOBJ): src/slep_cpp.cpp include/sleela_signal_processing.hpp include/sleela_signal_processing.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(LIB): $(COBJS) $(CPPOBJ)
	$(AR) rcs $@ $^

$(BUILD)/test_signal_processing.o: tests/test_signal_processing.c include/sleela_signal_processing.h | $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD)/test_signal_processing_cpp.o: tests/test_signal_processing.cpp include/sleela_signal_processing.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD)/test_signal_processing: $(BUILD)/test_signal_processing.o $(LIB)
	$(CXX) -o $@ $^ -lm
$(BUILD)/test_signal_processing_cpp: $(BUILD)/test_signal_processing_cpp.o $(LIB)
	$(CXX) -o $@ $^ -lm

$(BUILD)/basic_c.o: examples/c/basic.c include/sleela_signal_processing.h | $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD)/sleela_sp_c_example: $(BUILD)/basic_c.o $(LIB)
	$(CXX) -o $@ $^ -lm
$(BUILD)/sleela_sp_cpp_example: examples/cpp/basic.cpp $(LIB) include/sleela_signal_processing.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< $(LIB) -lm -o $@

examples: $(BUILD)/sleela_sp_c_example $(BUILD)/sleela_sp_cpp_example
test: $(BUILD)/test_signal_processing $(BUILD)/test_signal_processing_cpp
	$(BUILD)/test_signal_processing
	$(BUILD)/test_signal_processing_cpp

clean:
	rm -rf $(BUILD)
