CC ?= cc
CXX ?= c++
JAVA ?= java
JAVAC ?= javac
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
CPPFLAGS ?= -Iinclude
BUILD ?= build

ifeq ($(OS),Windows_NT)
  EXE := .exe
else
  EXE :=
endif
export EXE

.PHONY: all c cpp natural java test test-suites clean
all: c cpp natural java test-suites

c: $(BUILD)/regex-c$(EXE)
	$<

$(BUILD)/regex-c$(EXE): src/sleela_regex.c tests/test_regex.c | $(BUILD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

cpp: $(BUILD)/regex-cpp$(EXE)
	$<

$(BUILD)/regex-cpp$(EXE): src/sleela_regex.cpp tests/test_regex.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

natural: $(BUILD)/regex-natural-c$(EXE) $(BUILD)/regex-natural-cpp$(EXE)
	$(BUILD)/regex-natural-c$(EXE)
	$(BUILD)/regex-natural-cpp$(EXE)

$(BUILD)/regex-natural-c$(EXE): src/sleela_regex_natural.c tests/test_regex_natural.c | $(BUILD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(BUILD)/regex-natural-cpp$(EXE): src/sleela_regex_natural.cpp tests/test_regex_natural.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

java:
	mkdir -p $(BUILD)/java
	$(JAVAC) -d $(BUILD)/java java/SleelaRegexNatural.java java/SleelaRegexNaturalParser.java java/SleelaRegexNaturalTest.java
	$(JAVA) -cp $(BUILD)/java sleela.regex.SleelaRegexNaturalTest

test: test-suites

test-suites:
	$(MAKE) -C test-suites test EXE=$(EXE)

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)
	$(MAKE) -C test-suites clean EXE=$(EXE)
