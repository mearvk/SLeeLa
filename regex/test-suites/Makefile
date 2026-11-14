CC ?= cc
CXX ?= c++
JAVA ?= java
JAVAC ?= javac
BUILD ?= ../build/natural-tests
EXE ?=
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror

.PHONY: all test c cpp java sleela clean
all: test

test: c cpp java sleela
	./run-all.sh

c: $(BUILD)-c$(EXE)
	$<

$(BUILD)-c$(EXE): ../src/sleela_regex_natural.c ../tests/test_regex_natural.c | $(BUILD)
	$(CC) $(CFLAGS) -I../include $^ -o $@

cpp: $(BUILD)-cpp$(EXE)
	$<

$(BUILD)-cpp$(EXE): ../src/sleela_regex_natural.cpp ../tests/test_regex_natural.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -I../include $^ -o $@

java:
	mkdir -p $(BUILD)/java
	$(JAVAC) -d $(BUILD)/java ../java/SleelaRegexNatural.java ../java/SleelaRegexNaturalParser.java ../java/SleelaRegexNaturalTest.java
	$(JAVA) -cp $(BUILD)/java sleela.regex.SleelaRegexNaturalTest

sleela:
	./test_sleela_sources.sh

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD) $(BUILD)-c $(BUILD)-c.exe $(BUILD)-cpp $(BUILD)-cpp.exe
