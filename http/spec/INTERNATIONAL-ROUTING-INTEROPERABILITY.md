# SLeeLa HTTP — International Routing Interoperability

**Status:** Research and design baseline

## Scope

SLeeLa HTTP operates above the Internet routing system. It should be routing-aware without becoming a routing protocol. The Internet is a network of independently administered Autonomous Systems (ASes); BGP is the principal inter-domain routing protocol, while individual networks may use different interior routing systems. The Internet Society emphasizes decentralized management and the absence of a single global routing controller. [1]

SLeeLa HTTP should interoperate with ordinary IP routing, preserve end-to-end application identity where possible, expose useful route/security observations to authorized monitors, and never assume an application can dictate the path selected by BGP.

## Standards Baseline

The research baseline should include BGP-4, BGP operational/security practices, RPKI, BGPsec where deployed, IPv4, IPv6, DNS/DNSSEC, TLS, HTTP/1.1, HTTP/2, HTTP/3, MPLS, OSPF, IS-IS, Ethernet, Wi-Fi, and optical transport/access standards.

The Internet Society notes that BGP itself does not directly provide routing security and that RPKI, DNSSEC, PKI, and related mechanisms provide additional validation and identity assurance. [1]

## Optical and Physical Media

SLeeLa HTTP must not require a particular physical medium. ITU-T G.872 describes optical transport network architecture including multiplexing, routing, supervision, performance assessment, survivability, and optical media structures. [2] ITU-T L.250 describes optical access architectures including point-to-point, point-to-multipoint, ring, redundancy, installation, monitoring, and optical safety. [3] The current ITU-T G.652 edition in force is the 2024 edition for single-mode optical fibre and cable characteristics. [4]

The protocol should therefore treat fibre, optical transport, coaxial access, copper Ethernet, Wi-Fi, microwave, satellite, and other media as lower-layer transport conditions rather than HTTP variants.

## Fiber-Router Percentage

There is no authoritative global percentage of Internet routers that can accurately be classified as “fiber-optic routers.” A router may have fibre and copper interfaces simultaneously, use optical transceivers on Ethernet interfaces, or sit in a fibre-fed facility without every interface being fibre.

Public global routing data generally describes ASes, prefixes, paths, and routing relationships rather than a complete inventory of physical router interfaces. SLeeLa documentation must not invent a global fibre-router percentage.

A better measurement is capacity and path-medium evidence. TeleGeography reported 1,835 Tbps of international Internet bandwidth at mid-year 2025, with 23% growth during 2025. That is a bandwidth statistic, not a router-count or fibre-router percentage. [5]

## Router Geolocation

There is no single authoritative public database containing the geographic coordinates of every Internet router. Useful public geographic evidence can instead cover AS registration, Internet exchange points, DNS root-server instances, submarine cable landing points, data centers, published network PoPs, traceroute observations, and route collectors.

ICANN states that the root server system has 12 independent operators managing 13 named root identities, represented by more than 1,500 individual servers worldwide. [6] IANA publishes the 13 root-server identities and operators; these are DNS root authorities, not a map of all Internet routers. [7]

SLeeLa should represent geolocation as evidence with provenance, distinguishing measured, registered, inferred, and operator-declared location.

## International-Law Awareness

SLeeLa HTTP may record legal or jurisdictional metadata, but it must not claim that a packet itself determines legal responsibility.

The International Criminal Court is a treaty-based criminal court whose Rome Statute gives it jurisdiction over genocide, crimes against humanity, war crimes, and aggression under the conditions specified by the Statute. [8] The ICC's jurisdiction is over persons and defined crimes; an HTTP packet is not itself a legal finding. [9]

Accordingly, a routing/security record may contain jurisdictional observation, source/destination evidence, time, cryptographic integrity evidence, operator assertions, policy identifiers, and chain-of-custody metadata, but it must not encode an unsupported declaration that a packet is “lawful” or that a route constitutes an ICC crime.

## Engineering-Law Separation

SLeeLa should keep three layers distinct:

1. **Engineering fact:** a route was observed through AS X.
2. **Policy/security fact:** the route passed or failed configured RPKI validation.
3. **Legal assessment:** a competent legal authority may assess whether conduct violates applicable law.

These must never be silently collapsed into one field.

## International Routing Security

SLeeLa should learn from existing routing-security practice rather than replace it. RPKI can associate IP address resources with authorized AS numbers and improve route-origin validation. [10] BGPsec provides cryptographic protection for the inter-AS path where deployed. [11] BGP operational security guidance includes TTL protection, TCP-AO, control-plane filtering, prefix filtering, max-prefix filtering, AS-path filtering, route-flap controls, and community handling. [12]

SLeeLa HTTP should record which lower-layer security assertions were observed rather than inventing a competing routing-security protocol.

## Research Rule

Every future SLeeLa HTTP routing feature should answer:

1. What Internet standard already does this?
2. What engineering standard governs the physical or transport layer?
3. What evidence can the endpoint actually observe?
4. What legal conclusion, if any, must remain outside the protocol?

This keeps SLeeLa HTTP interoperable, technically grounded, and internationally conscious without overstating what a packet can establish.

## References

[1] Internet Society, Securing BGP and Critical Properties of the Internet.  
[2] ITU-T G.872, Architecture of the optical transport network.  
[3] ITU-T L.250, Topologies for optical access network.  
[4] ITU-T G.652, Characteristics of a single-mode optical fibre and cable.  
[5] TeleGeography, International Internet Bandwidth, 2025.  
[6] ICANN, The Root Server System.  
[7] IANA, Root Name Servers.  
[8] ICC, Rome Statute, Article 5.  
[9] ICC, Understanding the International Criminal Court.  
[10] Internet Society, RPKI and routing security.  
[11] Internet Society, BGPsec.  
[12] Internet Society, RFC 7454 BGP Operations and Security.
