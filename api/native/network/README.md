# Native Networking Foundation

Portable API with a POSIX implementation for UDP/TCP sockets and DNS-backed endpoint resolution.

The current implementation is intentionally a foundation: timeouts, IPv6-specific binding behavior, nonblocking event integration, TLS, connection pooling, retry policy, and Windows/macOS socket adapters remain separate work.

Applications should use the higher-level SLeeLa NetworkEndpoint/UdpTransport/TcpTransport abstractions rather than depending on OS calls directly.