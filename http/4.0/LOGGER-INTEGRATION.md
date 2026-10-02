# HTTP 4.0 Logger Integration

HTTP 4.0 follows the common SLeeLa packet-logging contract.

Incoming: OS socket -> HTTP 4 receiver -> SLeeLa Server packet boundary -> /logger.

Outgoing: HTTP 4 response -> SLeeLa Server -> /logger -> OS socket.

Preserve source, destination, protocol generation, stream, request, sequence, packet size, admission, and endpoint-resolution metadata when available. Raw socket observation is the secondary endpoint-awareness path for malformed, partial, or otherwise undecoded traffic.

Use /resolver when dynamic IP/DNS or trusted endpoint resolution is required before forwarding.