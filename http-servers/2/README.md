<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">


# SLeeLa HTTP Server Grade 2

Native HTTP/2 server for the SLeeLa HTTP 2.0/2.1 grade. Default TCP port: 8081.

Build with `make -C http-servers/2`; the implementation uses nghttp2 for RFC 9113 frame/stream and HPACK state handling. See `http-servers/2/http2/README.md`.