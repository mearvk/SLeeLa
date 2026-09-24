CXX?=c++
CXXFLAGS?=-std=c++17 -O2 -Wall -Wextra -Wpedantic
all: skya libskya.a
skya: main.o skya_engine.o
	$(CXX) $(CXXFLAGS) $^ -o $@
libskya.a: skya_engine.o
	ar rcs $@ $^
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f *.o skya libskya.a
.PHONY: all clean
