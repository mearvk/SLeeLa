# SLeeLa Website Generator -- build/preview helpers.
#
# The generator itself is authoritative SLeeLa source under website/. This
# Makefile only drives it: it runs the SLeeLa runtime on site.sleela and
# captures the generated HTML into out/index.html for preview. The JavaFX
# authoring GUI lives under gui/ and builds with Maven.
#
# The SLeeLa runtime is located via SLEELA_BIN (defaults to the repository's
# staged binary). Override on the command line if yours lives elsewhere.

REPO_ROOT := $(abspath $(CURDIR)/..)
SLEELA_BIN ?= $(REPO_ROOT)/bin/SLeeLa
OUT := out
SITE := website/site.sleela

export SLEELA_SHEET ?= $(REPO_ROOT)/SHEET.sheet
export SLEELA_SHA256_MANIFEST ?= $(REPO_ROOT)/security/sha256-manifest.json

.PHONY: all generate preview gui clean help

all: generate

# Run the SLeeLa generator and capture the generated site.
generate: $(OUT)
	@echo "Generating website via SLeeLa: $(SITE)"
	@"$(SLEELA_BIN)" run "$(SITE)" > "$(OUT)/index.html" \
		&& echo "Wrote $(OUT)/index.html" \
		|| { echo "SLeeLa runtime not available; see website/ for the generator source."; exit 1; }

$(OUT):
	@mkdir -p "$(OUT)"

# Print the path a browser can open after generate.
preview: generate
	@echo "Open: $(CURDIR)/$(OUT)/index.html"

# Build the JavaFX authoring GUI (presentation layer).
gui:
	@cd gui && mvn -q -DskipTests package

clean:
	@rm -rf "$(OUT)"
	@cd gui && mvn -q clean >/dev/null 2>&1 || true

help:
	@echo "SLeeLa Website Generator"
	@echo "  make generate  Run the SLeeLa generator -> out/index.html"
	@echo "  make preview   Generate and print the output path"
	@echo "  make gui       Build the JavaFX authoring GUI (Maven)"
	@echo "  make clean     Remove generated output"
