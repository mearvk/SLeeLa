# =============================================================================
# SLeeLa <-> Java 28 SecureJDK memory integration (J28-MEM-0001).
#
# Builds the Java model, TCP port driver, and JNI bridge on Linux, macOS, and
# Windows (MinGW/MSYS2). Set JAVA_HOME to a full JDK when auto-detection is not
# sufficient. The default target compiles; demo targets execute the channels.
# =============================================================================
CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra
JAVAC ?= javac
JAVA ?= java

# Java 9+ reports the active runtime home. A full JDK is required for jni.h.
# An explicit environment/command-line JAVA_HOME always takes precedence.
JAVA_HOME ?= $(shell command -v "$(JAVA)" >/dev/null 2>&1 && $(JAVA) -XshowSettings:properties -version 2>&1 | sed -n 's/^[[:space:]]*java\.home = //p' | head -n 1)

BUILD := build
CLASSES := $(BUILD)/classes
JNIH := $(BUILD)/jni
BIN := $(BUILD)/bin

PKG := com.mearvk.sleela.java28
JAVA_SRC := $(shell find src/java -type f -name '*.java' -print)
NATIVE_INC := -Isrc/native
JVM_FLAGS ?= --enable-native-access=ALL-UNNAMED

# JNI headers, linker mode, and library filename are host-specific.
ifeq ($(OS),Windows_NT)
  PLATFORM := windows
  EXE := .exe
  JNI_PLATFORM := win32
  JNI_SHARED_FLAGS := -shared
  JNI_LIB := $(BIN)/sleela_java28.dll
  PORT_LIBS := -lws2_32
  CFLAGS += -D_WIN32_WINNT=0x0600
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
    EXE :=
    JNI_PLATFORM := darwin
    JNI_SHARED_FLAGS := -dynamiclib -fPIC
    JNI_LIB := $(BIN)/libsleela_java28.dylib
    PORT_LIBS :=
  else
    PLATFORM := linux
    EXE :=
    JNI_PLATFORM := linux
    JNI_SHARED_FLAGS := -shared -fPIC
    JNI_LIB := $(BIN)/libsleela_java28.so
    PORT_LIBS :=
  endif
endif

.PHONY: all java port jni demo demo-port demo-jni check-deps clean
all: java port jni

check-deps:
	@command -v "$(JAVAC)" >/dev/null 2>&1 || { echo "java28: javac not found; install a full JDK or set JAVAC." >&2; exit 1; }
	@command -v "$(JAVA)" >/dev/null 2>&1 || { echo "java28: java not found; install a JDK/JRE or set JAVA." >&2; exit 1; }
	@command -v "$(CC)" >/dev/null 2>&1 || { echo "java28: C compiler '$(CC)' not found." >&2; exit 1; }
	@test -f "$(JAVA_HOME)/include/jni.h" || { \
	  echo "java28: JNI headers not found at JAVA_HOME=$(JAVA_HOME)." >&2; \
	  echo "  Set JAVA_HOME to a full JDK containing include/jni.h." >&2; exit 1; }

# Java model, server, and JNI headers.
java: check-deps | $(CLASSES) $(JNIH)
	$(JAVAC) -h $(JNIH) -d $(CLASSES) $(JAVA_SRC)

# TCP port channel. Winsock is linked only on Windows.
port: $(BIN)/sleela_java28_demo$(EXE)
$(BIN)/sleela_java28_demo$(EXE): src/native/sleela_java28_port.c src/native/sleela_java28_demo.c src/native/sleela_java28.h | $(BIN)
	$(CC) $(CFLAGS) $(NATIVE_INC) src/native/sleela_java28_port.c src/native/sleela_java28_demo.c -o $@ $(PORT_LIBS)

# JNI bridge: the include subdirectory is win32, darwin, or linux as appropriate.
jni: $(JNI_LIB)
$(JNI_LIB): src/native/sleela_java28_bridge.c java | $(BIN)
	@test -f "$(JAVA_HOME)/include/jni.h" || { echo "java28: missing $(JAVA_HOME)/include/jni.h" >&2; exit 1; }
	@test -f "$(JAVA_HOME)/include/$(JNI_PLATFORM)/jni_md.h" || { echo "java28: missing platform JNI header $(JAVA_HOME)/include/$(JNI_PLATFORM)/jni_md.h" >&2; exit 1; }
	$(CC) $(CFLAGS) $(JNI_SHARED_FLAGS) -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/$(JNI_PLATFORM)" -I$(JNIH) $(NATIVE_INC) src/native/sleela_java28_bridge.c -o $@

# Demo orchestration uses a POSIX shell; on Windows run from MSYS2/Git Bash.
demo: demo-port demo-jni

demo-port: java port
	@echo "=== Port channel demo (SLeeLa C driver -> Java 28 memory over TCP) ==="; \
	$(JAVA) $(JVM_FLAGS) -cp $(CLASSES) $(PKG).Java28PortServer 0 > $(BUILD)/server.log 2>&1 & \
	SRV=$$!; \
	PORT=""; for i in $$(seq 1 50); do PORT=$$(grep -oE '^PORT [0-9]+' $(BUILD)/server.log | awk '{print $$2}'); [ -n "$$PORT" ] && break; sleep 0.1; done; \
	if [ -z "$$PORT" ]; then echo "server failed:"; cat $(BUILD)/server.log; kill $$SRV 2>/dev/null; exit 1; fi; \
	$(BIN)/sleela_java28_demo$(EXE) $$PORT; RC=$$?; wait $$SRV 2>/dev/null; exit $$RC

demo-jni: jni
	@echo "=== JNI channel demo (in-process local feedback) ==="
	$(JAVA) $(JVM_FLAGS) -Djava.library.path=$(BIN) -cp $(CLASSES) $(PKG).Java28JniRuntime

$(CLASSES) $(JNIH) $(BIN):
	mkdir -p "$@"

clean:
	rm -rf $(BUILD)
