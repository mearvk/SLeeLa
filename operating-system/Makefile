# SLeeLa OS subsystem -- the C/C++ that backs the lib/os SLeeLa layer's fine and
# executive functions: downloading, manipulating, ideology inspection &
# conversion, security review, and build aggregation, plus the minimum-boot
# driver series. Compiled into one static library (libsleela_os.a) with the
# tests, following the /compression subsystem convention.
#
# This layer sits at the OS-build concern (driver tables, fetch command
# composition, shape/convert logic, SHA-256 review, aggregation). It does NOT
# reimplement the VM syscall bridge: the actual transport (git clone, mkdir,
# file I/O) is the runtime's job via impl/core (slos_*/slio_*). Everything here
# is pure, dependency-free value logic that is unit-testable on its own.

CC  ?= cc
CXX ?= c++
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra -Wpedantic -Iinclude -I.
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude -I.
LDFLAGS  ?=

BUILD := build

# C sources: the capability cores + the default driver table + the base-driver
# runtime (at the subsystem root).
SRC_C := \
  src/sleela_os_shape.c \
  src/sleela_os_fetch.c \
  src/sleela_os_ideology.c \
  src/sleela_os_security.c \
  src/sleela_os_aggregate.c \
  src/sleela_os_theme.c \
  src/base_drivers_table.c \
  base_drivers.c

# C++ sources: the executable-translation classifier + the base-driver C++ view.
SRC_CPP := \
  src/sleela_os_translate.cpp \
  base_drivers.cpp

OBJS := $(patsubst %,$(BUILD)/%.o,$(SRC_C) $(SRC_CPP))

all: $(BUILD)/libsleela_os.a

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.c.o: %.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.cpp.o: %.cpp | $(BUILD)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/libsleela_os.a: $(OBJS)
	ar rcs $@ $(OBJS)

# Tests. The library embeds C++ objects, so link the C test TUs with the C++
# driver (same approach as /compression).
TESTS := $(BUILD)/test_os_shape $(BUILD)/test_os_fetch $(BUILD)/test_os_ideology \
         $(BUILD)/test_os_security $(BUILD)/test_os_aggregate $(BUILD)/test_os_translate \
         $(BUILD)/test_os_theme $(BUILD)/test_os_base_drivers

test: $(TESTS)
	./$(BUILD)/test_os_shape
	./$(BUILD)/test_os_fetch
	./$(BUILD)/test_os_ideology
	./$(BUILD)/test_os_security
	./$(BUILD)/test_os_aggregate
	./$(BUILD)/test_os_translate
	./$(BUILD)/test_os_theme
	./$(BUILD)/test_os_base_drivers
	@echo "os: all tests passed"

$(BUILD)/test_%: tests/test_%.c $(BUILD)/libsleela_os.a | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@.o
	$(CXX) $@.o $(BUILD)/libsleela_os.a -o $@

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
