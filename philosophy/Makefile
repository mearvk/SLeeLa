CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
BUILD_DIR := build
ifeq ($(OS),Windows_NT)
EXE := .exe
THREAD_FLAGS :=
THREAD_LIBS :=
else
EXE :=
# POSIX/C11 threads need -pthread on Linux and the BSDs; harmless on macOS.
THREAD_FLAGS := -pthread
THREAD_LIBS := -pthread
endif
C_EXAMPLE := $(BUILD_DIR)/example-c$(EXE)
CPP_EXAMPLE := $(BUILD_DIR)/example-cpp$(EXE)
TEST_PROGRAM := $(BUILD_DIR)/test-screen-map$(EXE)
EVENT_C_EXAMPLE := $(BUILD_DIR)/event-example-c$(EXE)
EVENT_CPP_EXAMPLE := $(BUILD_DIR)/event-example-cpp$(EXE)
EVENT_TEST := $(BUILD_DIR)/test-event-tree$(EXE)
CONSISTENCY_EXAMPLE := $(BUILD_DIR)/consistency-example$(EXE)
CONSISTENCY_TEST := $(BUILD_DIR)/test-consistency$(EXE)
PARTITION_EXAMPLE := $(BUILD_DIR)/partition-example$(EXE)
PARTITION_TEST := $(BUILD_DIR)/test-partition-api$(EXE)

.PHONY: all test clean
all: $(C_EXAMPLE) $(CPP_EXAMPLE) $(TEST_PROGRAM) $(EVENT_C_EXAMPLE) $(EVENT_CPP_EXAMPLE) $(EVENT_TEST) $(CONSISTENCY_EXAMPLE) $(CONSISTENCY_TEST) $(PARTITION_EXAMPLE) $(PARTITION_TEST)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/screen_map.o: screen_map.c screen_map.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/conversation_mask.o: conversation_mask.cpp conversation_mask.hpp screen_map.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/example-c.o: example.c screen_map.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/example-cpp.o: example.cpp conversation_mask.hpp screen_map.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/test-screen-map.o: test_screen_map.cpp conversation_mask.hpp screen_map.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(C_EXAMPLE): $(BUILD_DIR)/screen_map.o $(BUILD_DIR)/example-c.o
	$(CC) $^ -lm -o $@
$(CPP_EXAMPLE): $(BUILD_DIR)/screen_map.o $(BUILD_DIR)/conversation_mask.o $(BUILD_DIR)/example-cpp.o
	$(CXX) $^ -lm -o $@
$(TEST_PROGRAM): $(BUILD_DIR)/screen_map.o $(BUILD_DIR)/conversation_mask.o $(BUILD_DIR)/test-screen-map.o
	$(CXX) $^ -lm -o $@

$(BUILD_DIR)/event_tree.o: event_tree.c event_tree.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/social_listener.o: social_listener.cpp social_listener.hpp event_tree.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/event-example-c.o: event_example.c event_tree.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/event-example-cpp.o: event_example.cpp social_listener.hpp event_tree.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/test-event-tree.o: test_event_tree.cpp event_tree.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(EVENT_C_EXAMPLE): $(BUILD_DIR)/event_tree.o $(BUILD_DIR)/event-example-c.o
	$(CC) $^ -lm -o $@
$(EVENT_CPP_EXAMPLE): $(BUILD_DIR)/event_tree.o $(BUILD_DIR)/social_listener.o $(BUILD_DIR)/event-example-cpp.o
	$(CXX) $^ -lm -o $@
$(EVENT_TEST): $(BUILD_DIR)/event_tree.o $(BUILD_DIR)/social_listener.o $(BUILD_DIR)/test-event-tree.o
	$(CXX) $^ -lm -o $@

$(BUILD_DIR)/thread_compat.o: thread_compat.c thread_compat.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@
$(BUILD_DIR)/consistency_monitor.o: consistency_monitor.c consistency_monitor.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/signal_toss.o: signal_toss.c signal_toss.h consistency_monitor.h thread_compat.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@
$(BUILD_DIR)/consistency-example.o: consistency_example.c consistency_monitor.h signal_toss.h event_tree.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@
$(BUILD_DIR)/test-consistency.o: test_consistency.c consistency_monitor.h signal_toss.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@
$(CONSISTENCY_EXAMPLE): $(BUILD_DIR)/consistency_monitor.o $(BUILD_DIR)/signal_toss.o $(BUILD_DIR)/thread_compat.o $(BUILD_DIR)/event_tree.o $(BUILD_DIR)/consistency-example.o
	$(CC) $^ -lm $(THREAD_LIBS) -o $@
$(CONSISTENCY_TEST): $(BUILD_DIR)/consistency_monitor.o $(BUILD_DIR)/signal_toss.o $(BUILD_DIR)/thread_compat.o $(BUILD_DIR)/test-consistency.o
	$(CC) $^ -lm $(THREAD_LIBS) -o $@

$(BUILD_DIR)/partition_api.o: partition_api.c partition_api.h consistency_monitor.h signal_toss.h event_tree.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@
$(BUILD_DIR)/partition-example.o: partition_example.c partition_api.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@
$(BUILD_DIR)/test-partition-api.o: test_partition_api.c partition_api.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@
PARTITION_OBJS := $(BUILD_DIR)/partition_api.o $(BUILD_DIR)/consistency_monitor.o $(BUILD_DIR)/signal_toss.o $(BUILD_DIR)/thread_compat.o $(BUILD_DIR)/event_tree.o
$(PARTITION_EXAMPLE): $(PARTITION_OBJS) $(BUILD_DIR)/partition-example.o
	$(CC) $^ -lm $(THREAD_LIBS) -o $@
$(PARTITION_TEST): $(PARTITION_OBJS) $(BUILD_DIR)/test-partition-api.o
	$(CC) $^ -lm $(THREAD_LIBS) -o $@

test: $(TEST_PROGRAM) $(EVENT_TEST) $(CONSISTENCY_TEST) $(PARTITION_TEST)
	./$(TEST_PROGRAM)
	./$(EVENT_TEST)
	./$(CONSISTENCY_TEST)
	./$(PARTITION_TEST)
clean:
	rm -rf $(BUILD_DIR)
