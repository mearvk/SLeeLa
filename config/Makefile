# SLeeLa configuration validation and native resolver smoke tests
SHELL := /bin/sh
.DEFAULT_GOAL := test
CC ?= cc
PYTHON ?= python3
BUILD_DIR ?= build
SMOKE := $(BUILD_DIR)/config-location-smoke
VM_SMOKE := $(BUILD_DIR)/vm-config-smoke

.PHONY: all test check smoke clean help
all: test
test: check smoke

check:
	cd .. && "$(PYTHON)" scripts/validate-config.py

$(SMOKE): ../runtime/config_location.c ../runtime/config_location.h ../runtime/config_location_smoke.c
	@mkdir -p "$(BUILD_DIR)"
	"$(CC)" -std=c11 -Wall -Wextra -pedantic -I../runtime ../runtime/config_location.c ../runtime/config_location_smoke.c -o "$@"

$(VM_SMOKE): ../runtime/vm_config.c ../runtime/vm_config.h ../runtime/vm_config_smoke.c
	@mkdir -p "$(BUILD_DIR)"
	"$(CC)" -std=c11 -Wall -Wextra -pedantic -I../runtime ../runtime/vm_config.c ../runtime/vm_config_smoke.c -o "$@"

smoke: $(SMOKE) $(VM_SMOKE)
	cd .. && "./config/$(SMOKE)"
	cd .. && "./config/$(VM_SMOKE)"

clean:
	$(RM) -r "$(BUILD_DIR)"

help:
	@echo "Configuration targets: all, test, check, smoke, clean, help"
