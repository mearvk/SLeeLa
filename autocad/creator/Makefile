# SLeeLa AutoCAD Creator(TM) -- build/run helpers.
#
# The Creator is authoritative SLeeLa source: the editable engine lives in
# /lib/autocad (SLCadEntity, SLCadDocument, SLCadCommand, SLCadCreator) and the
# SleelaUI GUI lives under cad/. This Makefile drives the self-contained,
# runnable aggregate (creator.sleela), capturing the altered-and-saved DXF into
# out/altered.dxf -- the "create -> alter -> save" story, headless.
#
# The SLeeLa runtime is located via SLEELA_BIN (defaults to the repository's
# staged binary). Override on the command line if yours lives elsewhere.

REPO_ROOT := $(abspath $(CURDIR)/../..)
SLEELA_BIN ?= $(REPO_ROOT)/bin/SLeeLa
OUT := out
CREATOR := creator.sleela

export SLEELA_SHEET ?= $(REPO_ROOT)/SHEET.sheet
export SLEELA_SHA256_MANIFEST ?= $(REPO_ROOT)/security/sha256-manifest.json
export SLEELA_LIB ?= $(REPO_ROOT)/lib

.PHONY: all creator gui clean help

all: creator

# Run the SLeeLa Creator aggregate and capture the altered-document DXF.
creator: $(OUT)
	@echo "Running AutoCAD Creator via SLeeLa: $(CREATOR)"
	@"$(SLEELA_BIN)" run "$(CREATOR)" > "$(OUT)/creator.log" 2>/dev/null \
		|| { echo "SLeeLa runtime not available; see cad/ for the Creator source."; exit 1; }
	@# The runtime prints a startup/verification banner on stdout before the
	@# program output; keep only the DXF document (from the first SECTION to EOF).
	@awk 'f{print} /^SECTION$$/{if(!f){print "0"; print; f=1}}' "$(OUT)/creator.log" \
		| awk '1; /^EOF$$/{exit}' > "$(OUT)/altered.dxf"
	@echo "Wrote $(OUT)/altered.dxf (open in AutoCAD or LibreCAD)"
	@echo "Report (from $(OUT)/creator.log):"
	@sed -n 's/^/  /p' "$(OUT)/creator.log" | grep -E "create|alter|delet|undo|saved|entities" || true

# The SleelaUI GUI is pure SLeeLa and needs no external build; on a display,
# run its application class through the runtime.
gui:
	@echo "The SleelaUI GUI is SLeeLa source under cad/ (CadCreatorApp.sleela)."
	@echo "Run it through the SLeeLa runtime on a host with a display."

clean:
	@rm -rf "$(OUT)"

help:
	@echo "SLeeLa AutoCAD Creator(TM)"
	@echo "  make creator  Run the Creator aggregate -> out/altered.dxf"
	@echo "  make gui      Notes on running the SleelaUI GUI"
	@echo "  make clean    Remove generated output"
