CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
ROOT := ..
.PHONY: all clean check
all: memory reflection nordshrift runtime
memory: native_memory_smoke.cpp $(ROOT)/api/native/memory/sleela_memory_manager.cpp
	$(CXX) $(CXXFLAGS) -I$(ROOT)/api/native/memory $^ -o $@
reflection: reflection_smoke.cpp $(ROOT)/api/native/reflection/sleela_reflection.cpp
	$(CXX) $(CXXFLAGS) -I$(ROOT)/api/native/reflection $^ -o $@
nordshrift: nordshrift_complete_smoke.cpp $(ROOT)/impl/nordshrift/complete/class_bindings.cpp
	$(CXX) $(CXXFLAGS) -I$(ROOT)/impl/nordshrift/complete $^ -o $@
runtime: runtime_smoke.cpp $(ROOT)/api/native/runtime/sleela_runtime.cpp
	$(CXX) $(CXXFLAGS) -pthread -I$(ROOT)/api/native/runtime $^ -o $@
check: all
	./memory
	./reflection
	./nordshrift
	./runtime
clean:
	rm -f memory reflection nordshrift runtime
