# SLeeLa CPU roster / fleet / net-results -- build & run helpers.
#
# The CPU stack is authoritative SLeeLa source under lib/cpu/. The runnable
# "world" aggregate (examples/world-net-results.sleela) inlines a faithful
# behavioral model of the roster (SLCPUCatalog), the fleet runner + net-results
# aggregation (SLCPUFleet), the clock policy, resource grants, workloads, and
# the Creator-over-network distribution path, then runs end to end and ends with
# a SELF-CHECK. This mirrors how autocad/creator ships a runnable aggregate
# alongside its split library classes.
#
# The SLeeLa runtime is located via SLEELA_BIN (build it with: make -C impl).

REPO_ROOT := $(abspath $(CURDIR)/../..)
SLEELA_BIN ?= $(REPO_ROOT)/impl/build/sleela
OUT := out
WORLD := examples/world-net-results.sleela

export SLEELA_LIB ?= $(REPO_ROOT)/lib
export SLEELA_SHA256_MANIFEST ?= $(REPO_ROOT)/security/sha256-manifest.json

.PHONY: all world verify clean help

all: world

# Run the roster -> fleet -> net-results world simulation and capture the log.
world: $(OUT)
	@echo "Running CPU world simulation via SLeeLa: $(WORLD)"
	@"$(SLEELA_BIN)" run "$(WORLD)" > "$(OUT)/world.log" 2>/dev/null \
		|| { echo "SLeeLa runtime not available; build it with: make -C $(REPO_ROOT)/impl"; exit 1; }
	@sed -n 's/^/  /p' "$(OUT)/world.log" | grep -E "Roster|fleet|NET OUTPUT|aggregate|SELF-CHECK|\[ok\]|\[FAIL\]" || true
	@$(MAKE) --no-print-directory verify

# Output-level check: the world simulation's SELF-CHECK must pass.
verify:
	@grep -q "SELF-CHECK: PASS" "$(OUT)/world.log" \
		|| { echo "VERIFY FAIL: world-net-results self-check did not pass"; exit 1; }
	@echo "VERIFY OK: roster + fleet + net-results + creator-network self-check passed"

$(OUT):
	@mkdir -p "$(OUT)"

clean:
	@rm -rf "$(OUT)"

help:
	@echo "SLeeLa CPU roster / fleet / net-results"
	@echo "  make world   Run the roster->fleet->net-results world sim -> out/world.log"
	@echo "  make verify  Assert the world SELF-CHECK passed"
	@echo "  make clean   Remove generated output"
