CC ?= cc
CXX ?= c++
JAVA ?= java
JAVAC ?= javac
BUILD ?= ../build/natural-tests
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
.PHONY: all test c cpp java sleela clean
all: test
test: c cpp java sleela
	./run-all.sh
c:
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I../include ../src/sleela_regex_natural.c ../tests/test_regex_natural.c -o $(BUILD)-c
	$(BUILD)-c
cpp:
	mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -I../include ../src/sleela_regex_natural.cpp ../tests/test_regex_natural.cpp -o $(BUILD)-cpp
	$(BUILD)-cpp
java:
	mkdir -p $(BUILD)/java
	$(JAVAC) -d $(BUILD)/java ../java/SleelaRegexNatural.java ../java/SleelaRegexNaturalParser.java ../java/SleelaRegexNaturalTest.java
	$(JAVA) -cp $(BUILD)/java sleela.regex.SleelaRegexNaturalTest
sleela:
	./test_sleela_sources.sh
clean:
	rm -rf $(BUILD) $(BUILD)-c $(BUILD)-cpp
