CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
AR ?= ar
ARFLAGS ?= rcs

BUILD_DIR ?= build
LIB := $(BUILD_DIR)/libslvm7.a

C_SOURCES := $(wildcard src/*.c)
CPP_SOURCES := $(wildcard src/*.cpp)
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))
OBJECTS := $(C_OBJECTS) $(CPP_OBJECTS)

.PHONY: all clean check config

all: $(LIB)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

check:
	@echo "SLVM/7 static contract check"
	@test -f config/slvm-runtime.conf
	@test -f include/slvm7.h
	@test -f include/slvm7_log.h
	@test -f include/slvm7_memory.h
	@test -f include/slvm7_manager.h
	@test -f include/slvm7_recovery.h
	@test -f include/slvm7_health.h
	@echo "SLVM/7 required files present"

config:
	@cat config/slvm-runtime.conf

clean:
	rm -rf $(BUILD_DIR)
