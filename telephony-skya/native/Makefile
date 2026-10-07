CXX?=c++
CXXFLAGS?=-std=c++17 -O2 -Wall -Wextra -Wpedantic
all: skya libskya.a sleela-skya
# The embedded engine demo (existing).
skya: main.o skya_engine.o
	$(CXX) $(CXXFLAGS) $^ -o $@
# The reusable engine archive (existing).
libskya.a: skya_engine.o
	ar rcs $@ $^
# The sleela-skya command-line program: load/unload modules, start ordered
# components (SleelaServer, then SkyaGui). Links the CLI model, the entry
# point, and the Skya engine.
sleela-skya: sleela_skya_main.o sleela_skya_cli.o skya_engine.o
	$(CXX) $(CXXFLAGS) $^ -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
# Dependencies on headers so edits trigger rebuilds.
sleela_skya_main.o: sleela_skya_main.cpp sleela_skya_cli.h
sleela_skya_cli.o: sleela_skya_cli.cpp sleela_skya_cli.h skya_engine.h
clean:
	rm -f *.o skya libskya.a sleela-skya
.PHONY: all clean
