# SLeeLa HTTP 4.0 Server

Native listener for the SLeeLa HTTP 4.0 application protocol. It is not an IETF HTTP/4 standard.

The binary packet header is 32 bytes:
VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH
followed by PAYLOAD. Fields are network byte order. The first frame must be OPEN with sequence 0. Packet identity is logged without payload contents.

HTTP 4.0 frame types: OPEN, DATA, END, RESET, WINDOW, PING, PONG, RESUME, CAPSULE.

Build: `make`
Run: `./http-server-4 --addr 127.0.0.1 --port 8404 --once`
