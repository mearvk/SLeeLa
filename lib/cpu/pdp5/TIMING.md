# PDP-5 Timing

Exact cycle timing is **unspecified** in this profile unless a reliable model-specific timing table is configured.

Provide two modes:
- **Functional:** deterministic instruction semantics without cycle-accuracy claims.
- **Timed:** consumes explicit per-operation timing metadata supplied by the selected machine profile.

Never silently reuse PDP-8 cycle counts for PDP-5. Report missing timing metadata as unavailable rather than fabricating a value. Host execution speed is not guest instruction timing.