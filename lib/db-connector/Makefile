# lib/db-connector: the SLeeLa DB connector (JDBC-like driver manager + driver).
# Builds a static lib and links the lib/sleela-sql engine core for the bundled
# "sleelasql" driver. Mirrors the lib/sleela-sql build layout.
CC ?= cc
AR ?= ar
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -I include -I ../sleela-sql/native/include
BUILD := build

CONN := $(BUILD)/sldbc.o
SQL := $(BUILD)/sleela_sql.o

all: $(BUILD)/libsldbc.a

$(BUILD):
	mkdir -p $(BUILD)

$(CONN): src/sldbc.c include/sldbc.h ../sleela-sql/native/include/sleela_sql.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(SQL): ../sleela-sql/native/src/sleela_sql.c ../sleela-sql/native/include/sleela_sql.h | $(BUILD)
	$(CC) -I ../sleela-sql/native/include $(CFLAGS) -c $< -o $@

# The connector lib bundles its own object plus the engine core so consumers
# link a single archive.
$(BUILD)/libsldbc.a: $(CONN) $(SQL)
	$(AR) rcs $@ $^

$(BUILD)/test-connector: tests/test_connector.c $(CONN) $(SQL) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test: $(BUILD)/test-connector
	rm -rf $(BUILD)/test-data && mkdir -p $(BUILD)/test-data
	$(BUILD)/test-connector $(BUILD)/test-data

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
