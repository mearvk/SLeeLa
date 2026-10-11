CC ?= cc
CXX ?= c++
MAKE ?= make
AR ?= ar

# All optional native client libraries must be installed before building.
# Override CC/CXX/CFLAGS/CXXFLAGS and connector *_CFLAGS/*_LIBS as needed.
DATABASE_CONNECTORS := mysql mariadb sqlite postgres odbc sqlserver oracle
DATA_CONNECTORS := delimited json xml rest mqtt
CONNECTORS := $(DATABASE_CONNECTORS) $(DATA_CONNECTORS)

.PHONY: all database data registry test clean help check-deps

all: registry database data

registry:
	$(MAKE) -C . registry-build

registry-build: connector_registry.o connector_registry_cpp.o libsleela_connector_registry.a

connector_registry.o: connector_registry.c connector_registry.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -O2 -Wall -Wextra -Wpedantic -c $< -o $@

connector_registry_cpp.o: connector_registry.cpp connector_registry.hpp connector_registry.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -std=c++17 -Wall -Wextra -Wpedantic -c $< -o $@

libsleela_connector_registry.a: connector_registry.o
	$(AR) rcs $@ $^

database:
	@set -e; for d in $(DATABASE_CONNECTORS); do echo "==> Building $$d"; $(MAKE) -C "$$d" all; done

data:
	@set -e; for d in $(DATA_CONNECTORS); do echo "==> Building $$d"; $(MAKE) -C "$$d" all; done

test: test_connector_registry
	./test_connector_registry
	$(MAKE) -C delimited test
	$(MAKE) -C json test
	$(MAKE) -C xml test

test_connector_registry: test_connector_registry.c connector_registry.c connector_registry.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -O2 -Wall -Wextra -Wpedantic $^ -o $@

check-deps:
	@echo "Compiler: $$($(CC) --version | head -n 1)"
	@echo "C++ compiler: $$($(CXX) --version | head -n 1)"
	@for p in libmariadb sqlite3 libpq odbc libcurl libxml-2.0 libmosquitto; do \
		if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists $$p; then \
			echo "OK: $$p ($$(pkg-config --modversion $$p))"; \
		else echo "MISSING/NOT DISCOVERABLE: $$p (required for relevant connector)"; fi; \
	done

clean:
	@set -e; for d in $(CONNECTORS); do echo "==> Cleaning $$d"; $(MAKE) -C "$$d" clean; done
	rm -f connector_registry.o connector_registry_cpp.o libsleela_connector_registry.a test_connector_registry

help:
	@echo "make all          Build registry, all database connectors, and all data-source connectors"
	@echo "make database     Build MySQL, MariaDB, SQLite, PostgreSQL, ODBC, SQL Server and Oracle profiles"
	@echo "make data         Build CSV/TSV, JSON, XML, REST and MQTT connectors"
	@echo "make test         Run registry, delimited, JSON and XML smoke tests"
	@echo "make check-deps   Report native dependencies discoverable through pkg-config"
	@echo "make clean        Clean generated files for all connector families"

# Explicit alias for scripts and IDE build actions.
build: all
.PHONY: build
