# web-server-technology/native/Makefile
#
# Builds the SLeeLa dynamic, SQL-backed HTTP/1.1 server. The server links the
# CSV-backed SQL engine from lib/sleela-sql (built here as a small static lib)
# so the /db endpoint executes real statements.
CXX ?= c++
CC ?= cc
AR ?= ar
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
BUILD := build
TARGET := $(BUILD)/sleela-web-server

SQL_SRC := ../../lib/sleela-sql/native/src/sleela_sql.c
SQL_HDR := ../../lib/sleela-sql/native/include/sleela_sql.h
SQL_OBJ := $(BUILD)/sleela_sql.o

# Shared web-server-technology C core (envelopment/hardening/keep-warm model).
# The server now delegates its envelopment + hardening logic to this one
# implementation instead of carrying a parallel copy.
SLWEB_SRC := ../../lib/web-server-technology/native/src/slweb.c
SLWEB_HDR := ../../lib/web-server-technology/native/include/slweb.h
SLWEB_OBJ := $(BUILD)/slweb.o

all: $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

$(SQL_OBJ): $(SQL_SRC) $(SQL_HDR) | $(BUILD)
	$(CC) -I ../../lib/sleela-sql/native/include $(CFLAGS) -c $< -o $@

$(SLWEB_OBJ): $(SLWEB_SRC) $(SLWEB_HDR) | $(BUILD)
	$(CC) -I ../../lib/web-server-technology/native/include $(CFLAGS) -c $< -o $@

$(TARGET): main.cpp dyn_sql_server.cpp dyn_sql_server.h envelopment.cpp envelopment.h hardening.cpp hardening.h cpu_warmer.cpp cpu_warmer.h $(SQL_OBJ) $(SLWEB_OBJ) | $(BUILD)
	$(CXX) $(CXXFLAGS) main.cpp dyn_sql_server.cpp envelopment.cpp hardening.cpp cpu_warmer.cpp $(SQL_OBJ) $(SLWEB_OBJ) -o $@

# Unit tests for the Section-3 hardening primitives (no sockets).
# hardening.cpp now delegates to the shared slweb C core, so link it too.
$(BUILD)/hardening-test: hardening-test.cpp hardening.cpp hardening.h $(SLWEB_OBJ) | $(BUILD)
	$(CXX) $(CXXFLAGS) -I ../../lib/web-server-technology/native/include hardening-test.cpp hardening.cpp $(SLWEB_OBJ) -o $@

# Convergence test for the CPU keep-warm controller (takes ~4s).
$(BUILD)/cpu-warmer-test: cpu_warmer-test.cpp cpu_warmer.cpp cpu_warmer.h | $(BUILD)
	$(CXX) $(CXXFLAGS) cpu_warmer-test.cpp cpu_warmer.cpp -o $@

check: $(BUILD)/hardening-test $(BUILD)/cpu-warmer-test
	$(BUILD)/hardening-test
	$(BUILD)/cpu-warmer-test

clean:
	rm -rf $(BUILD)

.PHONY: all check clean
