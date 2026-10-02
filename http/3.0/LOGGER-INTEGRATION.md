# HTTP 3 Logger Integration

Incoming: OS socket -> HTTP/3 receiver -> SLeeLa Server -> Logger.

Outgoing: HTTP/3 response -> SLeeLa Server -> Logger -> OS socket.

Use `/logger` and `server-edition/packet_logger_bridge.hpp`; preserve endpoint, stream, request and sequence metadata.