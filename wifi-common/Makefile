CC ?= cc
CXX ?= c++
AR ?= ar
CFLAGS ?= -O2 -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic
CPPFLAGS ?= -Iinclude
.PHONY: all test clean
all: libsleela_wifi.a
wifi_manager.o: src/wifi_manager.c include/sleela_wifi/wifi_manager.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
wifi_manager_cpp.o: src/wifi_manager_cpp.cpp include/sleela_wifi/wifi_manager_cpp.hpp
	$(CXX) $(CPPFLAGS) -O2 -std=c++17 -Wall -Wextra -Wpedantic -c $< -o $@
libsleela_wifi.a: wifi_manager.o
	$(AR) rcs $@ $^
test: tests/test_wifi_manager.c src/wifi_manager.c
	$(CC) $(CPPFLAGS) $(CFLAGS) src/wifi_manager.c tests/test_wifi_manager.c -o wifi-manager-test
	./wifi-manager-test
clean:
	rm -f wifi_manager.o wifi_manager_cpp.o libsleela_wifi.a wifi-manager-test
