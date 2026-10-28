# Router API

The C ABI is defined by [`sleela_router.h`](sleela_router.h) and implemented in [`sleela_router.c`](sleela_router.c). The current contract validates a method, absolute path, and absolute target, then matches **exactly** on method and path. It does not implement wildcard routes, path parameters, middleware, precedence rules, or request dispatch.

## Build and tests

The router object and static library are built by the aggregate [`../posting/Makefile`](../posting/Makefile). From `api/posting/`, run `make all` to build the posting, listener, and router static libraries, or `make test` to run the shared validation tests.

Invalid descriptors return `-1` from validation and a nonmatching or null input returns `0` from the match function. These are descriptor-validation and exact-match helpers, not a network server.
