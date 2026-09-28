# HTTP/4 Internet Update Loop

UPDATE-8-PER-MINUTE.sh performs eight outbound GET attempts per minute: one
request every 7.5 seconds. The endpoint is configurable with HTTP4_UPDATE_URL.

The updater prefers curl and falls back to wget. It performs a bounded GET, does
not upload packet contents, and records only timestamp/status/endpoint locally.

The updater is intentionally separate from frame transmission. HTTP/4 packets
must not block on arbitrary Internet fetches; production deployments should
run the updater as a sidecar/service and expose only a local freshness state.

## Basket

HTTP/3 already carries the frozen 14-item basket in every application envelope,
and its keyed digest covers that basket. HTTP/4 reuses the same canonical block
inside its packet context.

## Standards boundary

HTTP/4 remains an experimental SLeeLa protocol generation. The IETF currently
defines HTTP/1.1, HTTP/2, and HTTP/3 as the core protocol mappings, while RFC
9110 defines common HTTP semantics and extension mechanisms.
