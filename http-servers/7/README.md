# SLeeLa HTTP 7.0 Server

Runnable semantic listener corresponding to `http/7.0` and `http-7.0`. HTTP 7.0 is an experimental SLeeLa application protocol, not an IETF HTTP/7 standard.

The first bounded request line is the handshake. Protocol-specific metadata is returned after the gate succeeds. Logs contain request and protocol state, not payload contents.

Build: `make`
Run: `./http-server-7 --addr 127.0.0.1 --port 8407 --once`
