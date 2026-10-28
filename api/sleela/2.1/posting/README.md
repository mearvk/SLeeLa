# Posting API

The C ABI is defined by [`sleela_post.h`](sleela_post.h) and implemented in [`sleela_post.c`](sleela_post.c). It validates a POST descriptor and writes an HTTP/1.1-style POST header and body to a caller-supplied file descriptor. It does not create a connection, perform DNS, or negotiate TLS.

## Build and tests

From this directory:

```sh
make all
make test
make clean
```

The aggregate Makefile builds `libsleela-post.a`, `libsleela-listener.a`, and `libsleela-router.a`. The test target exercises the three C interfaces' validation boundaries and exact router matching. A successful local run is not evidence of a cross-platform CI pass.
