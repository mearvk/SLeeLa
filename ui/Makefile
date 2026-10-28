# Compatibility shim for the canonical user-interface implementation.
# The actual source tree is in ../user-interface.

REPO_ROOT := $(abspath $(CURDIR)/..)
UI_DIR := $(REPO_ROOT)/user-interface

.PHONY: all lib example military smoke snapshot throbber-snapshot \
	relief-snapshot font-snapshot mood-snapshot install install-strip \
	uninstall clean help

all: ; $(MAKE) -C $(UI_DIR) all
lib: ; $(MAKE) -C $(UI_DIR) lib
example: ; $(MAKE) -C $(UI_DIR) example
military: ; $(MAKE) -C $(UI_DIR) military
smoke: ; $(MAKE) -C $(UI_DIR) smoke
snapshot: ; $(MAKE) -C $(UI_DIR) snapshot
throbber-snapshot: ; $(MAKE) -C $(UI_DIR) throbber-snapshot
relief-snapshot: ; $(MAKE) -C $(UI_DIR) relief-snapshot
font-snapshot: ; $(MAKE) -C $(UI_DIR) font-snapshot
mood-snapshot: ; $(MAKE) -C $(UI_DIR) mood-snapshot
install: ; $(MAKE) -C $(UI_DIR) install
install-strip: ; $(MAKE) -C $(UI_DIR) install-strip
uninstall: ; $(MAKE) -C $(UI_DIR) uninstall
clean: ; $(MAKE) -C $(UI_DIR) clean

help:
	@echo "Compatibility ui/ shim for the canonical SLeeLa UI project."
	@echo "The real source tree is: $(UI_DIR)"
	@echo "Delegating to user-interface/Makefile."
	@$(MAKE) -C $(UI_DIR) help
