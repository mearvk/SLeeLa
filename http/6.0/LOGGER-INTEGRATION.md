# HTTP 6.0 Logger Integration

The HTTP 6.0 reference server now links the shared `../logger/sleela_logger.cpp` implementation through `http-6.0/http6_logger_bridge.{h,cpp}`.

Ingress records are written after a request body is received and validated, and HTTP parse failures are recorded as rejected. Egress response metadata is recorded immediately before socket writes. Records use the shared SLeeLa-PacketLog-1 JSON Lines format and 240 MiB segment boundary. The bridge never supplies payload bytes; logger payload capture remains disabled.

The current raw-socket bridge does not yet know the remote peer address, stream/request/sequence identifiers for ordinary HTTP requests, or all parse/framing failure outcomes. Its peer field is therefore `unknown-peer` where needed. Dedicated end-to-end tests must still verify persisted ingress/egress/rejection records, logger disk failures, and segment rotation.

Incoming: OS socket -> HTTP 6 receiver -> shared logger metadata record.
Outgoing: HTTP 6 response metadata -> shared logger -> OS socket.

The logger records outcomes; it does not make routing, admission, or authorization decisions. Preferred-router and resolver hooks are not security credentials.
