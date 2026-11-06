# Registry 3.0 — Binding Contract

Status: the native C record API and C++ RAII facade are introduced in the 3.0 source set. The source models are present. The CLI currently offers SLREG 1 migration, SLREG 2 review, and structural verification. Compiler, Decompiler, runtime, standard VM, Premium VMs, and OSLS dispatch remain integration targets, not completed end-to-end bindings.

## Shared model

Every layer keeps these concepts separate: stable identity, kind/schema classification, current locator, integrity result, provenance, and operation permission. A record ID, digest, successful lookup, or resolved locator never grants permission to load or execute code.

## Native C

Public headers:
- registry/sleela_registry.h — legacy SLREG 1 compatibility.
- registry/v3/sleela_registry_v3.h — Registry 3.0 record API.

The 3.0 API provides create/destroy, SHA-256-derived IDs, record initialization and insertion, lookup, alias and relationship insertion, revision-checked rename/locator/integrity updates, lifecycle transitions, Modium region catalog registration, SLREG 2 save/load, SLREG 1 migration, and human-readable review.

The configured total capacity defaults to 4096 and is capped at 1,000,000 records. Alias and relationship counts are explicitly bounded. Capacity exhaustion and malformed input return status codes; fixed-length fields reject overlong text rather than silently truncating it.

## C++

registry/v3/sleela_registry_v3.hpp provides a move-only C++17 RAII facade over the C ABI. It does not define a separate storage format or trust model.

## CLI

The separate native binary is registry/build/sleela-registry3:
- migrate-v1 INPUT OUTPUT REGISTRY_ID — writes a new SLREG 2 file and does not replace the source file.
- review FILE — prints a deterministic review.
- verify FILE — checks the SLREG 2 envelope, record schema, IDs, aliases, and relationship targets.

The verify command is structural format validation. It is not an author signature check or a permission check.

## SLeeLa model

The source package lib/registry includes the compatibility classes SLRegistry, SLRegistryKey, SLRegistryValue, and SLRegistryProvenance, plus the 3.0 source shapes SLRegistryRecord, SLRegistryIdentity, SLRegistryRelationship, SLRegistryLifecycle, SLRegistryResolver, and SLRegistryReview.

SLList is a growable String list. It is used for ID/path/name references, not arbitrary class instances. The eight direct object slots remain a legacy display window; the native record store is authoritative.

## Compiler and Decompiler

Future compiler bindings should preserve record ID, namespace, schema version, source digest, declared dependency IDs, and compiler version in compiled-product metadata. Unsupported schema versions and malformed references must be rejected explicitly. Decompiler output may display these references but must not invent missing source provenance.

## Runtime and VM

A future typed host service can expose read-only identify, describe, resolve, verify, and review operations. Results should distinguish resolved, not-found, ambiguous, offline, incompatible, integrity-failed, denied, unsupported, malformed, and revision-conflict.

Keep the first VM-facing binding read-only. Do not add an opcode or alter the frozen ISA merely to make Registry visible. Standard and Premium VM generations must share one record schema and status mapping. A VM cache is not the source of truth.

## Build and test

- make -C registry all
- make -C registry test
- make registry-test
- CMake/CTest via registry/CMakeLists.txt and .github/workflows/registry-v3.yml

No compiler/runtime/VM binding should be called complete until an executable integration test crosses that boundary.
