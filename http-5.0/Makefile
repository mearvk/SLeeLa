CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
all: http5_protocol.o http5_friends_pack.o
http5_protocol.o: http5_protocol.c http5_protocol.h
	$(CC) $(CFLAGS) -c http5_protocol.c
http5_friends_pack.o: http5_friends_pack.cpp http5_friends_pack.hpp
	$(CXX) $(CXXFLAGS) -c http5_friends_pack.cpp
test: all
	@echo "HTTP 5.0 compilation objects built successfully."
clean:
	rm -f *.o
