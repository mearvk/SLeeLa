# SLeeLa HTTP — Routing Security 244-Artifact Model

**Status:** Proposed metadata model

The 244 artifacts are metadata slots, not 244 mandatory bytes and not 244 claims of fact. Each artifact should have an identifier, value, source, timestamp, confidence, provenance, optional signature, and retention classification.

## Artifact Groups

| Range | Group | Count |
|---|---|---:|
| 001–016 | Protocol and method | 16 |
| 017–032 | Source/destination identity | 16 |
| 033–048 | Naming and service identity | 16 |
| 049–064 | Route and AS evidence | 16 |
| 065–080 | Path security | 16 |
| 081–096 | Cryptographic evidence | 16 |
| 097–112 | Transport conditions | 16 |
| 113–128 | Physical-medium evidence | 16 |
| 129–144 | Geographic evidence | 16 |
| 145–160 | Timing and movement | 16 |
| 161–176 | Reliability and resilience | 16 |
| 177–192 | Administrative/operator evidence | 16 |
| 193–208 | Jurisdictional/legal context | 16 |
| 209–224 | Audit and chain of custody | 16 |
| 225–240 | Incident/security state | 16 |
| 241–244 | Extension/control | 4 |

**Total: 244 artifacts.**

## 001–016 — Protocol and Method

protocol-version; method; request-class; response-class; content-type; content-length; encoding; compression; stream-mode; multiplex-mode; request-id; correlation-id; idempotency-class; retry-policy; capability-set; protocol-extension.

## 017–032 — Source/Destination Identity

source-ip; destination-ip; source-port; destination-port; source-asn; destination-asn; source-prefix; destination-prefix; source-identity; destination-identity; client-identity; service-identity; endpoint-identity; identity-method; identity-confidence; identity-provenance.

## 033–048 — Naming and Service Identity

hostname; service-name; operation-name; service-id; operation-id; dns-resolution-id; dnssec-state; certificate-name; certificate-id; namespace; naming-version; naming-source; name-resolution-time; name-cache-state; name-integrity; name-provenance.

## 049–064 — Route and AS Evidence

observed-as-path; origin-as; route-prefix; route-version; route-source; route-age; route-preference; next-hop; route-collector; route-observation-time; rpki-state; roa-state; bgpsec-state; as-path-validation; route-confidence; route-provenance.

## 065–080 — Path Security

tls-state; tls-version; cipher-suite; peer-authentication; endpoint-authentication; channel-integrity; forward-secrecy; certificate-chain-state; certificate-validity; certificate-revocation-state; key-exchange-state; session-resumption; anti-replay-state; spoofing-assessment; route-hijack-assessment; path-security-confidence.

## 081–096 — Cryptographic Evidence

algorithm-family; hash-algorithm; signature-algorithm; key-id; key-version; signature; signature-time; signature-status; verification-method; verification-result; trust-anchor; trust-store-version; certificate-fingerprint; payload-digest; envelope-digest; cryptographic-provenance.

## 097–112 — Transport Conditions

transport; transport-version; tcp-state; udp-state; quic-state; mtu; path-mtu; congestion-state; packet-loss-estimate; retransmission-count; latency; jitter; bandwidth-estimate; connection-age; keepalive-state; transport-provenance.

## 113–128 — Physical-Medium Evidence

medium-class; fibre-indicator; coax-indicator; copper-indicator; radio-indicator; ethernet-indicator; wifi-indicator; optical-transport-indicator; submarine-cable-indicator; terrestrial-cable-indicator; satellite-indicator; medium-observation-source; medium-confidence; link-redundancy; physical-fault-state; medium-provenance.

These fields are observational. An IP address alone must not be treated as proof of a physical medium.

## 129–144 — Geographic Evidence

source-country-observation; destination-country-observation; ingress-region; egress-region; ingress-site; egress-site; route-collector-location; exchange-point-location; data-center-location; cable-landing-location; optical-node-location; geographic-method; geographic-confidence; geolocation-time; geolocation-source; geographic-provenance.

Geolocation should distinguish measured, registered, inferred, and operator-declared location.

## 145–160 — Timing and Movement

send-time; receive-time; ingress-time; egress-time; processing-time; queue-time; route-change-time; dns-time; tls-time; application-time; total-latency; clock-source; clock-confidence; sequence-number; hop-observation-count; timing-provenance.

## 161–176 — Reliability and Resilience

availability-state; health-state; readiness-state; failover-state; redundancy-state; alternate-route-state; congestion-risk; outage-indicator; route-flap-indicator; packet-loss-risk; service-degradation; recovery-state; recovery-time; resilience-class; resilience-confidence; resilience-provenance.

## 177–192 — Administrative/Operator Evidence

originating-operator; transit-operator; terminating-operator; operator-id; network-policy-id; peering-policy-id; security-policy-id; incident-id; maintenance-window; change-id; configuration-version; equipment-class; equipment-vendor; equipment-model; operator-attestation; administrative-provenance.

## 193–208 — Jurisdictional/Legal Context

source-jurisdiction-observation; destination-jurisdiction-observation; transit-jurisdiction-observation; applicable-policy-reference; data-residency-observation; export-control-observation; sanctions-screening-state; lawful-process-reference; preservation-request-reference; court-order-reference; treaty-reference; jurisdictional-basis; legal-review-state; legal-review-authority; legal-confidence; legal-provenance.

These are context fields, not legal verdicts. The ICC has jurisdiction over persons and specified crimes under the Rome Statute; an HTTP packet cannot itself establish that a crime occurred.

## 209–224 — Audit and Chain of Custody

event-id; parent-event-id; evidence-id; evidence-type; evidence-hash; evidence-source; collector-id; collector-version; collection-time; storage-time; retention-policy; access-policy; access-event; export-event; chain-integrity; audit-provenance.

## 225–240 — Incident/Security State

threat-class; anomaly-state; intrusion-indicator; route-anomaly; dns-anomaly; tls-anomaly; certificate-anomaly; identity-anomaly; spoofing-indicator; hijack-indicator; replay-indicator; denial-of-service-indicator; exfiltration-indicator; incident-severity; incident-confidence; incident-provenance.

## 241–244 — Extension/Control

schema-version; extension-bitmap; reserved-control; artifact-integrity.

## Packing Rule

The 244-artifact model must not mean every HTTP request carries all 244 fields. A normal request carries only the fields necessary for its context. The packet model should support a minimal request, a compact artifact bitmap, only-present artifacts, and optional signed evidence.

This follows the SLeeLa principle established for RMI and HTTP: **faster name, faster pack, fewer bytes, fewer copies, faster dispatch.**

## Evidence Rule

Every artifact should distinguish observed fact, operator assertion, derived inference, external registry information, and legal/policy context. A derived geolocation must not be represented as an observed physical router location.

## Security Rule

The 244-artifact model is an observability framework. It does not authorize interception, surveillance, traffic manipulation, or access to private network infrastructure.

## Future Work

The next protocol revision should define a compact binary encoding, artifact bitmap, delta encoding for repeated route observations, signed evidence bundles, privacy-preserving geographic representations, RPKI/BGPsec observation adapters, standardized IETF/ITU terminology mappings, and interoperability tests against ordinary HTTP infrastructure.
