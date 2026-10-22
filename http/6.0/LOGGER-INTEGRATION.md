# HTTP 6.0 Logger Integration

HTTP 6.0 is required to follow the common SLeeLa packet-logging contract. As of this branch, the HTTP 6.0 server has not yet wired calls into the shared /logger implementation; this document is the target contract, not proof of runtime integration.

Incoming: OS socket -> HTTP 6 receiver -> SLeeLa Server packet boundary -> /logger.

Outgoing: HTTP 6 response -> SLeeLa Server -> /logger -> OS socket.

Preserve endpoint, stream, request, sequence, packet-size, admission, and protocol metadata when available. Raw socket observation is the secondary endpoint-awareness path. Use /resolver for dynamic endpoint resolution when forwarding requires it.

Before production release, add a tested bridge that records ingress before HTTP parsing (including parse failures) and egress immediately before socket writes. Use SLeeLa-PacketLog-1 JSON Lines records, the shared 240 MiB segment boundary, and payload logging disabled by default. The logger must record outcomes, not make routing or admission decisions. Until that bridge is wired and tested, do not claim HTTP 6.0 runtime packet logging is implemented.