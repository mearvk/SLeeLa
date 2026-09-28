# SLeeLa HTTP 6.0 Server

Native listener for the SLeeLa HTTP 6.0 application protocol. It is not an IETF HTTP/6 standard.

The binary packet header is 32 bytes:
VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH
followed by PAYLOAD. Fields are network byte order. The first frame must be OPEN with sequence 0. Packet identity is logged without payload contents.

HTTP 6.0 adds the HTTP 5.0 extensions plus CONSOLIDATED_FRIENDS_BET, TEAMSTER_DEBATE, CONSOLIDATE_IQ, TEAM_AREA, DEBATE_TOPIC, DEBATE_POSITION and RECIPIENT_LABEL.

Build: `make`
Run: `./http-server-6 --addr 127.0.0.1 --port 8406 --once`
