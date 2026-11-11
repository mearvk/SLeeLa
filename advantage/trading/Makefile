# Makefile for the trading models (C core + C++ layer + demo).
#
# Needs no OpenSSL and no network: the models are pure computation and link
# only the advantage core, store and counsel from the parent directory.
#
#   make        builds ./trading_demo
#   make run    builds and runs it
#   make clean

CC       ?= cc
CXX      ?= c++
CFLAGS   ?= -O2 -Wall -Wextra -std=c11
CXXFLAGS ?= -O2 -Wall -Wextra -std=c++17

P := ..

.PHONY: all run clean

all: trading_demo

trading_demo: trading_demo.o trade_models_cpp.o trade_models.o \
              adv_core.o adv_store.o adv_counsel.o
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

trade_models.o: trade_models.c trade_models.h $(P)/advantage.h
	$(CC) $(CFLAGS) -c trade_models.c -o $@

trade_models_cpp.o: trade_models.cpp trade_models.hpp trade_models.h \
                    $(P)/advantage_store.hpp
	$(CXX) $(CXXFLAGS) -c trade_models.cpp -o $@

trading_demo.o: trading_demo.cpp trade_models.hpp trade_models.h \
                $(P)/counsel.hpp $(P)/advantage_store.hpp
	$(CXX) $(CXXFLAGS) -c trading_demo.cpp -o $@

# Parent-directory sources, compiled here under distinct names so this build
# never collides with the parent's own object files.
adv_core.o: $(P)/advantage.c $(P)/advantage.h
	$(CC) $(CFLAGS) -c $(P)/advantage.c -o $@

adv_store.o: $(P)/advantage_store.cpp $(P)/advantage_store.hpp
	$(CXX) $(CXXFLAGS) -c $(P)/advantage_store.cpp -o $@

adv_counsel.o: $(P)/counsel.cpp $(P)/counsel.hpp $(P)/advantage_store.hpp \
               $(P)/advantage.hpp $(P)/advantage.h
	$(CXX) $(CXXFLAGS) -c $(P)/counsel.cpp -o $@

run: trading_demo
	./trading_demo

clean:
	rm -f trading_demo *.o
