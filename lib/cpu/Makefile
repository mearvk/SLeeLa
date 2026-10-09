# SLeeLa CPU package build
# Max Rupplin - MEARVK LLC - 2026
#
# lib/cpu is a .sleela source library (the Sleela CPU models, the Secondary-VM
# executor base, the component catalog, and the DMA/GPU accelerators). The Sleela
# toolchain resolves these through the shared library index, so this Makefile is
# a source-inventory and contract check rather than a C/C++ compile — the same
# role lib/vm's checks play for the VM package. The per-architecture native bus/
# timing detail lives in each folder's markdown docs.

.PHONY: all inventory fleet-check components-check options-check clean help

all: inventory fleet-check components-check options-check

# Count and report the CPU source inventory.
inventory:
	@printf '%s\n' 'SLeeLa CPU source inventory'
	@printf '  CPU models (SL*CPU.sleela): %s\n' "$$(find . -name 'SL*CPU.sleela' | wc -l | tr -d ' ')"
	@printf '  architecture folders:       %s\n' "$$(find . -mindepth 1 -maxdepth 1 -type d | wc -l | tr -d ' ')"
	@printf '  component sources:          %s\n' "$$(find components -maxdepth 1 -name '*.sleela' | wc -l | tr -d ' ')"
	@printf '  total .sleela:              %s\n' "$$(find . -name '*.sleela' | wc -l | tr -d ' ')"

# Every architecture folder must carry a runnable model that extends the executor
# base (SLCPURuntime), so each CPU can run resource-assigned C/C++/Java/Sleela
# workloads. pdp8 keeps its own hand-written model (also extends SLCPURuntime).
fleet-check:
	@printf '%s\n' 'CPU fleet check: every folder has an SL<ARCH>CPU extending SLCPURuntime'
	@missing=0; \
	for d in */ ; do \
	  case "$$d" in examples/|components/) continue ;; esac ; \
	  f=$$(ls "$$d"SL*CPU.sleela 2>/dev/null | head -1) ; \
	  if [ -z "$$f" ]; then echo "  MISSING model: $$d" ; missing=1 ; \
	  elif ! grep -q 'extends SLCPURuntime' "$$f" ; then echo "  not runnable: $$f" ; missing=1 ; fi ; \
	done ; \
	if [ "$$missing" = 0 ]; then printf '%s\n' '  all architecture folders runnable: FINE' ; else exit 1 ; fi

# The component catalog (user-selectable parts) and the executor builder.
components-check:
	@printf '%s\n' 'Component catalog check'
	@for c in SLMemoryModule SLStorageDevice SLOpticalDrive SLSystemBus SLComponentCatalog SLExecutorBuild ; do \
	  test -f "components/$$c.sleela" && printf '  present: %s\n' "$$c" || { echo "  MISSING: $$c" ; exit 1 ; } ; \
	done
	@printf '%s\n' '  clock/overclock: SLClockPolicy; parts: RAM(DRAM..DDR5) storage(floppy..NVMe) optical(CD/DVD/BD) bus(ISA..PCIe)'

# Optional capabilities layered on the executor and the Secondary VM.
options-check:
	@printf '%s\n' 'Executor options check: direct DMA + native GPU access'
	@test -f SLDMAController.sleela || { echo '  MISSING: SLDMAController' ; exit 1 ; }
	@test -f SLGPUDevice.sleela || { echo '  MISSING: SLGPUDevice' ; exit 1 ; }
	@printf '%s\n' '  options: DMA (SLDMAController), GPU (SLGPUDevice); enabled per-CPU, per-SLVM, or via the VM Creator'

clean:
	@printf '%s\n' 'lib/cpu is a source library; nothing to clean'

help:
	@printf '%s\n' 'SLeeLa CPU package build'
	@printf '%s\n' '  make all              Inventory + fleet + components + options checks'
	@printf '%s\n' '  make inventory        Count CPU models, folders, and components'
	@printf '%s\n' '  make fleet-check      Verify every folder has a runnable SL<ARCH>CPU'
	@printf '%s\n' '  make components-check Verify the component catalog is present'
	@printf '%s\n' '  make options-check    Verify the DMA and GPU option sources are present'
