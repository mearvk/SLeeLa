# Churn data validation and Java constraint-report builds
PYTHON ?= python3
JAVAC ?= javac
JAVA ?= java
SETS := 1 2 3 4 5
JAVA_PACKAGE := com.mearvk.sleela.churn.Constraints

.PHONY: all compile test data-check clean help
all: compile data-check

compile:
	@command -v "$(JAVAC)" >/dev/null 2>&1 || { echo "javac is required" >&2; exit 1; }
	@set -eu; for n in $(SETS); do mkdir -p "build/$$n"; echo "[churn] compiling set $$n"; "$(JAVAC)" -encoding UTF-8 -d "build/$$n" "$$n/Constraints.java"; done

test: compile data-check
	@set -eu; for n in $(SETS); do echo "[churn] running set $$n"; (cd "$$n" && "$(JAVA)" -cp "../build/$$n" $(JAVA_PACKAGE)); done

data-check:
	@"$(PYTHON)" validate_data.py

clean:
	rm -rf build 1/out 2/out 3/out 4/out 5/out

help:
	@echo "all/compile: compile each Java set separately"
	@echo "test: run all five constraint reports"
	@echo "data-check: parse JSON/XML/CSV and check TXT files"
	@echo "clean: remove generated build outputs"
