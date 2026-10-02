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

## HTTP 1.0-9.0 Standard

All SLeeLa HTTP generations use the same logging boundary contract. Each generation records ingress at the SLeeLa Server packet boundary and egress immediately before the OS socket send. Raw socket observation remains the secondary endpoint-awareness path, while /resolver supplies protocol-aware dynamic endpoint resolution.

Generations covered: HTTP 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, and 9.0.

The canonical record format is SLeeLa-PacketLog-1. The common logger supplies the 240 MiB segment boundary and optional completed-segment copy/archive destination. Each generation retains endpoint, stream/request, sequence, packet-size, admission, and protocol metadata when available. HTTP 8.0 applies the same contract to Subscription/Radio traffic.