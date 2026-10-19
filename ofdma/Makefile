CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS += -Iinclude
BUILD := build
.PHONY: all test clean
all: $(BUILD)/ofdma-demo $(BUILD)/test-ofdma
$(BUILD):
	mkdir -p $(BUILD)
$(BUILD)/ofdma.o: src/ofdma.c include/ofdma.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD)/scheduler.o: src/scheduler.c include/ofdma.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD)/ofdma-demo: cpp/ofdma_demo.cpp cpp/ofdma.hpp $(BUILD)/ofdma.o $(BUILD)/scheduler.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Icpp cpp/ofdma_demo.cpp $(BUILD)/ofdma.o $(BUILD)/scheduler.o -o $@
$(BUILD)/test-ofdma: tests/test_ofdma.c $(BUILD)/ofdma.o $(BUILD)/scheduler.o
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_ofdma.c $(BUILD)/ofdma.o $(BUILD)/scheduler.o -o $@
test: $(BUILD)/test-ofdma
	./$(BUILD)/test-ofdma
clean:
	rm -rf $(BUILD)
