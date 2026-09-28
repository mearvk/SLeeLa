CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic

COMMON_CPP = debugger.cpp debugger_backend.cpp debugger_actions.cpp debugger_platform.cpp debugger_backend_linux.cpp debugger_backend_macos.cpp debugger_backend_windows.cpp

ENGINE_CPP = debug_engine.cpp
DIAGNOSTICS_CPP = debug_diagnostics.cpp
ENGINE_HPP = debug_engine.hpp

all: sleela-debugger debug-engine-test debug-diagnostics-test

sleela-debugger: main.cpp $(COMMON_CPP) debugger.hpp debugger_backend.hpp debugger_actions.hpp debugger_platform.hpp
	$(CXX) $(CXXFLAGS) main.cpp $(COMMON_CPP) -o $@

debug-engine-test: debug_engine_test.cpp $(ENGINE_CPP) $(ENGINE_HPP) debugger.hpp debugger_backend.hpp
	$(CXX) $(CXXFLAGS) debug_engine_test.cpp $(ENGINE_CPP) -o $@

debug-diagnostics-test: debug_diagnostics_test.cpp $(DIAGNOSTICS_CPP) debug_diagnostics.hpp
	$(CXX) $(CXXFLAGS) debug_diagnostics_test.cpp $(DIAGNOSTICS_CPP) -o $@

test:
	./tests/run-tests.sh
	./debug-engine-test
	./debug-diagnostics-test

clean:
	rm -f sleela-debugger debug-engine-test debug-diagnostics-test
