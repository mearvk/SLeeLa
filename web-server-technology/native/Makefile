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

all: $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

$(SQL_OBJ): $(SQL_SRC) $(SQL_HDR) | $(BUILD)
	$(CC) -I ../../lib/sleela-sql/native/include $(CFLAGS) -c $< -o $@

$(TARGET): main.cpp dyn_sql_server.cpp dyn_sql_server.h envelopment.cpp envelopment.h $(SQL_OBJ) | $(BUILD)
	$(CXX) $(CXXFLAGS) main.cpp dyn_sql_server.cpp envelopment.cpp $(SQL_OBJ) -o $@

clean:
	rm -rf $(BUILD)

.PHONY: all clean
