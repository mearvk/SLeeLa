# SLeeLa Registry 3.0 — Unified Registry Architecture

*Status: architecture baseline and compatibility contract. The existing native C/C++ store and SLeeLa object model remain the compatibility implementation until the 3.0 record engine and migration tests are shipped.*

## 1. Purpose

Registry is SLeeLa's authoritative catalog of identity, classification, location references, lifecycle, provenance, integrity state, and declared relationships.

Registry is not merely a settings file, filesystem index, database schema, or permission system. It is the common naming and discovery layer used by the language, compiler, decompiler, runtime, virtual machines, packages, APIs, operating-system adapters, services, and Modium.

The governing rule:

> Registry identifies and describes. A resolver locates. A verifier checks. A policy authority authorizes. A runtime executes.

No registration, reference, hash, seal, status flag, or successful lookup grants execution authority by itself.

## 2. Relationship to Modium

Modium supplies two complementary capabilities:

1. Artifact integrity and resolution — versioned VM files are compared with their manifest and recovery references.
2. Persistent storage regions — file-backed or explicitly claimed removable-media regions, with bounded sizes and consent-gated writes.

Registry catalogs Modium regions and their relationships to VM versions and artifacts. It does not replace Modium's integrity scanner or storage engine.

A Modium region record describes:
- stable registry record ID and namespace;
- region kind (file-backed or removable-volume);
- descriptor format version and region byte capacity;
- region/container identity and device seal where applicable;
- lifecycle (discovered, inspected, registered, available, offline, quarantined, retired);
- integrity result and time of last verification;
- owner label and provenance;
- references to VM versions, manifests, or artifacts assigned to it.

Never make a mount path, drive letter, raw device path, file descriptor, or VM-local handle the permanent identity. These are runtime locators or capabilities and may change. Registry stores a stable record identity and non-secret descriptive locator metadata; Modium resolves the current host location and returns only bounded VM-local handles.

Discovery and inspection remain read-only. Claiming, initializing, resizing, writing, restoring, or retiring storage requires the relevant explicit operation and authorization. Registry must never turn a discovered volume into an automatically claimed volume.

## 3. Canonical record model

The 3.0 logical unit is a record, not a value attached to a path-only key. A record has these conceptual fields:

| Field | Meaning | Mutability |
|---|---|---|
| record_id | Stable, globally unique identifier within the registry identity scheme | Immutable |
| namespace | Owning domain, e.g. sleela.runtime, sleela.modium | Immutable after creation |
| kind | Typed record class: artifact, package, VM, region, service, API, OS adapter, setting, feature, policy reference, etc. | Immutable after creation |
| schema_version | Version of this record's schema | Changes only through migration |
| name | Human-readable display name | Mutable with audit |
| aliases | Additional lookup names | Mutable with collision checks |
| state | Explicit lifecycle state | Controlled transition |
| locator | Resolver-specific location reference; not identity or authority | Mutable |
| version | Domain version, distinct from schema version | Controlled |
| content_digest | Digest of exact content, when content-addressable | Recomputed |
| provenance | Author/source/time/origin and supporting evidence | Append-only history |
| relationships | Typed links to other record IDs | Audited |
| capabilities | Declared capabilities, not grants | Audited |
| policy_ref | Reference to policy used to decide whether an operation is allowed | Controlled |
| created_at, updated_at | UTC timestamps | System-maintained |
| revision | Monotonic record revision for compare-and-swap updates | System-maintained |

A record ID is not a path, display name, version string, hash, or permission token. Aliases may change without changing identity. Content digests identify bytes, not authors or trust.

## 4. Namespaces and classification

Use stable, lowercase, dot-separated namespaces and explicit kind values. Initial domains:

- sleela.system
- sleela.language
- sleela.compiler
- sleela.decompiler
- sleela.runtime
- sleela.vm
- sleela.vm.premium
- sleela.package
- sleela.api
- sleela.os
- sleela.service
- sleela.modium
- sleela.registry
- sleela.policy

A namespace classifies ownership and domain; it does not imply privilege. A record may be related to records in other namespaces through typed relationships.

Avoid creating a new namespace for every folder or spelling variant. Directory structure is a source locator, not the semantic taxonomy.

