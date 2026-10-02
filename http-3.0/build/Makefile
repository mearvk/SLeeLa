SHELL := /bin/sh
.PHONY: all preferred syntax clean test
all: preferred
	$(MAKE) -C ..

preferred:
	@$(MAKE) -C ../../preferred-routers syntax

all-old:
	$(MAKE) -C ..
syntax:
	$(MAKE) -C .. cpp-check
clean:
	$(MAKE) -C .. clean
test:
	$(MAKE) -C .. test
