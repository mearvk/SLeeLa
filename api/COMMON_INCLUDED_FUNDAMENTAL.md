# SLeeLa Common, Included — Fundamental Responsibility API

## Status
Common Included API surface for reusable SLeeLa fundamentals.

## Rule
A fundamental class owns one coherent responsibility. Higher-level SLeeLa modules should reuse these classes rather than create parallel one-off abstractions for the same task.

## Supported class sets
### Set 1 — Fundamental
Resource, Identifier, Name, Version, Status, Error, Result, Option, Configuration, Parameter, TypeDescriptor, EnumDescriptor, Schema, Validator, Serializer, Deserializer, Buffer, ByteStream, Input, Output, Clock, Timer, Mutex, Condition, Thread.

### Set 2 — Platform/runtime
Process, Environment, File, Directory, Path, Task, Scheduler, Event, EventBus, Future, CancellationToken, NetworkEndpoint, Transport, TcpTransport, UdpTransport, DnsResolver, SecureChannel, Service, Router, Listener, Server, Client, Session, Connection, Handler.

## Java responsibility correspondence
These are responsibility correspondences, not claims of identical implementation or ABI. Thread ↔ java.lang.Thread; Clock/Timer ↔ Java time and scheduling facilities; Mutex/Condition ↔ Java locking and condition facilities; File/Path ↔ Java filesystem responsibilities; Buffer/ByteStream ↔ Java byte-buffer and stream responsibilities; Process/Environment ↔ Java process/environment responsibilities; Serializer/Deserializer ↔ Java data encoding.

## Source location
Native fundamental implementations are under impl/fundamental/, with one .hpp contract and one .cpp implementation per responsibility class.

## Inclusion contract
The Common Included layer is foundational. HTTP, server, GUI, database, driver, antivirus, compiler, and application modules may depend on it, but domain-specific policy remains outside this layer.

## Census
The repository now contains 50 Common, Included responsibility classes represented by 100 native source files in impl/fundamental/. Set 1 and Set 2 each contain 25 classes.
