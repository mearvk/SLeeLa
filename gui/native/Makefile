CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror

all: build/slgui_bridge_test build/slgui_theme_test

build:
	mkdir -p build

build/sleela_gui_bridge.o: sleela_gui_bridge.cpp sleela_gui_bridge.h | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/slgui_bridge_test.o: slgui_bridge_test.cpp sleela_gui_bridge.h | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/slgui_bridge_test: build/sleela_gui_bridge.o build/slgui_bridge_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

build/sleela_gui_theme.o: sleela_gui_theme.cpp sleela_gui_theme.h | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/slgui_theme_test.o: slgui_theme_test.cpp sleela_gui_theme.h | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/slgui_theme_test: build/sleela_gui_theme.o build/slgui_theme_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test: all
	./build/slgui_bridge_test
	./build/slgui_theme_test

clean:
	rm -rf build

.PHONY: all test clean
