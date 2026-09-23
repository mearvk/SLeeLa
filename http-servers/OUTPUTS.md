# HTTP Server Outputs and Artifacts

This document distinguishes source files, build outputs, runtime outputs, and
deployment inputs for the SLeeLa HTTP server grades.

## Source

Protocol source is kept under:

    http-servers/1/
    http-servers/2/
    http-servers/3/

The HTTP/3 transport adapter is:

    http-servers/3/http3/

The Grade 3 adapter intentionally delegates QUIC, TLS 1.3, loss recovery,
congestion control, stream transport, and QPACK to the selected QUIC backend.

## Build outputs

The standalone Makefiles produce local executables in their respective server
directories:

    http-servers/1/http-server-1
    http-servers/2/http-server-2
    http-servers/3/http-server-3

The HTTP/3 adapter can also be built independently as:

    http-servers/3/http3/http-server-3-http3

These are generated artifacts. They should not be treated as source.

The main SLeeLa build places its executable under:

    impl/build/sleela

The HTTP/2 and HTTP/3 adapter object files used by that executable are build
artifacts under:

    impl/build/http2_server.o
    impl/build/http3_server.o

## Runtime outputs

HTTP servers can produce:

- standard output;
- standard error;
- configured operational logs;
- HTTP response bodies;
- status/health responses;
- generated files only when an explicitly configured application operation
  writes them.

The HTTP/3 server itself does not create a repository file merely by accepting
a request.

## HTTP/3 deployment inputs

Grade 3 consumes:

    private key
    certificate
    backend executable

These are runtime/deployment inputs. They are not generated repository
outputs and should not be committed to Git.

## Recommended deployment separation

Use separate locations for:

1. source and documentation;
2. build artifacts;
3. configuration;
4. TLS credentials;
5. runtime logs;
6. application data.

This keeps credentials and operational data outside the source tree and makes
cleanup of generated build outputs deterministic.

## Cleanup

The standalone server Makefiles provide `make clean`. The main SLeeLa build
uses `impl/build/` for generated objects and executables.

A deployment should never use a source-tree cleanup command as a substitute
for log rotation, application-data retention, or credential management.
