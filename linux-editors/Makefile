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
	$(TARGET) --themes | grep -q '^russian-slavic$$'
	$(TARGET) --theme eurasia --hex type | grep -q '#69C9B3'
	$(TARGET) --theme russian-slavic --hex keyword | grep -q '#A9C0DD'
	$(TARGET) --theme silk-road --hex string | grep -q '#D9B56D'
	$(TARGET) --theme east-asian --hex keyword | grep -q '#D97878'
clean:
	rm -rf $(BUILD)
