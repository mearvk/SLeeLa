SHELL := /bin/sh
GRADES := 7.0 8.0 9.0

.PHONY: all syntax audit clean

all: syntax

syntax:
	@set -e; for g in $(GRADES); do $(MAKE) -C ../$$g/build syntax; done

audit:
	@./audit.sh

clean:
	@for g in $(GRADES); do $(MAKE) -C ../$$g/build clean || true; done
