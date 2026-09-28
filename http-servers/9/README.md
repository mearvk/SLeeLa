# SLeeLa HTTP 9.0 Server

Runnable semantic listener corresponding to `http/9.0` and `http-9.0`. HTTP 9.0 is an experimental SLeeLa application protocol, not an IETF HTTP/9 standard.

The first bounded request line is the handshake. Protocol-specific metadata is returned after the gate succeeds. Logs contain request and protocol state, not payload contents.

Build: `make`
Run: `./http-server-9 --addr 127.0.0.1 --port 8409 --once`
