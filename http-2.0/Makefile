CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra

# HTTP 2.1 protocol-core sketch (envelope + naming + pipeline). Pure data; no
# OpenSSL dependency. This is an early-generation sketch: envelope/response are
# implemented; naming/pipeline are stubs to be filled in.
PROTO_SOURCES = \
	h21_envelope.c \
	h21_naming.c \
	h21_pipeline.c

PROTO_OBJECTS = $(PROTO_SOURCES:.c=.o)

.PHONY: all proto syntax clean

all: proto

# Build the protocol-core objects (compile check for the sketch).
proto: $(PROTO_OBJECTS)

# Syntax-only check of every C source in the sketch.
syntax:
	$(CC) $(CFLAGS) -I. -fsyntax-only $(PROTO_SOURCES)

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
	rm -f $(PROTO_OBJECTS)
	rm -rf __pycache__
