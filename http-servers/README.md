# SLeeLa HTTP Servers

Three native server grades live under `http-servers/1`, `2`, and `3`, sharing `common/`.

Grade 2 and Grade 3 are transport-specific implementations. Grade 2 uses native HTTP/2 framing over TCP; Grade 3 uses HTTP/3 over QUIC/UDP. They do not falsely treat either protocol as HTTP/1 text. Lower-protocol compatibility is a separate service/dispatch concern and is not claimed by the dedicated Grade 2/3 listeners yet.

Configuration, logging, and generated-output rules are documented in [`CONFIGURATION.md`](CONFIGURATION.md), [`LOGGING.md`](LOGGING.md), and [`OUTPUTS.md`](OUTPUTS.md). HTTP/3 additionally requires deployment-managed TLS key/certificate material and a QUIC-capable backend.

The HTTP/1.x parser follows the repository's HTTP specifications plus RFC 1945 for HTTP/1.0 and RFC 9112 for current HTTP/1.1 message framing.

## Command line
```text
sleela http-server 1
sleela http-server 2
sleela http-server 3
```

Optional flags: `--port N`, `--threads N`, `--root DIR`, `--log FILE`, `--once`.

Defaults are TCP 8080/8081/8082.

## Careful architecture
- bounded worker pool and connection queue;
- bounded headers and request bodies;
- RAII ownership for buffers/files/threads;
- synchronized operational logging;
- no request bodies or query strings in ordinary logs;
- strict framing checks;
- bounded chunked decoding;
- HTTP/1.1 Host enforcement;
- keep-alive and timeouts;
- percent-decoding and traversal containment;
- graceful shutdown;
- explicit separation between HTTP semantics and transport framing.

Apache's documented modular MPM model and Tomcat's connector/request-worker model were used as architectural references, not copied code.

## Native builds
```sh
make -C http-servers/1
make -C http-servers/2
make -C http-servers/3
```

The common source is reusable by future HTTP/2 and HTTP/3 transport adapters.
