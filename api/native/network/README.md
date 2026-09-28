# Native Networking Foundation

Portable POSIX networking foundation for UDP/TCP sockets and DNS-backed endpoint resolution.

This is a foundation, not a claim of complete networking. TLS, certificate validation, nonblocking event integration, explicit timeouts, retry/backoff, pooling, and Windows/macOS adapters remain subsequent packages.

SLeeLa applications should consume the higher-level NetworkEndpoint, UdpTransport, and TcpTransport abstractions.