CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
BUILD ?= build
TARGET := $(BUILD)/sleela-editor-theme
.PHONY: all clean test
all: $(TARGET)
$(BUILD):
	mkdir -p $@
$(BUILD)/sleela_editor_theme.o: sleela_editor_theme.c sleela_editor_theme.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
$(TARGET): sleela_editor_theme.cpp $(BUILD)/sleela_editor_theme.o sleela_editor_theme.h
	$(CXX) $(CXXFLAGS) -I. sleela_editor_theme.cpp $(BUILD)/sleela_editor_theme.o -o $@
test: $(TARGET)
	$(TARGET) --palette
	$(TARGET) --hex keyword | grep -q '#7FB3FF'
clean:
	rm -rf $(BUILD)
