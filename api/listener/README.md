# Listener API

The C ABI is defined by [`sleela_listener.h`](sleela_listener.h) and implemented in [`sleela_listener.c`](sleela_listener.c). It validates a descriptor containing `bind_host`, a nonzero port, the currently accepted `http` protocol, and an absolute route path. It does **not** open a socket or run a listener.

## Build and tests

The listener object and static library are built by the aggregate [`../posting/Makefile`](../posting/Makefile). From `api/posting/`, run `make all` to build the posting, listener, and router static libraries, or `make test` to run the shared validation tests.

Validation errors are written to the caller's buffer when one is supplied; invalid descriptors return `-1`. Keep these checks distinct from transport, socket binding, and server lifecycle behavior, which are not implemented by this descriptor validator.
