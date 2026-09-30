CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic

.PHONY: all clean check jdk28-conformance jdk28-functional jdk28-signatures java-source-equivalence

all: memory reflection nordshrift runtime network-security

memory: native_memory_smoke.cpp ../api/native/memory/sleela_memory_manager.cpp
	$(CXX) $(CXXFLAGS) -I../api/native/memory $^ -o $@

reflection: reflection_smoke.cpp ../api/native/reflection/sleela_reflection.cpp
	$(CXX) $(CXXFLAGS) -I../api/native/reflection $^ -o $@

nordshrift: nordshrift_complete_smoke.cpp ../impl/nordshrift/complete/class_bindings.cpp
	$(CXX) $(CXXFLAGS) -I../impl/nordshrift/complete $^ -o $@

runtime: runtime_smoke.cpp ../api/native/runtime/sleela_runtime.cpp
	$(CXX) $(CXXFLAGS) -pthread -I../api/native/runtime $^ -o $@

network-security: network_security_smoke.cpp ../api/native/network/sleela_network.cpp ../api/native/security/sleela_security.cpp
	$(CXX) $(CXXFLAGS) $^ -lcrypto -o $@

check: all
	./memory
	./reflection
	./nordshrift
	./runtime
	./network-security

jdk28-conformance:
	python3 jdk28_functional_equivalence.py

jdk28-functional:
	python3 jdk28_functional_equivalence.py --functional

jdk28-signatures:
	python3 ../lib/java/tools/jdk28_signature_sync.py --check

java-source-equivalence:
	python3 java_source_equivalence.py --java java_equivalence/EquivalenceBasic.java --sleela java_equivalence/EquivalenceBasic.sleela --json java_equivalence/report.json
	python3 java_source_equivalence.py --java java_equivalence/ExpressionSurface.java --sleela java_equivalence/ExpressionSurface.sleela --json java_equivalence/expression-report.json

clean:
	rm -f memory reflection nordshrift runtime network-security
