# SLeeLa HTTP 3.0 Extended Ports

## Logical port namespace

HTTP 3.0 now carries an explicit **160-bit logical port number** in every application envelope.

The supported namespace is:

- Minimum: `0`
- Maximum: `10^48 - 1`
- Total distinct values: `10^48`
- Wire width: **20 bytes**
- Wire order: **big-endian / network order**

The decimal value is preserved exactly. It is not truncated to the traditional 16-bit TCP/UDP port type.

## Packet representation

Textual HTTP 3.0 envelopes now contain:

`H3 VERSION FLAGS SERVICE-ID OP-ID REQUEST-ID PORT NONCE DIGEST INTACTX BASKET PAYLOAD-LENGTH:PAYLOAD`

Binary envelopes contain a 20-byte PORT field immediately after REQUEST-ID:

`[REQUEST-ID:8][PORT:20][NONCE:8][DIGEST:8][INTACTX:8]`

The PORT is included in the canonical keyed-MAC input. Changing the port therefore invalidates the packet DIGEST.

## Process model

The extended port value is a **logical protocol port namespace**. A process may identify up to (10^{48}) logical ports without allocating (10^{48}) operating-system sockets or file descriptors.

The native operating system can still impose a smaller transport binding limit. A logical port can therefore identify a service, channel, endpoint, virtual listener, or application route above the native TCP/UDP port space.

## Native API

The implementation is in:

- `http3_port.h`
- `http3_port.c`
- `http3_envelope.h`
- `http3_envelope.c`

Important functions include:

- `http3_port_from_decimal()`
- `http3_port_to_decimal()`
- `http3_port_from_u64()`
- `http3_port_is_valid()`
- `http3_port_increment()`
- `http3_envelope_set_port_decimal()`
- `http3_envelope_set_port_u64()`

The boundary self-test is available through:

`make -C http-3.0 port-test`



## Multiplexing Relationship

The 160-bit logical PORT is an application namespace layered over HTTP/3 stream multiplexing.

```text
QUIC connection
  ├── stream A → PORT A → service/channel A
  ├── stream B → PORT B → service/channel B
  └── stream C → PORT C → service/channel C
```

The PORT value and HTTP/3 stream identifier are intentionally different. Stream identifiers control transport multiplexing; PORT values identify SLeeLa application routes.

The same conceptual model is used by HTTP 2.1. HTTP 3.0 adds the larger authenticated PORT field to the envelope and includes it in the canonical keyed-MAC input.


## Stream Multiplexing Boundary

The 160-bit logical PORT is an application namespace layered over HTTP/3 stream multiplexing. A single QUIC connection may carry many streams, and each stream may carry a different SLeeLa PORT.

The PORT value and HTTP/3 stream identifier are different namespaces: the stream controls transport multiplexing; PORT identifies the SLeeLa application route. HTTP 3.0 therefore does not require one native socket for every logical PORT.


## Download Routing

DOWNLOAD is an application operation routed through the SLeeLa logical PORT namespace. It does not require a dedicated native TCP/UDP port. Large-file transfers over 50 MB use the same logical-port namespace and may be carried on HTTP/3 streams.
