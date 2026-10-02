SHELL := /bin/sh
GRADES := 7.0 8.0 9.0

.PHONY: all preferred syntax audit clean

all: preferred syntax

preferred:
	@$(MAKE) -C ../../preferred-routers syntax

syntax:
	@set -e; for g in $(GRADES); do $(MAKE) -C ../$g/build syntax; done

audit:
	@sh ./audit.sh

clean:
	@for g in $(GRADES); do $(MAKE) -C ../$$g/build clean || true; done
