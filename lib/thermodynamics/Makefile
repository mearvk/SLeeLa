# SLeeLa Thermodynamics IV package build
# Max Rupplin - MEARVK LLC - 2026
CC ?= cc
CXX ?= c++
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
LDLIBS ?= -lm
BUILD_DIR ?= build

C_SOURCES := src/thermodynamics.c src/thermo_stochastic.c src/thermo_slots.c
CPP_SOURCES := src/thermodynamics.cpp
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.c.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.cpp.o,$(CPP_SOURCES))
LIB_C_OBJECTS := $(BUILD_DIR)/thermodynamics.c.o $(BUILD_DIR)/thermo_stochastic.c.o $(BUILD_DIR)/thermo_slots.c.o

.PHONY: all native selftest sanity clean help
all: native selftest sanity
native: $(C_OBJECTS) $(CPP_OBJECTS)

# Behavioral self-test: links the C library objects with each driver and runs
# the assertions. Also compiles the C++ orchestration layer to prove the
# C ABI + C++ boundary builds cleanly.
selftest: $(LIB_C_OBJECTS) $(BUILD_DIR)/thermodynamics.cpp.o | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/vignette_demo.c $(LIB_C_OBJECTS) -o $(BUILD_DIR)/vignette_demo $(LDLIBS)
	$(BUILD_DIR)/vignette_demo
	$(CC) $(CPPFLAGS) $(CFLAGS) src/stochastic_demo.c $(LIB_C_OBJECTS) -o $(BUILD_DIR)/stochastic_demo $(LDLIBS)
	$(BUILD_DIR)/stochastic_demo
	$(CC) $(CPPFLAGS) $(CFLAGS) src/slots_demo.c $(LIB_C_OBJECTS) -o $(BUILD_DIR)/slots_demo $(LDLIBS)
	$(BUILD_DIR)/slots_demo
	$(CC) $(CPPFLAGS) $(CFLAGS) src/courses_demo.c $(LIB_C_OBJECTS) -o $(BUILD_DIR)/courses_demo $(LDLIBS)
	$(BUILD_DIR)/courses_demo

sanity:
	@printf '%s\n' 'SLeeLa Thermodynamics IV package sanity check'
	@printf '%s\n' '  scalar relations: sensible heat, Fourier flux, Newton cooling, Carnot efficiency'
	@printf '%s\n' '  statistical mechanics: partition function, Boltzmann probability, mean energy'
	@printf '%s\n' '  multivariable (3D model): gradient, divergence, Laplacian, FTCS heat equation'
	@printf '%s\n' '  stochastic: Langevin noise step, Monte Carlo over futures, Arrhenius log-rate'
	@printf '%s\n' '  slot caps (enforced): futures<=22, positive gains<=6, long-term confidences<=2'
	@printf '%s\n' '  courses: Thermo I/II, statistical mechanics, advanced/chemical (SLeeLa + C self-test)'
	@printf '%s\n' '  native boundary: C ABI (thermodynamics.c) + C++ orchestration (thermodynamics.cpp)'
	@printf '%s\n' '  stability rule: FTCS requires dt <= h^2 / (6 alpha)'

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.c.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.cpp.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(BUILD_DIR)
help:
	@printf '%s\n' 'SLeeLa Thermodynamics IV package build'
	@printf '%s\n' '  make all       Build objects, run the vignette self-test, print sanity'
	@printf '%s\n' '  make native    Compile C and C++ objects only'
	@printf '%s\n' '  make selftest  Build and run the worked-example assertions'
	@printf '%s\n' '  make clean     Remove build artifacts'
