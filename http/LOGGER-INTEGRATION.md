# HTTP Logger Integration

The SLeeLa HTTP implementations use the common `/logger` Packet Logger and the server-edition packet bridge.

## Required paths

Incoming: OS socket -> HTTP receiver -> SLeeLa Server packet boundary -> Logger.

Outgoing: HTTP response -> SLeeLa Server -> Logger -> OS socket.

The bridge can also record raw socket receive/send events independently of protocol parsing. This preserves an ingress record when parsing fails or a packet is only partially decoded.

## Endpoint Awareness

Records carry source, destination, protocol, stream, request and sequence information. HTTP implementations should use `/resolver` when dynamic endpoint resolution is required before forwarding.

The raw-socket observation is the secondary endpoint-awareness path: it records the actual OS socket boundary even when protocol-level metadata is incomplete.

HTTP 1, 2 and 3 should use this contract and the common `SLeeLa-PacketLog-1` format.