## 5. Identity, integrity, provenance, and trust are separate

These concepts must never be collapsed:

- Identity: which logical record is being referenced.
- Classification: what kind of thing the record describes.
- Location: where a resolver can currently find it.
- Version: which domain release is represented.
- Schema version: how the record itself is encoded.
- Integrity: whether observed bytes match the recorded digest.
- Provenance: who or what supplied the record and its history.
- Authenticity: whether provenance is cryptographically attributable to a trusted signer.
- Authorization: whether the requested actor may perform a particular operation now.

The existing SHA-256 originality seal (hex_id | version | hash | author) is a deterministic tamper-evident consistency check. It is not a digital signature, does not establish the real-world identity of an author, and does not grant trust or permission. A future signature field must identify the signature algorithm, key ID, signature bytes, and verification policy separately.

Preserve the existing seven provenance fields during migration. Add richer provenance as a versioned record rather than silently changing the meaning of existing fields.

## 6. Relationships and resolution

Relationships are typed edges with stable target record IDs. Initial relationship types:

- contains — package or container owns a component;
- depends_on — build/runtime dependency;
- implements — implementation of a language/API/specification;
- compatible_with — declared compatibility, subject to verification;
- stores — Modium region stores an artifact/version;
- resolves_to — logical record maps to a current locator;
- supersedes — a later record replaces an earlier one;
- derived_from — provenance lineage;
- uses_policy — policy reference used for an operation.

A resolver returns an explicit result (resolved, not_found, ambiguous, offline, incompatible, integrity_failed, denied, unsupported). It must not silently select an arbitrary candidate. Compatibility and resolution do not authorize execution.

## 7. Lifecycle and controlled transitions

Recommended general lifecycle:

draft → discovered → registered → verified → available → deprecated → retired

Exceptional states include offline, quarantined, rejected, and revoked. Not every kind uses every state. Each kind defines its allowed transition graph and required evidence.

- Discovery creates an observation, not an approved installation.
- Registration creates a catalog record, not a trusted artifact.
- Verification records the result of a defined check, not an authorization.
- Quarantine prevents normal resolution until an explicit review clears it.
- Retirement preserves identity and history; it does not erase provenance.

Invalid transitions fail closed and produce a reviewable reason.

## 8. Registry service boundaries

The public service is divided into narrow operations:

1. identify — resolve stable IDs and aliases.
2. classify — validate namespace, kind, and schema.
3. register — create a record after schema validation.
4. describe — read record metadata and relationships.
5. resolve — ask a domain resolver for a current locator.
6. verify — request integrity/provenance checks from the responsible verifier.
7. relate — add or retire a typed relationship.
8. transition — perform a valid lifecycle change.
9. review — produce a deterministic human-readable report.
10. export/import — move versioned records with validation and explicit migration.

The Registry core must not execute programs, load native libraries, mount devices, claim storage, change OS settings, or bypass a domain service. It may return a reference to the responsible service.

## 9. Storage, transactions, and compatibility

The current .sleelareg format (SLREG 1) remains readable and writable for compatibility. Do not reinterpret existing files in place.

The 3.0 format should be introduced as a versioned record envelope (SLREG 2) with:
- explicit schema version and registry identity;
- deterministic canonical field ordering and escaping;
- UTF-8 validation and defined normalization policy;
- strict bounds and rejection of duplicate IDs, duplicate aliases, malformed digests, and unknown mandatory fields;
- atomic save (temporary file, flush, replace) and no partial success;
- revision-aware updates to prevent lost writes;
- append-only audit events for mutations;
- deterministic export and reproducible import;
- explicit v1-to-v2 migration report and rollback copy.

The native store must stop treating a fixed 16-values-per-key limit as a semantic model limit. Capacity must be configurable, bounded, reported, and fail explicitly on exhaustion. The SLeeLa mirror must not impose its current eight-key/eight-value teaching-model limits on the authoritative native registry.

SQL is an export/analysis projection, not the sole source of truth. Exported rows must preserve record IDs, schema versions, relationship IDs, revision numbers, and provenance history.

## 10. SLeeLa 3.0 source model

Keep the native C ABI as the stable low-level boundary and the C++ RAII facade as orchestration. The SLeeLa package under lib/registry/ should model the same concepts with explicit record and relationship classes.

