# SLeeLa HTTP 1.0

HTTP 1.0 is the baseline SLeeLa application generation. Its port model preserves compatibility with conventional HTTP/1.0 while introducing the same logical routing concept used by later HTTP generations.

## Port and Multiplexing Model

HTTP 1.0 does not have HTTP/2-style stream multiplexing. SLeeLa therefore performs logical multiplexing above the HTTP/1.0 request/connection boundary.

```text
native TCP connection
        |
        +-- HTTP/1.0 request -- logical PORT -- SERVICE/OP -- payload
        +-- HTTP/1.0 request -- logical PORT -- SERVICE/OP -- payload
        +-- another connection/request as required
```

The logical port is an application identifier. It does **not** mean that every logical port receives its own TCP socket.

## Addressing Layers

```text
native transport endpoint
        ↓
HTTP/1.0 request/connection
        ↓
SLeeLa logical PORT
        ↓
SERVICE-ID / OP-ID
        ↓
application operation
```

HTTP 1.0 implementations should keep the logical-port layer independent from native TCP/UDP port numbering. This allows the same service-routing model to continue into HTTP 2.0+.

## Compatibility

The HTTP 1.0 layer remains compatible with ordinary HTTP infrastructure. Proxies, TLS termination, routers, and conventional TCP port bindings remain transport concerns; SLeeLa logical ports remain application-routing concerns.
