# SLVM/4 Security

SLVM/4 extends /3 into distributed execution.

1. Remote execution is never implicit.
2. Peer identity and encrypted transport are required by secure profiles.
3. Requests use freshness/nonces and correlation IDs.
4. Remote objects have explicit leases and lifecycle.
5. Capability delegation is scoped and expires.
6. Resolver results cannot grant authority.
7. JVM/JavaFX interoperability occurs through the broker boundary.
8. Observer records redact secrets and correlate distributed activity.
9. Certificate and attestation decisions remain policy-controlled.
10. Isolation and resource quotas apply to remote as well as local work.
