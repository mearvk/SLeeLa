# =============================================================================
# Standard VMs: the ten escalating idea-vm-001..010 (think-tank ladder).
#
# Builds one smoke/demo program per version into build/NNN/idea-vm-NNN and runs
# it. Each version is compiled from ITS OWN folder only (no dependency on the
# native Core archive, so this does not need `make core`). The per-version main
# is vm_*_test.c (001..009) or vm_runtime_demo.c (010); every other vm_*.c plus
# the four core pieces it links against (memmgr, memory, thread, platform) go in
# with it, exactly as documented in each main file header.
#
#   make -C virtual-machine/vm-version all     build + run all ten
#   make -C virtual-machine/vm-version 005     build + run one version
#   make -C virtual-machine/vm-version clean
#
# Output goes to build/ (git-ignored); the version folders are never written to,
# so their MODIUM.json manifests stay valid.
# =============================================================================
CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -DVM_HAVE_MEMMGR -DVM_HAVE_PLATFORM
LDLIBS ?= -pthread -ldl -lm

VERSIONS := 001 002 003 004 005 006 007 008 009 010
CORE_SRCS := sleela_memmgr.c sleela_memory.c sleela_thread.c sleela_platform.c

all: $(VERSIONS)
	@echo "SLeeLa standard VMs built and run: virtual-machine/vm-version/build/NNN/idea-vm-NNN"

build/%:
	@mkdir -p $@

# One version: pick its main, compile, run. Fails the build if either step fails.
$(VERSIONS): %: | build/%
	@main=$$(cd $@ && ls vm_*_test.c vm_runtime_demo.c 2>/dev/null | head -1); \
	test -n "$$main" || { echo "standard VM $@: no main program found" >&2; exit 1; }; \
	srcs=$$(cd $@ && ls vm_*.c | grep -v "^$$main$$" | grep -v '_test\.c$$\|_demo\.c$$'); \
	echo "=== standard VM $@: build ($$main) ==="; \
	$(CC) $(CFLAGS) -I$@ $@/$$main $(addprefix $@/,$(CORE_SRCS)) \
	  $$(for s in $$srcs; do echo $@/$$s; done) $(LDLIBS) -o build/$@/idea-vm-$@ || exit 1; \
	echo "=== standard VM $@: run ==="; \
	build/$@/idea-vm-$@ >build/$@/run.log 2>&1 || { cat build/$@/run.log; echo "standard VM $@: FAILED" >&2; exit 1; }

clean:
	rm -rf build

.PHONY: all clean $(VERSIONS)
