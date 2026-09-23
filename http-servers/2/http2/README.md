# SLeeLa HTTP/2 Server

Native HTTP/2 transport for SLeeLa Grade 2 (the repository's HTTP 2.0/2.1 naming).

RFC 9113 defines HTTP/2 as binary framed, stream-multiplexed HTTP semantics; HPACK is defined by RFC 7541. SLeeLa delegates the protocol state machine and HPACK implementation to nghttp2 rather than maintaining a partial handwritten compressor. citeturn0search1turn0search2

Build:
```sh
make -C http-servers/2/http2
```

Run:
```sh
./http-server-2-h2 --port 8081 --root ./www
```

The SLeeLa CLI remains:
```sh
sleela http-server 2
```

The logical SLeeLa HTTP PORT is separate from the HTTP/2 stream identifier. Stream IDs are transport state; SLeeLa logical PORT values remain application routing state.
