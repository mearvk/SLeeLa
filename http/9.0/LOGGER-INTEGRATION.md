# HTTP 9.0 Logger Integration

HTTP 9.0 follows the common SLeeLa packet-logging contract.

Incoming: OS socket -> HTTP 9 receiver -> SLeeLa Server packet boundary -> /logger.

Outgoing: HTTP 9 response -> SLeeLa Server -> /logger -> OS socket.

Preserve endpoint, stream, request, sequence, packet-size, admission, and protocol metadata when available. Raw socket observation is the secondary endpoint-awareness path. Use /resolver for dynamic endpoint resolution when forwarding requires it.