# Makefile for the advantage module.
#
# Targets:
#   advantage - C++ demo (advantage.cpp) linked against the C core (advantage.c)
#   analyze   - loads advantages.xml into memory, reports on it, and can
#               optionally enrich it from remote knowledge sources via the
#               secure transport layer (links OpenSSL + transport/).
#               With --counsel it ranks the sources: the C advantage math is
#               applied to each source's recorded evidence, discounted by
#               confidence and by doubted assumptions (see counsel.hpp).
# Convenience: `make run` runs the demo, `make run-analyze` runs the loader,
#              `make run-counsel` prints the ranked counsel.

CC      ?= cc
CXX     ?= c++
CFLAGS  ?= -O2 -Wall -Wextra -std=c11
CXXFLAGS?= -O2 -Wall -Wextra -std=c++17

OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS   := $(shell pkg-config --libs openssl 2>/dev/null)
ifeq ($(strip $(OPENSSL_LIBS)),)
OPENSSL_LIBS := -lssl -lcrypto
endif

DEMO     := advantage
ANALYZER := analyze

# Transport objects the analyzer links against for enrichment.
T := transport
TRANSPORT_OBJS := \
	$(T)/acquire.o $(T)/http_client.o $(T)/sftp_client.o \
	$(T)/uri.o $(T)/digest.o

.PHONY: all clean run run-analyze run-counsel

all: $(DEMO) $(ANALYZER)

# --- advantage demo ---------------------------------------------------------
$(DEMO): advantage.o advantage_cpp.o
	$(CXX) $(CXXFLAGS) -o $@ advantage.o advantage_cpp.o -lm

advantage.o: advantage.c advantage.h
	$(CC) $(CFLAGS) -c advantage.c -o advantage.o

advantage_cpp.o: advantage.cpp advantage.hpp advantage.h
	$(CXX) $(CXXFLAGS) -c advantage.cpp -o advantage_cpp.o

# --- analyzer (XML loader + optional secure enrichment) ---------------------
$(ANALYZER): analyze.o advantage_store.o enrich.o counsel.o advantage.o \
            $(TRANSPORT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ analyze.o advantage_store.o enrich.o counsel.o \
		advantage.o $(TRANSPORT_OBJS) $(OPENSSL_LIBS) -lm

analyze.o: analyze.cpp advantage_store.hpp enrich.hpp counsel.hpp
	$(CXX) $(CXXFLAGS) -c analyze.cpp -o analyze.o

advantage_store.o: advantage_store.cpp advantage_store.hpp
	$(CXX) $(CXXFLAGS) -c advantage_store.cpp -o advantage_store.o

counsel.o: counsel.cpp counsel.hpp advantage_store.hpp advantage.hpp advantage.h
	$(CXX) $(CXXFLAGS) -c counsel.cpp -o counsel.o

enrich.o: enrich.cpp enrich.hpp advantage_store.hpp \
          $(T)/acquire.h $(T)/uri.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c enrich.cpp -o enrich.o

# Build the transport objects via their own Makefile so rules stay in one place.
$(TRANSPORT_OBJS):
	$(MAKE) -C $(T) $(notdir $@)

# --- convenience -------------------------------------------------------------
run: $(DEMO)
	./$(DEMO)

run-analyze: $(ANALYZER)
	./$(ANALYZER)

run-counsel: $(ANALYZER)
	./$(ANALYZER) --counsel

clean:
	rm -f $(DEMO) $(ANALYZER) *.o
	$(MAKE) -C $(T) clean
