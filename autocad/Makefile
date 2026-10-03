# SLeeLa AutoCAD Renderer -- build/render helpers.
#
# The renderer itself is authoritative SLeeLa source under cad/. This Makefile
# only drives it: it runs the SLeeLa runtime on render.sleela and captures the
# generated AutoCAD DXF into out/drawing.dxf. The JavaFX plan-entry GUI lives
# under gui/ and builds with Maven.
#
# The SLeeLa runtime is located via SLEELA_BIN (defaults to the repository's
# staged binary). Override on the command line if yours lives elsewhere.

REPO_ROOT := $(abspath $(CURDIR)/..)
SLEELA_BIN ?= $(REPO_ROOT)/bin/SLeeLa
OUT := out
RENDER := cad/render.sleela

export SLEELA_SHEET ?= $(REPO_ROOT)/SHEET.sheet
export SLEELA_SHA256_MANIFEST ?= $(REPO_ROOT)/security/sha256-manifest.json

.PHONY: all render gui clean help

all: render

# Run the SLeeLa renderer and capture the generated AutoCAD DXF.
render: $(OUT)
	@echo "Rendering AutoCAD DXF via SLeeLa: $(RENDER)"
	@"$(SLEELA_BIN)" run "$(RENDER)" > "$(OUT)/drawing.dxf" \
		&& echo "Wrote $(OUT)/drawing.dxf (open in AutoCAD or LibreCAD)" \
		|| { echo "SLeeLa runtime not available; see cad/ for the renderer source."; exit 1; }

$(OUT):
	@mkdir -p "$(OUT)"

# Build the JavaFX plan-entry GUI (presentation layer).
gui:
	@cd gui && mvn -q -DskipTests package

clean:
	@rm -rf "$(OUT)"
	@cd gui && mvn -q clean >/dev/null 2>&1 || true

help:
	@echo "SLeeLa AutoCAD Renderer"
	@echo "  make render  Run the SLeeLa renderer -> out/drawing.dxf"
	@echo "  make gui     Build the JavaFX plan-entry GUI (Maven)"
	@echo "  make clean   Remove generated output"
