# HTTP 2 Logger Integration

Incoming: OS socket -> HTTP/2 receiver -> SLeeLa Server -> Logger.

Outgoing: HTTP/2 response -> SLeeLa Server -> Logger -> OS socket.

Use `/logger` and `server-edition/packet_logger_bridge.hpp`; preserve endpoint, stream, request and sequence metadata.