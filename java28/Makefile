# =============================================================================
# Makefile for the Sleela <-> Java 28 SecureJDK memory integration (J28-MEM-0001).
#
# Builds both channels:
#   - port : Java memory server (Java28PortServer) + native C driver (over TCP)
#   - jni  : JNI shared library (libsleela_java28) + Java28JniRuntime (in-process)
#
# Requires a JDK (javac/java) and a C compiler. JAVA_HOME must point at a JDK
# with include/ headers for the JNI target. On this toolchain the default JDK
# works; override JAVA_HOME to build against the Java 28 SecureJDK.
# =============================================================================
CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra
JAVAC ?= javac
JAVA ?= java
JAVA_HOME ?= $(shell dirname $$(dirname $$(readlink -f $$(which javac))))

BUILD := build
CLASSES := $(BUILD)/classes
JNIH := $(BUILD)/jni
BIN := $(BUILD)/bin

PKG := com.mearvk.sleela.java28
JAVA_SRC := $(shell find src/java -name '*.java')
NATIVE_INC := -Isrc/native

# JVM flags: silence the JDK native-access advisory for the JNI demo.
JVM_FLAGS ?= --enable-native-access=ALL-UNNAMED

.PHONY: all java port jni demo demo-port demo-jni clean
all: java port jni

# --- Java side (memory server, value model, both runtimes, demo objects) -----
java: | $(CLASSES) $(JNIH)
	$(JAVAC) -h $(JNIH) -d $(CLASSES) $(JAVA_SRC)

# --- Port channel: native C driver linking the socket client ------------------
port: $(BIN)/sleela_java28_demo
$(BIN)/sleela_java28_demo: src/native/sleela_java28_port.c src/native/sleela_java28_demo.c src/native/sleela_java28.h | $(BIN)
	$(CC) $(CFLAGS) $(NATIVE_INC) src/native/sleela_java28_port.c src/native/sleela_java28_demo.c -o $@

# --- JNI channel: shared library (depends on generated JNI header) ------------
jni: $(BIN)/libsleela_java28.so
$(BIN)/libsleela_java28.so: src/native/sleela_java28_bridge.c java | $(BIN)
	$(CC) $(CFLAGS) -fPIC -shared \
	  -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux" -I$(JNIH) $(NATIVE_INC) \
	  src/native/sleela_java28_bridge.c -o $@

# --- Demos --------------------------------------------------------------------
demo: demo-port demo-jni

# Start the Java 28 memory server on an ephemeral port, then drive it from the
# native C client over the loopback port channel.
demo-port: java port
	@echo "=== Port channel demo (Sleela C driver -> Java 28 memory over TCP) ==="; \
	$(JAVA) $(JVM_FLAGS) -cp $(CLASSES) $(PKG).Java28PortServer 0 > $(BUILD)/server.log 2>&1 & \
	SRV=$$!; \
	PORT=""; for i in $$(seq 1 50); do PORT=$$(grep -oE '^PORT [0-9]+' $(BUILD)/server.log | awk '{print $$2}'); [ -n "$$PORT" ] && break; sleep 0.1; done; \
	if [ -z "$$PORT" ]; then echo "server failed:"; cat $(BUILD)/server.log; kill $$SRV 2>/dev/null; exit 1; fi; \
	$(BIN)/sleela_java28_demo $$PORT; RC=$$?; wait $$SRV 2>/dev/null; exit $$RC

# In-process local feedback: Java hosts the JVM, loads the JNI lib, native
# driver calls back into the Java 28 memory model.
demo-jni: jni
	@echo "=== JNI channel demo (in-process local feedback) ==="; \
	$(JAVA) $(JVM_FLAGS) -Djava.library.path=$(BIN) -cp $(CLASSES) $(PKG).Java28JniRuntime

$(CLASSES) $(JNIH) $(BIN):
	mkdir -p $@

clean:
	rm -rf $(BUILD)
