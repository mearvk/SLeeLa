[![SLeeLa](https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png)](https://github.com/mearvk/SLeeLa)
# Native Networking Foundation

Portable POSIX networking foundation for UDP/TCP sockets and DNS-backed endpoint resolution.

This is a foundation, not a claim of complete networking. TLS, certificate validation, nonblocking event integration, explicit timeouts, retry/backoff, pooling, and Windows/macOS adapters remain subsequent packages.

SLeeLa applications should consume the higher-level NetworkEndpoint, UdpTransport, and TcpTransport abstractions.