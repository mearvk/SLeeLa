# HTTP 8.0 Logger Integration

HTTP 8.0 follows the common SLeeLa packet-logging contract, including Subscription/Radio traffic.

Incoming: OS socket -> HTTP 8 receiver -> SLeeLa Server packet boundary -> /logger.

Outgoing: HTTP 8 response/radio packet -> SLeeLa Server -> /logger -> OS socket.

Preserve endpoint, stream, request, sequence, packet-size, admission, protocol-generation, and transport-state metadata. Raw socket observation is the secondary endpoint-awareness path. Use /resolver for dynamic endpoint resolution when forwarding requires it.