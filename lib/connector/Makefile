CC ?= cc
CXX ?= c++
MAKE ?= make
AR ?= ar
PKG_CONFIG ?= pkg-config

# Optional native client libraries. Registry/parser smoke tests do not require
# all these packages; build each family only when its dependency exists.
DATABASE_CONNECTORS := mysql mariadb sqlite postgres odbc sqlserver oracle
DATA_CONNECTORS := delimited json xml rest mqtt
CONNECTORS := $(DATABASE_CONNECTORS) $(DATA_CONNECTORS)

.PHONY: all build database data registry test test-rest test-mqtt clean help check-deps

all: registry database data
build: all

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

# Dependency-free tests are the default and never contact external services.
test: test_connector_registry
	./test_connector_registry
	$(MAKE) -C delimited test
	$(MAKE) -C json test
	$(MAKE) -C xml test

test_connector_registry: test_connector_registry.c connector_registry.c connector_registry.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -O2 -Wall -Wextra -Wpedantic $^ -o $@

# Optional compile-and-guard tests require the matching native development package.
test-rest:
	$(MAKE) -C rest test

test-mqtt:
	$(MAKE) -C mqtt test

check-deps:
	@echo "Toolchain:"
	@command -v "$(CC)" >/dev/null 2>&1 && "$(CC)" --version | head -n 1 || echo "MISSING: $(CC)"
	@command -v "$(CXX)" >/dev/null 2>&1 && "$(CXX)" --version | head -n 1 || echo "MISSING: $(CXX)"
	@echo "Optional native connector dependencies (missing packages only disable that family):"
	@for p in mysqlclient libmariadb sqlite3 libpq odbc libcurl libxml-2.0 libmosquitto; do \
		if command -v "$(PKG_CONFIG)" >/dev/null 2>&1 && $(PKG_CONFIG) --exists $$p; then \
			echo "AVAILABLE: $$p ($$($(PKG_CONFIG) --modversion $$p))"; \
		else echo "OPTIONAL/MISSING: $$p"; fi; \
	done
	@echo "Note: SQL Server and Oracle profiles also require a configured vendor ODBC driver."

clean:
	@set -e; for d in $(CONNECTORS); do echo "==> Cleaning $$d"; $(MAKE) -C "$$d" clean; done
	rm -f connector_registry.o connector_registry_cpp.o libsleela_connector_registry.a test_connector_registry

help:
	@echo "make all          Build registry and all optional native connector families"
	@echo "make database     Build database connector profiles (requires native client libraries)"
	@echo "make data         Build delimited, JSON, XML, REST and MQTT connectors"
	@echo "make test         Run dependency-free registry, delimited, JSON and XML smoke tests"
	@echo "make test-rest    Run optional REST guard tests (requires libcurl development files)"
	@echo "make test-mqtt    Run optional MQTT guard tests (requires libmosquitto development files)"
	@echo "make check-deps   Report toolchain and optional native connector dependencies"
	@echo "make clean        Clean generated files for all connector families"
