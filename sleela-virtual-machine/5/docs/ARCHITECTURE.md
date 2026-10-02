# SLVM/5 Architecture

SLVM/5 builds on /4 with reproducibility, durable evidence, transaction boundaries, and checkpoint-based recovery.

SLeeLa source -> authoritative compiler -> Output Symbols/Core -> verified artifact -> SLVM/5 -> authenticated broker/capability boundary -> OS adapter.

## Reproducibility

Build identity, artifact hash, manifest hash, VM identity, runtime identity, crypto provider, and policy hash are represented in attestation evidence.

## Policy

A policy snapshot has a version and hash. Sensitive execution uses the exact snapshot selected for the operation. Immutable policy snapshots prevent silent policy mutation during an execution context.

## Transactions

Distributed operations can carry a transaction context. Commit/abort state is explicit and observable. Transaction support does not imply that arbitrary OS operations are magically atomic; adapters must define their actual commit semantics.

## Recovery

Checkpoint records contain integrity evidence and transaction context. Recovery validates the checkpoint before restoring execution state.

## Resolver

Resolver provenance is retained with the execution evidence. DNS/IP/path changes remain subject to capability and certificate policy.

## Evidence

Observer checkpoints correlate execution, broker, transaction, resolver, certificate, recovery, and attestation events.
