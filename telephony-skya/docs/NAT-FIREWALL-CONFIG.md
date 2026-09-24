# Skya NAT / Firewall Initial Configuration

Skya makes a bounded, visible initial policy decision before creating its engine. It does not silently claim that a private address is publicly reachable.

Basic: HTTP/3 preferred, TLS required, outbound/relay-capable NAT policy, port 8443, and firewall auto. This avoids requiring an inbound rule when outbound connectivity is sufficient.

Intermediate: reads the existing SLeeLa NAT environment and preserves explicit direct, port-forward, ipv6, outbound, or relay choices. Relay is enabled only for outbound/relay modes.

Advanced: accepts explicit HTTP and firewall policy. SLEELA_SKYA_FIREWALL=always fails closed when the platform has no supported firewall controller.

Environment controls:
SLEELA_SKYA_CONFIG_LEVEL=basic|intermediate|advanced
SLEELA_SKYA_FIREWALL=never|auto|always
SLEELA_SKYA_HTTP=2|3
SLEELA_NAT_MODE=direct|port-forward|ipv6|outbound|relay
SLEELA_NAT_BIND_HOST, SLEELA_NAT_BIND_PORT
SLEELA_NAT_PUBLIC_HOST, SLEELA_NAT_PUBLIC_PORT
SLEELA_NAT_RENDEZVOUS, SLEELA_NAT_KEEPALIVE, SLEELA_NAT_TLS_REQUIRED

SLEELA_NAT_MODE is interpreted by the existing api/server/nat_aware.* implementation. Firewall port lifecycle remains owned by SLeeLa's platform-specific port-awareness controller; Skya does not create a second firewall engine.

Use ./telephony-skya/native/skya plan to inspect the initial decision without changing firewall state.
