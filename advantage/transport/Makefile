# Makefile for the advantage secure transport & acquisition module.
#
# Builds:
#   transport                       - the main CLI
#   bulkfetch                       - rate-limited parallel bulk fetcher CLI
#   libadvantagetransport.a         - static library
#   libadvantagetransport.so.X.Y.Z  - shared library (+ soname/dev symlinks)
#   advantagetransport.pc           - pkg-config metadata
#
# Targets: all (default), lib, shared, pc, test, install, uninstall, clean
#
# Library sources:
#   keystore.c      - AES-256-GCM encrypted secret store
#   kex.c           - Diffie-Hellman / ECDH key exchange + HKDF
#   digest.cpp      - SHA-256 content addressing
#   uri.cpp         - URI parsing
#   http_client.cpp - TLS-verified HTTP(S) client
#   sftp_client.cpp - SSH/SFTP via OpenSSH (scp)
#   acquire.cpp     - idempotent content-addressed acquisition facade
#   bulk.cpp        - rate-limited parallel bulk fetcher
#
# Requires OpenSSL (libssl/libcrypto) development headers and, at runtime for
# ssh/sftp fetches, the OpenSSH `scp` binary. Bulk fetching uses threads.

CC      ?= cc
CXX     ?= c++
CFLAGS  ?= -O2 -Wall -Wextra -std=c11
CXXFLAGS?= -O2 -Wall -Wextra -std=c++17
AR      ?= ar

# --- version & install layout ----------------------------------------------
VERSION      := 1.0.0
SOVERSION    := 1
NAME         := advantagetransport

PREFIX      ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BINDIR      ?= $(EXEC_PREFIX)/bin
LIBDIR      ?= $(EXEC_PREFIX)/lib
INCLUDEDIR  ?= $(PREFIX)/include/advantage/transport
PKGCONFIGDIR?= $(LIBDIR)/pkgconfig
INSTALL     ?= install

OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS   := $(shell pkg-config --libs openssl 2>/dev/null)
ifeq ($(strip $(OPENSSL_LIBS)),)
OPENSSL_LIBS := -lssl -lcrypto
endif
LDLIBS_EXTRA := $(OPENSSL_LIBS) -lpthread

TARGET    := transport
BULK      := bulkfetch
STATICLIB := lib$(NAME).a
SHAREDLIB := lib$(NAME).so
SHARED_V  := $(SHAREDLIB).$(VERSION)
SHARED_SO := $(SHAREDLIB).$(SOVERSION)
PC        := $(NAME).pc

# Objects that make up the reusable library (everything except the CLI mains).
LIB_C_OBJS   := keystore.o kex.o
LIB_CPP_OBJS := digest.o uri.o http_client.o sftp_client.o acquire.o bulk.o
LIB_OBJS     := $(LIB_C_OBJS) $(LIB_CPP_OBJS)

# Position-independent objects for the shared library (parallel .pic.o set).
PIC_OBJS     := $(LIB_OBJS:.o=.pic.o)

# Public headers to install.
HEADERS := keystore.h kex.h digest.h uri.h http_client.h sftp_client.h \
           acquire.h bulk.h

.PHONY: all clean test lib shared pc install uninstall

all: $(TARGET) $(BULK) $(STATICLIB) $(SHAREDLIB)

lib:    $(STATICLIB)
shared: $(SHAREDLIB)
pc:     $(PC)

# --- static library ---------------------------------------------------------
$(STATICLIB): $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

# --- shared library (soname + versioned file + dev symlink) -----------------
$(SHARED_V): $(PIC_OBJS)
	$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$(SHARED_SO) -o $@ $(PIC_OBJS) $(LDLIBS_EXTRA)

$(SHAREDLIB): $(SHARED_V)
	ln -sf $(SHARED_V) $(SHARED_SO)
	ln -sf $(SHARED_V) $(SHAREDLIB)

# --- CLIs link against the static library -----------------------------------
$(TARGET): transport_cli.o $(STATICLIB)
	$(CXX) $(CXXFLAGS) -o $@ transport_cli.o $(STATICLIB) $(LDLIBS_EXTRA)

$(BULK): bulk_cli.o $(STATICLIB)
	$(CXX) $(CXXFLAGS) -o $@ bulk_cli.o $(STATICLIB) $(LDLIBS_EXTRA)

