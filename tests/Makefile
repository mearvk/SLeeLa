CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -pedantic
CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
LDLIBS ?= -lm

.PHONY: all clean check ai-neural-nets-xor neural-nets-xor ai-character-recognition character-recognition-smoke jdk28-conformance jdk28-functional jdk28-signatures java-source-equivalence java-conversions java-flow java-exceptions java-api-dependencies java-constraints java-signature-comparison java-platform-qualification

all: memory reflection nordshrift runtime network-security neural-nets-xor character-recognition-smoke

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

neural-nets-xor: ../artificial-intelligence/neural-nets/xor_example.c ../artificial-intelligence/neural-nets/neural_nets.c ../artificial-intelligence/neural-nets/neural_nets.h
	$(CC) $(CFLAGS) -I../artificial-intelligence/neural-nets ../artificial-intelligence/neural-nets/xor_example.c ../artificial-intelligence/neural-nets/neural_nets.c $(LDLIBS) -o $@

ai-neural-nets-xor: neural-nets-xor
	./neural-nets-xor

character-recognition-smoke: character_recognition_smoke.c ../artificial-intelligence/character-recognition/character_recognition.c ../artificial-intelligence/character-recognition/character_recognition.h
	$(CC) $(CFLAGS) -I../artificial-intelligence/character-recognition $< ../artificial-intelligence/character-recognition/character_recognition.c $(LDLIBS) -o $@

ai-character-recognition: character-recognition-smoke
	./character-recognition-smoke

check: all
	./memory
	./reflection
	./nordshrift
	./runtime
	./network-security
	./neural-nets-xor
	./character-recognition-smoke

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
	rm -f memory reflection nordshrift runtime network-security neural-nets-xor character-recognition-smoke character-recognition-smoke.txt character-recognition-smoke.jsonl

java-conversions:
	$(CXX) $(CXXFLAGS) -I../impl/frontend java_conversion_semantics.cpp ../impl/frontend/java_equivalence.cpp -o java-conversions
	./java-conversions

java-overload:
	$(CXX) $(CXXFLAGS) -I../impl/frontend java_overload_override.cpp ../impl/frontend/java_equivalence.cpp -o java-overload
	./java-overload

java-flow:
	python3 java_flow_suite.py

java-exceptions:
	python3 java_exceptions_suite.py

java-api-dependencies:
	python3 java_api_dependency_suite.py

java-constraints:
	python3 java_constraints_suite.py

java-signature-comparison:
	python3 java_signature_comparison_suite.py

java-platform-qualification:
	python3 java_platform_qualification_suite.py

java-qualification:
	python3 java_qualification_manifest.py
