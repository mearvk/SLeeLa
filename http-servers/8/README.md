# SLeeLa HTTP 8.0 Server

Runnable semantic listener corresponding to `http/8.0` and `http-8.0`. HTTP 8.0 is an experimental SLeeLa application protocol, not an IETF HTTP/8 standard.

The first bounded request line is the handshake. Protocol-specific metadata is returned after the gate succeeds. Logs contain request and protocol state, not payload contents.

Build: `make`
Run: `./http-server-8 --addr 127.0.0.1 --port 8408 --once`
