CC ?= cc
CXX ?= c++
AR ?= ar
PKG_CONFIG ?= pkg-config
MYSQL_PKG ?= libmariadb
CFLAGS ?= -O2
CXXFLAGS ?= -O2
CPPFLAGS ?=
MYSQL_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags $(MYSQL_PKG) 2>/dev/null)
MYSQL_LIBS ?= $(shell $(PKG_CONFIG) --libs $(MYSQL_PKG) 2>/dev/null)
BUILD_DIR ?= build

.PHONY: all clean test
all: $(BUILD_DIR)/libsleela_mysql_connector.a $(BUILD_DIR)/mysql_connector_cpp.o
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/mysql_connector.o: mysql_connector.c mysql_connector.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(MYSQL_CFLAGS) -I. -Wall -Wextra -Wpedantic -c $< -o $@
$(BUILD_DIR)/libsleela_mysql_connector.a: $(BUILD_DIR)/mysql_connector.o
	$(AR) rcs $@ $<
$(BUILD_DIR)/mysql_connector_cpp.o: mysql_connector.cpp mysql_connector.hpp mysql_connector.h | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(MYSQL_CFLAGS) -I. -Wall -Wextra -Wpedantic -std=c++17 -c $< -o $@
$(BUILD_DIR)/test_mysql_connector: test_mysql_connector.c $(BUILD_DIR)/mysql_connector.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $(MYSQL_CFLAGS) -I. $^ $(MYSQL_LIBS) -o $@
test: $(BUILD_DIR)/test_mysql_connector
	@echo "Integration test needs a reachable server and SLEELA_MYSQL_* environment variables."
	$(BUILD_DIR)/test_mysql_connector
clean:
	rm -rf $(BUILD_DIR)
