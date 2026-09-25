CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
.PHONY: all clean check
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
clean:
	rm -f memory reflection nordshrift runtime network-security
