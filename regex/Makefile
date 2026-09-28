CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
CPPFLAGS ?= -Iinclude
BUILD ?= build

.PHONY: all c cpp natural test java clean
all: c cpp natural java
c:
	$(CC) $(CFLAGS) $(CPPFLAGS) src/sleela_regex.c tests/test_regex.c -o $(BUILD)/regex-c
	$(BUILD)/regex-c
cpp:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) src/sleela_regex.cpp tests/test_regex.cpp -o $(BUILD)/regex-cpp
	$(BUILD)/regex-cpp
natural:
	$(CC) $(CFLAGS) $(CPPFLAGS) src/sleela_regex_natural.c tests/test_regex_natural.c -o $(BUILD)/regex-natural-c
	$(BUILD)/regex-natural-c
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) src/sleela_regex_natural.cpp tests/test_regex_natural.cpp -o $(BUILD)/regex-natural-cpp
	$(BUILD)/regex-natural-cpp
java:
	javac -d $(BUILD)/java java/SleelaRegexNatural.java java/SleelaRegexNaturalParser.java java/SleelaRegexNaturalTest.java
	java -cp $(BUILD)/java sleela.regex.SleelaRegexNaturalTest

test: all
clean:
	rm -rf $(BUILD)
