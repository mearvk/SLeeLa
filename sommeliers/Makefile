CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
BUILD_DIR := build

ifeq ($(OS),Windows_NT)
LDLIBS += -lws2_32
EXE := .exe
else
EXE :=
endif

PROGRAM := $(BUILD_DIR)/sommeliers-server$(EXE)
TEST_PROGRAM := $(BUILD_DIR)/feedback-test$(EXE)

.PHONY: all clean test run
all: $(PROGRAM)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/feedback.o: feedback.c feedback.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c feedback.c -o $@

$(BUILD_DIR)/server.o: server.cpp feedback.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c server.cpp -o $@

$(PROGRAM): $(BUILD_DIR)/feedback.o $(BUILD_DIR)/server.o
	$(CXX) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/feedback-test.o: feedback_test.c feedback.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c feedback_test.c -o $@

$(TEST_PROGRAM): $(BUILD_DIR)/feedback.o $(BUILD_DIR)/feedback-test.o
	$(CC) $^ -o $@

test: $(TEST_PROGRAM)
	./$(TEST_PROGRAM)

run: all
	./$(PROGRAM) 4081

clean:
	rm -f $(BUILD_DIR)/*.o $(PROGRAM) $(TEST_PROGRAM)
