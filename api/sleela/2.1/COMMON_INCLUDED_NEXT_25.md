# SLeeLa Common, Included — Next 25 Supported Responsibility Classes

These 25 classes extend the first 25 fundamental responsibilities into operating-system, concurrency, networking, transport, and service execution concerns.

## Execution and operating-system services
- Process — native process execution and exit status.
- Environment — process environment access and mutation.
- File — file existence, byte I/O, and metadata.
- Directory — directory creation and enumeration.
- Path — filesystem path normalization and decomposition.

## Concurrency and asynchronous execution
- Task — asynchronous callable execution.
- Scheduler — delayed task scheduling and due-task dispatch.
- Event — named event with payload and fields.
- EventBus — event subscription and publication.
- Future — typed asynchronous result transport.
- CancellationToken — shared cooperative cancellation state.

## Networking and transport
- NetworkEndpoint — host/port endpoint identity.
- Transport — native transport polymorphic boundary.
- TcpTransport — TCP socket transport.
- UdpTransport — UDP socket transport.
- DnsResolver — native DNS/address resolution.
- SecureChannel — transport security/provider boundary.

## Services and application networking
- Service — start/stop service lifecycle.
- Router — method/path handler dispatch.
- Listener — endpoint listening state.
- Server — service + listener + router composition.
- Client — transport-backed client lifecycle.
- Session — logical session lifecycle.
- Connection — endpoint/transport connection lifecycle.
- Handler — callable request/operation handler.

## Native underpinnings
C++ supplies the cross-platform object contracts and implementations. POSIX/Unix socket and filesystem facilities are used on Unix-like systems, while Winsock/environment APIs are selected on Windows where required. TcpTransport and UdpTransport therefore have concrete socket underpinnings.

Future, CancellationToken, Transport, and SecureChannel retain explicit provider boundaries where implementation is type-dependent or where a platform TLS/cryptographic provider must be selected.

## Source layout
Every class has impl/fundamental/<Class>.hpp as its C++ contract and impl/fundamental/<Class>.cpp as its native implementation or explicit native boundary.

Together with Set 1, the Common, Included layer now contains 50 responsibility classes and 100 native source files.
