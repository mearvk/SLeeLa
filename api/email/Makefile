CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS := $(shell pkg-config --libs openssl 2>/dev/null)
TARGET := libsleela_email.a
OBJ := sleela_email.o

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $^

sleela_email.o: sleela_email.c sleela_email.h
	$(CC) $(CFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

cpp-check: sleela_email.hpp sleela_email.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -I. -x c++ -fsyntax-only sleela_email.hpp

clean:
	rm -f $(OBJ) $(TARGET)
