CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic

COMMON_CPP = debugger.cpp debugger_backend.cpp debugger_actions.cpp debugger_platform.cpp debugger_backend_linux.cpp debugger_backend_macos.cpp debugger_backend_windows.cpp
ENGINE_CPP = debug_engine.cpp
DIAGNOSTICS_CPP = debug_diagnostics.cpp
CONFORMANCE_CPP = debug_conformance.cpp

all: sleela-debugger debug-engine-test debug-diagnostics-test debug-conformance-test

sleela-debugger: main.cpp $(COMMON_CPP) debugger.hpp debugger_backend.hpp debugger_actions.hpp debugger_platform.hpp
	$(CXX) $(CXXFLAGS) main.cpp $(COMMON_CPP) -o $@

debug-engine-test: debug_engine_test.cpp $(ENGINE_CPP) debug_engine.hpp debugger.hpp debugger_backend.hpp
	$(CXX) $(CXXFLAGS) debug_engine_test.cpp $(ENGINE_CPP) -o $@

debug-diagnostics-test: debug_diagnostics_test.cpp $(DIAGNOSTICS_CPP) debug_diagnostics.hpp
	$(CXX) $(CXXFLAGS) debug_diagnostics_test.cpp $(DIAGNOSTICS_CPP) -o $@

debug-conformance-test: debug_conformance_test.cpp $(CONFORMANCE_CPP) debug_conformance.hpp
	$(CXX) $(CXXFLAGS) debug_conformance_test.cpp $(CONFORMANCE_CPP) -o $@

test:
	./tests/run-tests.sh
	./debug-engine-test
	./debug-diagnostics-test
	./debug-conformance-test

# Conformance tests validate the debugger's model/integration contract.
# They do not by themselves certify native Linux/macOS/Windows execution.
conformance: debug-conformance-test
	./debug-conformance-test

clean:
	rm -f sleela-debugger debug-engine-test debug-diagnostics-test debug-conformance-test
