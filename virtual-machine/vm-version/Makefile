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
# Output is plain ASCII text (no colour, no progress bars) so it can be pasted
# straight into a bug report: every compiler message and every program line is
# printed inline, each version ends in a one-line RESULT, and a summary table
# closes the run. All versions are attempted even if one fails; the exit status
# is non-zero if any failed. Per-version logs: build/NNN/{compile,run}.log and
# the combined transcript build/BUILD.log.
#
# Output goes to build/ (git-ignored); the version folders are never written to,
# so their MODIUM.json manifests stay valid.
# =============================================================================
CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -DVM_HAVE_MEMMGR -DVM_HAVE_PLATFORM
LDLIBS ?= -pthread -ldl -lm

VERSIONS := 001 002 003 004 005 006 007 008 009 010
CORE_SRCS := sleela_memmgr.c sleela_memory.c sleela_thread.c sleela_platform.c

export LC_ALL := C
export NO_COLOR := 1
CFLAGS += -fdiagnostics-color=never

# Attempt every version, tee the transcript, then print a summary.
all:
	@mkdir -p build; rm -f build/*/result; \
	{ echo "# standard VMs build transcript"; \
	  echo "# host: $$(uname -srm)   cc: $$($(CC) --version | head -1)"; \
	  for v in $(VERSIONS); do $(MAKE) --no-print-directory $$v; done; \
	  echo; echo "=== summary ==="; \
	  fail=0; for v in $(VERSIONS); do \
	    r=$$(cat build/$$v/result 2>/dev/null || echo "FAIL (not attempted)"); \
	    echo "  standard VM $$v: $$r"; \
	    case "$$r" in PASS*) ;; *) fail=1;; esac; done; \
	  if [ $$fail = 0 ]; then echo "ALL 10 PASS"; else echo "SOME FAILED"; fi; \
	  echo $$fail > build/.fail; } 2>&1 | tee build/BUILD.log; \
	test "$$(cat build/.fail)" = 0

build/%:
	@mkdir -p $@

# One version: pick its main, compile, run. Never aborts the whole run; writes
# build/NNN/result as "PASS" or "FAIL (stage, detail)".
$(VERSIONS): %: | build/%
	@d=$@; r=build/$@/result; \
	main=$$(cd $$d && ls vm_*_test.c vm_runtime_demo.c 2>/dev/null | head -1); \
	echo; echo "=== standard VM $$d: build ($$main) ==="; \
	if [ -z "$$main" ]; then echo "FAIL (no main program found)" > $$r; echo "RESULT $$d: $$(cat $$r)"; exit 0; fi; \
	srcs=$$(cd $$d && ls vm_*.c | grep -v "^$$main$$" | grep -v '_test\.c$$\|_demo\.c$$'); \
	files="$$d/$$main $(addprefix $$d/,$(CORE_SRCS)) $$(for s in $$srcs; do echo $$d/$$s; done)"; \
	echo "\$$ $(CC) $(CFLAGS) -I$$d <$$(echo $$files | wc -w) files> $(LDLIBS) -o build/$$d/idea-vm-$$d"; \
	if $(CC) $(CFLAGS) -I$$d $$files $(LDLIBS) -o build/$$d/idea-vm-$$d >build/$$d/compile.log 2>&1; then \
	  echo "compile: ok ($$(grep -c 'warning:' build/$$d/compile.log) warning(s))"; cat build/$$d/compile.log; \
	else \
	  echo "compile: FAILED"; cat build/$$d/compile.log; \
	  echo "FAIL (compile, see build/$$d/compile.log)" > $$r; echo "RESULT $$d: $$(cat $$r)"; exit 0; fi; \
	echo "--- run: build/$$d/idea-vm-$$d ---"; \
	build/$$d/idea-vm-$$d >build/$$d/run.log 2>&1; rc=$$?; cat build/$$d/run.log; \
	echo "--- exit code: $$rc ---"; \
	if [ $$rc = 0 ]; then echo PASS > $$r; else echo "FAIL (run, exit $$rc, see build/$$d/run.log)" > $$r; fi; \
	echo "RESULT $$d: $$(cat $$r)"

clean:
	rm -rf build

.PHONY: all clean $(VERSIONS)
