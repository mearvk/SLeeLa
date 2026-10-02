SHELL := /bin/sh
.PHONY: all preferred syntax clean
all:
	$(MAKE) -C ..
preferred:
	@$(MAKE) -C ../../preferred-routers syntax

syntax:
	$(MAKE) -C .. syntax
clean:
	$(MAKE) -C .. clean
