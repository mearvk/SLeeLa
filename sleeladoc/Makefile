# SleelaDoc distribution: the terminal-reachable docs, bundled as a JAR that
# also carries the known .sleela sources.
# Max Rupplin - MEARVK LLC - 2026
#
# Targets:
#   make corpus   Generate the plain-text SleelaDoc corpus (via `sleela docs --generate`)
#   make jar      Build sleeladoc.jar = the corpus + the matching .sleela sources + manifest
#   make all      corpus then jar
#   make clean    Remove build outputs
#
# The JAR layout (reviewable with `jar tf build/sleeladoc.jar`):
#   docs/TOPICS.index                 one "package<TAB>Unit" line per documented unit
#   docs/<package>/<Unit>.txt         the rendered class page (same bytes as `sleela docs`)
#   docs/<package>/_package.txt       the rendered package overview
#   sources/<package>/<Unit>.sleela   the known source each page was generated from
#   META-INF/MANIFEST.MF              identity + build provenance
#   README.txt                        how to read and re-generate the bundle

SHELL := /bin/sh
.DEFAULT_GOAL := all

# The repository root is this directory's parent.
REPO_ROOT := $(abspath ..)
# Pin the compiler's SHA-256 gate and /lib to this checkout (same reasoning as
# the root dispatcher: an inherited SLEELA_HOME from an install must not hijack
# a source-tree build). Unconditional ':=' on purpose.
SLEELA_HOME := $(REPO_ROOT)
export SLEELA_HOME
SLEELA_LIB ?= $(REPO_ROOT)/lib
export SLEELA_LIB

SLEELA ?= $(REPO_ROOT)/impl/build/sleela
JAR    ?= jar

BUILD    := build
DOCS_DIR := $(BUILD)/stage/docs
SRC_DIR  := $(BUILD)/stage/sources
STAGE    := $(BUILD)/stage
JAR_FILE := $(BUILD)/sleeladoc.jar

.PHONY: all corpus jar sources clean help

all: jar

# 1) Generate the text corpus into the staging area using the built compiler.
#    `sleela docs --generate <dir>` writes docs/<pkg>/<Unit>.txt + TOPICS.index.
corpus:
	@test -x "$(SLEELA)" || { echo "Missing $(SLEELA); run 'make core' at the repo root first." >&2; exit 1; }
	@mkdir -p "$(DOCS_DIR)"
	"$(SLEELA)" docs --generate "$(DOCS_DIR)"

# 2) Copy the known .sleela sources that back each documented unit. We mirror
#    the corpus: for every docs/<pkg>/<Unit>.txt we copy lib/<...>/<Unit>.sleela
#    into sources/<pkg>/<Unit>.sleela, resolving the real path from TOPICS.index
#    is unnecessary -- we copy by walking the SAME package scope the corpus used.
#    The corpus already excluded the java mirror / early_access, so the source
#    set matches the doc set exactly.
sources: corpus
	@mkdir -p "$(SRC_DIR)"
	@python3 "$(REPO_ROOT)/sleeladoc/collect-sources.py" \
	    --lib "$(SLEELA_LIB)" \
	    --topics "$(DOCS_DIR)/TOPICS.index" \
	    --out "$(SRC_DIR)"

# 3) Pack the staged docs + sources into the JAR with a manifest and README.
jar: sources
	@mkdir -p "$(BUILD)" "$(STAGE)/META-INF"
	@{ \
	  echo "Manifest-Version: 1.0"; \
	  echo "Implementation-Title: SleelaDoc"; \
	  echo "Implementation-Vendor: MEARVK LLC"; \
	  echo "Implementation-Version: $$(cd "$(REPO_ROOT)" && git rev-parse --short HEAD 2>/dev/null || echo unknown)"; \
	  echo "SleelaDoc-Generated: $$(date -u +%Y-%m-%dT%H:%M:%SZ)"; \
	  echo "SleelaDoc-Contains: docs (plain text) and sources (.sleela)"; \
	} > "$(STAGE)/META-INF/MANIFEST.MF"
	@{ \
	  echo "SleelaDoc bundle -- the SLeeLa documentation brand."; \
	  echo ""; \
	  echo "docs/     plain-text API pages, identical to the output of:"; \
	  echo "              sleela docs <package> <Unit>"; \
	  echo "          TOPICS.index lists every documented unit (package<TAB>Unit)."; \
	  echo "sources/  the known .sleela source each page was generated from."; \
	  echo ""; \
	  echo "Read a page:   jar xf sleeladoc.jar docs/crypto/SLAes.txt && cat docs/crypto/SLAes.txt"; \
	  echo "List topics:   jar xf sleeladoc.jar docs/TOPICS.index && cat docs/TOPICS.index"; \
	  echo "Re-generate:   make -C sleeladoc jar   (needs impl/build/sleela)"; \
	} > "$(STAGE)/README.txt"
	"$(JAR)" --create --file "$(JAR_FILE)" --manifest "$(STAGE)/META-INF/MANIFEST.MF" \
	    -C "$(STAGE)" README.txt \
	    -C "$(STAGE)" docs \
	    -C "$(STAGE)" sources
	@echo "SleelaDoc JAR: $(JAR_FILE)"
	@echo "  docs pages:    $$(find "$(DOCS_DIR)" -name '*.txt' | wc -l | tr -d ' ')"
	@echo "  source files:  $$(find "$(SRC_DIR)" -name '*.sleela' | wc -l | tr -d ' ')"
	@echo "  entries:       $$("$(JAR)" tf "$(JAR_FILE)" | wc -l | tr -d ' ')"

clean:
	rm -rf "$(BUILD)"

help:
	@echo "SleelaDoc distribution targets:"
	@echo "  make corpus   Generate the plain-text SleelaDoc corpus"
	@echo "  make sources  Collect the matching .sleela sources"
	@echo "  make jar      Build build/sleeladoc.jar (corpus + sources + manifest)"
	@echo "  make all      corpus -> sources -> jar"
	@echo "  make clean    Remove build/"