# --- static (non-PIC) objects ----------------------------------------------
keystore.o: keystore.c keystore.h
	$(CC) $(CFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@
kex.o: kex.c kex.h
	$(CC) $(CFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@
digest.o: digest.cpp digest.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@
uri.o: uri.cpp uri.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
http_client.o: http_client.cpp http_client.h uri.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@
sftp_client.o: sftp_client.cpp sftp_client.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
acquire.o: acquire.cpp acquire.h digest.h uri.h http_client.h sftp_client.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@
bulk.o: bulk.cpp bulk.h acquire.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

transport_cli.o: transport_cli.cpp acquire.h kex.h keystore.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@
bulk_cli.o: bulk_cli.cpp bulk.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

# --- PIC objects for the shared library -------------------------------------
keystore.pic.o: keystore.c keystore.h
	$(CC) $(CFLAGS) -fPIC $(OPENSSL_CFLAGS) -c $< -o $@
kex.pic.o: kex.c kex.h
	$(CC) $(CFLAGS) -fPIC $(OPENSSL_CFLAGS) -c $< -o $@
digest.pic.o: digest.cpp digest.h
	$(CXX) $(CXXFLAGS) -fPIC $(OPENSSL_CFLAGS) -c $< -o $@
uri.pic.o: uri.cpp uri.h
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@
http_client.pic.o: http_client.cpp http_client.h uri.h
	$(CXX) $(CXXFLAGS) -fPIC $(OPENSSL_CFLAGS) -c $< -o $@
sftp_client.pic.o: sftp_client.cpp sftp_client.h
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@
acquire.pic.o: acquire.cpp acquire.h digest.h uri.h http_client.h sftp_client.h
	$(CXX) $(CXXFLAGS) -fPIC $(OPENSSL_CFLAGS) -c $< -o $@
bulk.pic.o: bulk.cpp bulk.h acquire.h
	$(CXX) $(CXXFLAGS) -fPIC $(OPENSSL_CFLAGS) -c $< -o $@

# --- pkg-config metadata (generated so paths track the install prefix) ------
$(PC):
	@printf '%s\n' \
	  'prefix=$(PREFIX)' \
	  'exec_prefix=$(EXEC_PREFIX)' \
	  'libdir=$(LIBDIR)' \
	  'includedir=$(PREFIX)/include' \
	  '' \
	  'Name: advantage-transport' \
	  'Description: Secure, idempotent acquisition layer (HTTPS/SFTP/SSH, DH/ECDH, keystore)' \
	  'Version: $(VERSION)' \
	  'Requires.private: openssl' \
	  'Libs: -L$${libdir} -l$(NAME)' \
	  'Libs.private: -lpthread' \
	  'Cflags: -I$${includedir}/advantage/transport' \
	  > $(PC)

# --- safety + functional battery -------------------------------------------
test: $(TARGET) $(BULK) $(STATICLIB)
	@bash test.sh

# --- install / uninstall ----------------------------------------------------
install: all $(PC)
	$(INSTALL) -d $(DESTDIR)$(BINDIR) $(DESTDIR)$(LIBDIR) \
	             $(DESTDIR)$(INCLUDEDIR) $(DESTDIR)$(PKGCONFIGDIR)
	$(INSTALL) -m 0755 $(TARGET) $(DESTDIR)$(BINDIR)/
	$(INSTALL) -m 0755 $(BULK)   $(DESTDIR)$(BINDIR)/
	$(INSTALL) -m 0644 $(STATICLIB) $(DESTDIR)$(LIBDIR)/
	$(INSTALL) -m 0755 $(SHARED_V)  $(DESTDIR)$(LIBDIR)/
	ln -sf $(SHARED_V) $(DESTDIR)$(LIBDIR)/$(SHARED_SO)
	ln -sf $(SHARED_V) $(DESTDIR)$(LIBDIR)/$(SHAREDLIB)
	for h in $(HEADERS); do $(INSTALL) -m 0644 $$h $(DESTDIR)$(INCLUDEDIR)/; done
	$(INSTALL) -m 0644 $(PC) $(DESTDIR)$(PKGCONFIGDIR)/

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET) $(DESTDIR)$(BINDIR)/$(BULK)
	rm -f $(DESTDIR)$(LIBDIR)/$(STATICLIB)
	rm -f $(DESTDIR)$(LIBDIR)/$(SHARED_V) $(DESTDIR)$(LIBDIR)/$(SHARED_SO) \
	      $(DESTDIR)$(LIBDIR)/$(SHAREDLIB)
	rm -f $(DESTDIR)$(PKGCONFIGDIR)/$(PC)
	rm -rf $(DESTDIR)$(INCLUDEDIR)

clean:
	rm -f $(TARGET) $(BULK) $(STATICLIB) \
	      $(SHAREDLIB) $(SHARED_SO) $(SHARED_V) $(PC) \
	      *.o *.pic.o
