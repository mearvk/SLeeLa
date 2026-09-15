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

# Rendering stack: math -> group -> city renderer.
RENDER_SRC := render_math.cpp render_group.cpp
CITY_SRC := city_model.cpp city_renderer.cpp $(RENDER_SRC)
CLI_SRC  := city3d.cpp $(CITY_SRC) $(PHRAIGN_SRC)
SMOKE_SRC := city3d_smoke.cpp $(CITY_SRC) $(PHRAIGN_SRC)
RENDER_SMOKE_SRC := render_smoke.cpp $(RENDER_SRC) $(PHRAIGN_SRC)

# Rebuild when any header changes too.
HEADERS := city_model.hpp city_renderer.hpp render_math.hpp render_group.hpp \
           ../pixel_terminal.hpp

CITY3D := $(BUILD)/city3d
SMOKE  := $(BUILD)/city3d_smoke
RENDER_SMOKE := $(BUILD)/render_smoke

.PHONY: all smoke test clean demo

all: $(CITY3D) $(SMOKE) $(RENDER_SMOKE)

$(CITY3D): $(CLI_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(CLI_SRC) -o $@

$(SMOKE): $(SMOKE_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SMOKE_SRC) -o $@

$(RENDER_SMOKE): $(RENDER_SMOKE_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(RENDER_SMOKE_SRC) -o $@

$(BUILD):
	mkdir -p $(BUILD)

# Run all smoke tests.
smoke test: $(SMOKE) $(RENDER_SMOKE)
	$(RENDER_SMOKE)
	$(SMOKE)

# Generate a city from the default config and write a model + PPM preview.
demo: $(CITY3D)
	$(CITY3D) generate --config city.config --out $(BUILD)/demo.city --ppm $(BUILD)/demo.ppm

clean:
	rm -rf $(BUILD)
