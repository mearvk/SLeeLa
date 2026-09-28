# Sleelavirin™ Verdict Model

A normalized verdict should contain:

- request ID;
- package/artifact identity;
- scanner/provider;
- scanner version;
- rule/signature identifier when available;
- verdict class;
- confidence/evidence level when provided by the engine;
- timestamp;
- artifact digest;
- provenance;
- error state, if any.

Provider-specific fields should be retained as extensions rather than discarded.
