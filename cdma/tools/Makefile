# SPDX-License-Identifier: MIT
CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
INCLUDES = -I../common/include
COMMON = ../common/src/cdma_common.c ../common/src/cdma_link.c
LDLIBS_MATH = -lm
.PHONY: all syntax test clean
all: cdma-sim
cdma-sim: cdma_sim.c $(COMMON)
	$(CC) $(CFLAGS) $(INCLUDES) cdma_sim.c $(COMMON) -o cdma-sim $(LDLIBS_MATH)
syntax:
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only cdma_sim.c
# Smoke test: a clean single-user run must report BER 0, and a noisy 8-user
# run must complete and report a finite BER.
test: cdma-sim
	./cdma-sim --std is95 --users 1 --ebn0 20 --trials 2000 --seed 1
	./cdma-sim --std wcdma --sf 64 --users 8 --ebn0 6 --paths 3 --trials 2000 --seed 2
clean:
	rm -f cdma-sim
