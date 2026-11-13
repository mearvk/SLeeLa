# sleelactl/convert -- Java -> SLeeLa converter (C lexer + model loader, C++ translator).
#
#   make            build build/javaconv
#   make test       self-check the model, then run the JDK differential suite
#   make clean
#
# Built by `sleelactl build java-convert`; run by `sleelactl run java-convert ...`.
CC  ?= cc
CXX ?= c++
CFLAGS   ?= -O2 -Wall -Wextra -std=c99
CXXFLAGS ?= -O2 -Wall -Wextra -std=c++17
BUILD := build

OBJS := $(BUILD)/java_lex.o $(BUILD)/conv_model.o $(BUILD)/java_to_sleela.o $(BUILD)/javaconv_main.o

all: $(BUILD)/javaconv

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.cpp java_to_sleela.h conv_model.h java_lex.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/javaconv: $(OBJS)
	$(CXX) $(OBJS) -o $@

test: all
	$(BUILD)/javaconv --self-check
	bash ../../tests/javaconv_differential_test.sh

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
