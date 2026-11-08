# Makefile for the advantage trade examples.
#
# Consumes the existing advantage C/C++ model:
#   ../advantage.c            the C core (advantage_value / advantage_mean)
#   ../transport/digest.cpp   SHA-256 content addressing (needs OpenSSL)
# linked into one C++ program, trade_examples.

CC      ?= cc
CXX     ?= c++
CFLAGS  ?= -O2 -Wall -Wextra -std=c11
CXXFLAGS?= -O2 -Wall -Wextra -std=c++17

OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS   := $(shell pkg-config --libs openssl 2>/dev/null)
ifeq ($(strip $(OPENSSL_LIBS)),)
OPENSSL_LIBS := -lssl -lcrypto
endif

TARGET := trade_examples
OBJS   := trade_examples.o advantage_core.o digest.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(OPENSSL_LIBS)

trade_examples.o: trade_examples.cpp ../advantage.hpp ../advantage.h ../transport/digest.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c trade_examples.cpp -o trade_examples.o

# the C core, consumed as-is
advantage_core.o: ../advantage.c ../advantage.h
	$(CC) $(CFLAGS) -c ../advantage.c -o advantage_core.o

# the digest helper, consumed as-is
digest.o: ../transport/digest.cpp ../transport/digest.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c ../transport/digest.cpp -o digest.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