Recommended source-level family:
- SLRegistry — registry service facade and review;
- SLRegistryRecord — canonical record envelope;
- SLRegistryIdentity — stable ID and namespace;
- SLRegistryRelationship — typed edge;
- SLRegistryLifecycle — state and transition validation;
- SLRegistryValue — typed setting/attribute value;
- SLRegistryProvenance — source history and seal/signature metadata;
- SLRegistryResolver — resolution request/result;
- SLRegistryReview — deterministic diagnostics.

The existing SLRegistryKey remains a compatibility view over hierarchical paths. It must not be the only identity model. Existing callers of SLRegistry, key/value accessors, feature flags, provenance, save/load, and review continue to work through a compatibility adapter.

All SLeeLa source declarations in this package target #sleela 3.0. API documentation is regenerated from source; generated HTML must not be hand-edited as the canonical definition.

## 11. CLI contract

Preserve existing commands and add explicit record operations:

    sleela registry
    sleela registry review [namespace-or-id]
    sleela registry get <key> <name>
    sleela registry set <key> <name> <value> [--int|--bool]
    sleela registry feature <name> [on|off]

    sleela registry identify <record-id-or-alias>
    sleela registry describe <record-id>
    sleela registry resolve <record-id>
    sleela registry verify <record-id>
    sleela registry relations <record-id>
    sleela registry migrate --from SLREG1 --to SLREG2 --dry-run

Mutating operations must say what will change and return a non-zero status on validation, persistence, conflict, or authorization failure. Resolve and verify are read/verification operations; neither executes the target.

## 12. Modium registration contract

Modium remains the authority for storage mechanics. Registry keeps the catalog entry and relationships:

- sleela.modium.region records describe region identity, capacity, descriptor format, lifecycle, device seal (when applicable), and last verification.
- sleela.modium.volume-observation records are temporary observations, not permanent device claims.
- stores relationships connect a region to VM versions or artifacts.
- A region's available state requires a fresh Modium resolution and integrity check; a cached registry state alone is insufficient.
- An offline or foreign-sealed device becomes offline or quarantined; never silently rebind it to another volume.
- Registry cannot call claim, write, resize, or restore as a side effect of lookup, review, registration, or resolution.

## 13. Verification and acceptance criteria

Before calling Registry 3.0 implemented, tests must prove:

- stable IDs survive rename, relocation, save/load, and export/import;
- aliases are unique within their namespace and collisions are rejected;
- schema and kind validation reject malformed or unsupported records;
- relationship targets exist or are explicitly marked external/unresolved;
- lifecycle transitions reject illegal state changes;
- digest mismatch, broken seal, unknown signer, and denied operation remain distinct outcomes;
- v1 migration preserves every existing key, typed value, provenance field, feature flag, and ordering semantics;
- repeated canonical saves are byte-for-byte deterministic;
- failed writes leave the previous registry intact;
- concurrent revision conflicts are detected;
- Modium lookup is read-only and never claims, formats, or writes a volume;
- unavailable/foreign Modium media cannot be reported as available;
- native C ABI, C++ facade, SLeeLa source model, CLI, and generated API agree on the same contract.

Required matrix: Linux, Windows 10+, and macOS; native C and C++; SLeeLa compiler/runtime integration; clean install; existing v1 registry fixtures; malformed-input and capacity-boundary tests.

## 14. Delivery stages

1. Architecture and compatibility contract — this document; no runtime behavior claimed.
2. Schema and migration — SLREG 2 parser/writer, deterministic migration, validation tests.
3. Record core — identity, namespaces, relationships, lifecycle, revisions, audit events.
4. Bindings — C ABI, C++ facade, SLeeLa 3.0 model, CLI.
5. Modium integration — region registration and read-only resolution/verification bridge.
6. Cross-platform verification — build and test matrix, generated API, install review, documentation synchronization.

A stage is complete only when its code and tests exist and the relevant checks have actually run.

---
Core invariant: Registry is the shared map of what SLeeLa knows exists; Modium verifies and stores; resolvers locate; policy authorizes; runtimes execute. No one subsystem silently assumes another subsystem's authority.
