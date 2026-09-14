# =============================================================================
# Phraign(TM) City 3D -- sprawling city model rendered on the Phraign frame.
# Self-contained build; includes the Phraign native layer from the parent bash/
# directory (../pixel_terminal.hpp / ../pixel_terminal.cpp).
# =============================================================================
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
CPPFLAGS += -I. -I..

BUILD := build
PHRAIGN_SRC := ../pixel_terminal.cpp

CITY_SRC := city_model.cpp city_renderer.cpp
CLI_SRC  := city3d.cpp $(CITY_SRC) $(PHRAIGN_SRC)
SMOKE_SRC := city3d_smoke.cpp $(CITY_SRC) $(PHRAIGN_SRC)

# Rebuild when any header changes too.
HEADERS := city_model.hpp city_renderer.hpp ../pixel_terminal.hpp

CITY3D := $(BUILD)/city3d
SMOKE  := $(BUILD)/city3d_smoke

.PHONY: all smoke test clean demo

all: $(CITY3D) $(SMOKE)

$(CITY3D): $(CLI_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(CLI_SRC) -o $@

$(SMOKE): $(SMOKE_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SMOKE_SRC) -o $@

$(BUILD):
	mkdir -p $(BUILD)

# Run the smoke test.
smoke test: $(SMOKE)
	$(SMOKE)

# Generate a city from the default config and write a model + PPM preview.
demo: $(CITY3D)
	$(CITY3D) generate --config city.config --out $(BUILD)/demo.city --ppm $(BUILD)/demo.ppm

clean:
	rm -rf $(BUILD)
