CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
BUILD ?= build
ifeq ($(OS),Windows_NT)
LDLIBS += -lws2_32
endif
.PHONY: all syntax test clean
all: $(BUILD)/sleela-resolver
$(BUILD):
	mkdir -p $(BUILD)
$(BUILD)/resolver.o: resolver.c resolver.h | $(BUILD)
	$(CC) $(CFLAGS) -c resolver.c -o $@
$(BUILD)/sleela-resolver: $(BUILD)/resolver.o
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)
syntax:
	$(CC) $(CFLAGS) -fsyntax-only resolver.c
	$(CXX) $(CXXFLAGS) -fsyntax-only resolver.cpp
test: $(BUILD)/sleela-resolver
	$(BUILD)/sleela-resolver --self-test
clean:
	rm -rf $(BUILD)
