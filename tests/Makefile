CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -pedantic
CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
LDLIBS ?= -lm
PYTHON ?= python3

ifeq ($(OS),Windows_NT)
  EXE := .exe
  THREADFLAGS :=
else
  EXE :=
  THREADFLAGS := -pthread
endif

.PHONY: all check-deps clean check ai-neural-nets-xor neural-nets-xor ai-character-recognition character-recognition-smoke character-recognition-cli jdk28-conformance jdk28-functional jdk28-signatures java-source-equivalence java-conversions java-flow java-exceptions java-api-dependencies java-constraints java-signature-comparison java-platform-qualification

all: check-deps memory$(EXE) reflection$(EXE) nordshrift$(EXE) runtime$(EXE) network-security$(EXE) neural-nets-xor$(EXE) character-recognition-smoke$(EXE) character-recognition-cli$(EXE)

# The network/security smoke target uses OpenSSL EVP. Fail early with an
# actionable message rather than a later opaque -lcrypto link error.
check-deps:
	@echo '#include <openssl/evp.h>' | $(CXX) $(CXXFLAGS) -x c++ - -fsyntax-only >/dev/null 2>&1 || { echo "tests: OpenSSL headers are required for network-security." >&2; exit 1; }; echo 'int main(void) { return 0; }' | $(CXX) $(CXXFLAGS) -x c++ - -lcrypto -o .openssl-probe$(EXE) >/dev/null 2>&1 || { echo "tests: OpenSSL library (-lcrypto) is required for network-security." >&2; exit 1; }
	@rm -f .openssl-probe .openssl-probe.exe

memory$(EXE): native_memory_smoke.cpp ../api/native/memory/sleela_memory_manager.cpp
	$(CXX) $(CXXFLAGS) -I../api/native/memory $^ -o $@

reflection$(EXE): reflection_smoke.cpp ../api/native/reflection/sleela_reflection.cpp
	$(CXX) $(CXXFLAGS) -I../api/native/reflection $^ -o $@

nordshrift$(EXE): nordshrift_complete_smoke.cpp ../impl/nordshrift/complete/class_bindings.cpp
	$(CXX) $(CXXFLAGS) -I../impl/nordshrift/complete $^ -o $@

runtime$(EXE): runtime_smoke.cpp ../api/native/runtime/sleela_runtime.cpp
	$(CXX) $(CXXFLAGS) $(THREADFLAGS) -I../api/native/runtime $^ -o $@

network-security$(EXE): network_security_smoke.cpp ../api/native/network/sleela_network.cpp ../api/native/security/sleela_security.cpp
	$(CXX) $(CXXFLAGS) $^ -lcrypto -o $@

neural-nets-xor$(EXE): ../artificial-intelligence/neural-nets/xor_example.c ../artificial-intelligence/neural-nets/neural_nets.c ../artificial-intelligence/neural-nets/neural_nets.h
	$(CC) $(CFLAGS) -I../artificial-intelligence/neural-nets ../artificial-intelligence/neural-nets/xor_example.c ../artificial-intelligence/neural-nets/neural_nets.c $(LDLIBS) -o $@

ai-neural-nets-xor: neural-nets-xor$(EXE)
	./neural-nets-xor$(EXE)

character-recognition-smoke$(EXE): character_recognition_smoke.c ../artificial-intelligence/character-recognition/character_recognition.c ../artificial-intelligence/character-recognition/character_recognition.h
	$(CC) $(CFLAGS) -I../artificial-intelligence/character-recognition $< ../artificial-intelligence/character-recognition/character_recognition.c $(LDLIBS) -o $@

ai-character-recognition: character-recognition-smoke$(EXE)
	./character-recognition-smoke$(EXE)

character-recognition-cli$(EXE): ../artificial-intelligence/character-recognition/character_recognition_main.c ../artificial-intelligence/character-recognition/character_recognition.c ../artificial-intelligence/character-recognition/character_recognition.h
	$(CC) $(CFLAGS) -I../artificial-intelligence/character-recognition ../artificial-intelligence/character-recognition/character_recognition_main.c ../artificial-intelligence/character-recognition/character_recognition.c $(LDLIBS) -o $@
	./character-recognition-cli$(EXE) --help

check: all
	./memory$(EXE)
	./reflection$(EXE)
	./nordshrift$(EXE)
	./runtime$(EXE)
	./network-security$(EXE)
	./neural-nets-xor$(EXE)
	./character-recognition-smoke$(EXE)

jdk28-conformance:
	$(PYTHON) jdk28_functional_equivalence.py

jdk28-functional:
	$(PYTHON) jdk28_functional_equivalence.py --functional

jdk28-signatures:
	$(PYTHON) ../lib/java/tools/jdk28_signature_sync.py --check

java-source-equivalence:
	$(PYTHON) java_source_equivalence.py --java java_equivalence/EquivalenceBasic.java --sleela java_equivalence/EquivalenceBasic.sleela --json java_equivalence/report.json
	$(PYTHON) java_source_equivalence.py --java java_equivalence/ExpressionSurface.java --sleela java_equivalence/ExpressionSurface.sleela --json java_equivalence/expression-report.json

clean:
	rm -f java-conversions java-conversions.exe java-overload java-overload.exe memory memory.exe reflection reflection.exe nordshrift nordshrift.exe runtime runtime.exe network-security network-security.exe neural-nets-xor neural-nets-xor.exe character-recognition-smoke character-recognition-smoke.exe character-recognition-cli character-recognition-cli.exe character-recognition-smoke.txt character-recognition-smoke.jsonl

java-conversions$(EXE):
	$(CXX) $(CXXFLAGS) -I../impl/frontend -I../impl/frontend/java java_conversion_semantics.cpp ../impl/frontend/java/java_equivalence.cpp -o java-conversions$(EXE)
	./java-conversions$(EXE)

java-overload$(EXE):
	$(CXX) $(CXXFLAGS) -I../impl/frontend -I../impl/frontend/java java_overload_override.cpp ../impl/frontend/java/java_equivalence.cpp -o java-overload$(EXE)
	./java-overload$(EXE)

java-flow:
	$(PYTHON) java_flow_suite.py

java-exceptions:
	$(PYTHON) java_exceptions_suite.py

java-api-dependencies:
	$(PYTHON) java_api_dependency_suite.py

java-constraints:
	$(PYTHON) java_constraints_suite.py

java-signature-comparison:
	$(PYTHON) java_signature_comparison_suite.py

java-platform-qualification:
	$(PYTHON) java_platform_qualification_suite.py

java-qualification:
	$(PYTHON) java_qualification_manifest.py

.PHONY: java-api-dependency-graph java-api-member-dependencies java-api-visibility-constructor java-source-implementation-audit java-lang-native

java-api-dependency-graph:
	$(PYTHON) java_api_dependency_graph_suite.py

java-api-member-dependencies:
	$(PYTHON) java_api_member_dependencies_suite.py

java-api-visibility-constructor:
	$(PYTHON) java_api_visibility_constructor_suite.py

java-source-implementation-audit:
	$(PYTHON) java_source_implementation_audit_suite.py
	$(PYTHON) java_source_implementation_audit.py --top 40

java-lang-native:
	$(PYTHON) java_object_identity_native_suite.py
	$(PYTHON) java_string_native_suite.py
