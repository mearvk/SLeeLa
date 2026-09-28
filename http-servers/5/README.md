# SLeeLa HTTP 5.0 Server

Native listener for the SLeeLa HTTP 5.0 application protocol. It is not an IETF HTTP/5 standard.

The binary packet header is 32 bytes:
VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH
followed by PAYLOAD. Fields are network byte order. The first frame must be OPEN with sequence 0. Packet identity is logged without payload contents.

HTTP 5.0 adds FRIENDS_PACK, BONUS_OFFER, FP_UPDATE and AUDIT.

Build: `make`
Run: `./http-server-5 --addr 127.0.0.1 --port 8405 --once`
