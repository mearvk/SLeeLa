CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
ROOT := ..
.PHONY: all clean check
all: memory reflection nordshrift
memory: native_memory_smoke.cpp $(ROOT)/api/native/memory/sleela_memory_manager.cpp
	$(CXX) $(CXXFLAGS) -I$(ROOT)/api/native/memory $^ -o $@
reflection: reflection_smoke.cpp $(ROOT)/api/native/reflection/sleela_reflection.cpp
	$(CXX) $(CXXFLAGS) -I$(ROOT)/api/native/reflection $^ -o $@
nordshrift: nordshrift_complete_smoke.cpp $(ROOT)/impl/nordshrift/complete/class_bindings.cpp
	$(CXX) $(CXXFLAGS) -I$(ROOT)/impl/nordshrift/complete $^ -o $@
check: all
	./memory
	./reflection
	./nordshrift
clean:
	rm -f memory reflection nordshrift
