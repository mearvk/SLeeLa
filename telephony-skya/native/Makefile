CXX?=c++
CXXFLAGS?=-std=c++17 -O2 -Wall -Wextra -Wpedantic -I../../impl/core -I../.. -pthread
SKYA_OBJS=skya_engine.o skya_policy.o skya_sleela_bridge.o
all: skya libskya.a
skya: main.o $(SKYA_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ 
libskya.a: $(SKYA_OBJS)
	ar rcs $@ $^
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f *.o skya libskya.a
.PHONY: all clean
