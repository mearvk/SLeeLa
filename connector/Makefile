# SLeeLa Java connector build
# Requires GNU Make, a JDK 22+ (Foreign Function & Memory API), curl, and a POSIX shell.
# Override SERVLET_API_JAR to use an existing Jakarta Servlet API 6.0 jar.
# On Windows shells, set CLASSPATH_SEP=';' if the Java tools require Windows separators.

SHELL := /bin/sh
.DEFAULT_GOAL := all

JAVA_RELEASE ?= 22
JAVAC ?= javac
JAVA ?= java
JAR ?= jar
CURL ?= curl
PYTHON ?= python3
BUILD_DIR ?= build
CLASSPATH_SEP ?= :
CLASSES_DIR := $(BUILD_DIR)/classes
DIST_DIR := $(BUILD_DIR)/dist
DEPS_DIR := $(BUILD_DIR)/deps
SERVLET_API_VERSION ?= 6.0.0
SERVLET_API_JAR ?= $(DEPS_DIR)/jakarta.servlet-api-$(SERVLET_API_VERSION).jar
SERVLET_API_URL ?= https://repo.maven.apache.org/maven2/jakarta/servlet/jakarta.servlet-api/$(SERVLET_API_VERSION)/jakarta.servlet-api-$(SERVLET_API_VERSION).jar
JAR_FILE ?= $(DIST_DIR)/sleela-connector.jar

JAVA_SOURCES := $(shell find java -type f -name '*.java' -print 2>/dev/null)
CLASS_STAMP := $(CLASSES_DIR)/.compiled

.PHONY: all build deps check java-check compile test conformance jar package install clean help

all: compile
build: compile
deps: $(SERVLET_API_JAR)

java-check:
	@command -v "$(JAVAC)" >/dev/null 2>&1 || { echo "ERROR: javac not found; install JDK 22 or newer." >&2; exit 1; }
	@major=$$("$(JAVAC)" -version 2>&1 | sed -E 's/.* ([0-9]+).*/\1/'); \
	  case "$$major" in ''|*[!0-9]*) echo "ERROR: cannot determine javac version." >&2; exit 1;; esac; \
	  if [ "$$major" -lt "$(JAVA_RELEASE)" ]; then echo "ERROR: JDK $$major found; JDK $(JAVA_RELEASE)+ is required." >&2; exit 1; fi
	@echo "Java compiler requirement: satisfied"

$(SERVLET_API_JAR):
	@mkdir -p "$(dir $@)"
	@echo "Downloading Jakarta Servlet API $(SERVLET_API_VERSION)..."
	@$(CURL) --fail --location --silent --show-error "$(SERVLET_API_URL)" --output "$@.tmp"
	@mv "$@.tmp" "$@"

compile: java-check $(CLASS_STAMP)

$(CLASS_STAMP): $(JAVA_SOURCES) $(SERVLET_API_JAR)
	@test -n "$(JAVA_SOURCES)" || { echo "ERROR: no Java sources found under connector/java." >&2; exit 1; }
	@mkdir -p "$(CLASSES_DIR)"
	"$(JAVAC)" --release "$(JAVA_RELEASE)" -Xlint:all -cp "$(SERVLET_API_JAR)" -d "$(CLASSES_DIR)" $(JAVA_SOURCES)
	@touch "$@"
	@echo "Compiled $(words $(JAVA_SOURCES)) Java source files into $(CLASSES_DIR)"

conformance: compile
	"$(JAVA)" -ea -cp "$(CLASSES_DIR)$(CLASSPATH_SEP)$(SERVLET_API_JAR)" com.mearvk.sleela.connector.SleelaJdkConformance

test: conformance check
	@echo "SLeeLa connector tests: PASS"

check:
	@test -s deploy/web.xml || { echo "ERROR: deploy/web.xml missing or empty." >&2; exit 1; }
	@test -s deploy/tomcat-server.xml.sample || { echo "ERROR: Tomcat sample configuration missing or empty." >&2; exit 1; }
	@test -s deploy/apache-h3.conf || { echo "ERROR: Apache H3 configuration missing or empty." >&2; exit 1; }
	@if command -v "$(PYTHON)" >/dev/null 2>&1; then "$(PYTHON)" -c 'import xml.etree.ElementTree as ET; ET.parse("deploy/web.xml"); print("deploy/web.xml: valid XML")'; else echo "NOTE: Python not found; skipped XML well-formedness check."; fi

jar: compile
	@mkdir -p "$(DIST_DIR)"
	"$(JAR)" --create --file "$(JAR_FILE)" -C "$(CLASSES_DIR)" .
	@echo "Created $(JAR_FILE)"
package: jar

install: jar
	@test -n "$(DESTDIR)" || { echo "Set DESTDIR to an explicit installation directory, e.g. make install DESTDIR=/opt/sleela/connector" >&2; exit 2; }
	@mkdir -p "$(DESTDIR)"
	cp "$(JAR_FILE)" "$(DESTDIR)/sleela-connector.jar"
	@echo "Installed to $(DESTDIR)/sleela-connector.jar"

clean:
	$(RM) -r "$(BUILD_DIR)"

help:
	@echo "SLeeLa connector targets:"
	@echo "  all / build  Compile all Java connector sources"
	@echo "  deps         Download the Jakarta Servlet API dependency"
	@echo "  test         Run JDK bridge conformance + deployment checks"
	@echo "  conformance  Run the JDK bridge conformance test"
	@echo "  jar/package  Build $(JAR_FILE)"
	@echo "  install      Install the JAR (requires DESTDIR=...)"
	@echo "  check        Validate required deployment files and web.xml"
	@echo "  clean        Remove generated build artifacts"
	@echo "Overrides: JAVAC, JAVA, JAR, CURL, PYTHON, JAVA_RELEASE, CLASSPATH_SEP, SERVLET_API_JAR, BUILD_DIR"
