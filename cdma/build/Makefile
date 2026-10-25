# SPDX-License-Identifier: MIT
CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Werror
DIRS = ../common ../is-95-cdmaone ../cdma2000-1xrtt ../cdma2000-evdo ../wcdma-umts ../tools
.PHONY: all test clean
all:
	@set -e; for d in $(DIRS); do if [ -f $$d/build/Makefile ]; then $(MAKE) -C $$d/build all; elif [ -f $$d/Makefile ]; then $(MAKE) -C $$d all; fi; done
test:
	@set -e; for d in $(DIRS); do if [ -f $$d/build/Makefile ]; then $(MAKE) -C $$d/build test; elif [ -f $$d/Makefile ]; then $(MAKE) -C $$d test; fi; done
clean:
	@set -e; for d in $(DIRS); do if [ -f $$d/build/Makefile ]; then $(MAKE) -C $$d/build clean; elif [ -f $$d/Makefile ]; then $(MAKE) -C $$d clean; fi; done
