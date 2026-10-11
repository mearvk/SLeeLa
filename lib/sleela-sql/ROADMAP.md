# SLeeLa SQL Functionality Roadmap

This document tracks the native C11 engine, C++17 facade, and eventual SLeeLa runtime bridge. The engine remains a local CSV-backed SQL dialect; it is not a network server or a claim of full SQLite, PostgreSQL, or MySQL compatibility.

## Current baseline

- [x] Portable C/C++ build and CI on Linux, macOS, and Windows (MinGW-w64).
- [x] CREATE TABLE / DROP TABLE / INSERT / SELECT / SHOW TABLES.
- [x] Prepared positional parameters.
- [x] CREATE TABLE IF NOT EXISTS and SELECT COUNT(*) with an optional equality predicate.
- [x] SQL-dialect UPDATE and DELETE with an optional single equality predicate.
- [x] UPDATE/DELETE rewrite through a temporary file; replacement failures are reported and Windows attempts to restore a backup.
- [x] C and C++ smoke tests exercise CRUD updates/deletes.

## Priority A — persistence and integrity

- [ ] Define and implement explicit transaction API with documented durability and rollback guarantees.
- [ ] Add crash-recovery journal/manifest and recovery tests before claiming transaction safety.
- [ ] Add portable inter-process locking and deterministic concurrent-writer behavior.
- [ ] Detect truncated/oversized records and malformed CSV instead of silently accepting partial records.
- [ ] Parse quoted CSV fields that contain embedded newlines.
- [ ] Distinguish NULL from empty text; preserve error context and affected-row counts.
- [ ] Validate write/flush/close errors consistently in every write path.

## Priority B — complete basic SQL operations

- [x] UPDATE and DELETE for SQL dialect with one equality predicate.
- [ ] UPDATE/DELETE fluent SLeeLaSQL forms.
- [ ] INSERT with explicit column lists and multi-row values.
- [ ] ALTER TABLE ADD/RENAME/DROP COLUMN.
- [ ] Column metadata and typed values: integer, unsigned integer, decimal, text, Boolean, date/time.
- [ ] NOT NULL, UNIQUE, PRIMARY KEY, DEFAULT and foreign-key constraints.

## Priority C — query language

- [ ] Tokenizer/parser that handles quoted strings, escaped identifiers, and parentheses consistently.
- [ ] Comparison operators, AND/OR/NOT, parentheses, IN, BETWEEN, LIKE, IS NULL.
- [ ] ORDER BY, LIMIT, OFFSET, DISTINCT and column aliases.
- [ ] INNER JOIN and LEFT JOIN.
- [ ] SUM/AVG/MIN/MAX, GROUP BY and HAVING.
- [ ] Subqueries after expression evaluation and join semantics stabilize.

## Priority D — storage and query performance

- [ ] Storage interface independent of CSV persistence.
- [ ] Indexed backend and persistent B-tree/hash indexes.
- [ ] Query plan representation and EXPLAIN.
- [ ] Streaming result API and bounded-memory execution.
- [ ] Configurable cache, limits, and query time/resource controls.

## Priority E — API and language integration

- [ ] Stable error categories with diagnostics and row counts.
- [ ] Full C/C++ parity for supported operations and API conformance tests.
- [ ] SLeeLa runtime binding that executes native statements and returns typed result rows.
- [ ] Database path, permissions, and resource configuration aligned with SLeeLa installation conventions.

## Priority F — release hardening

- [ ] Shared conformance suite for SQL and SLeeLaSQL dialects.
- [ ] Parser/CSV fuzzing and sanitizers.
- [ ] Crash-injection, recovery, concurrency, and large-table tests.
- [ ] Cross-platform CI for every supported operation.
- [ ] Benchmarks and documented compatibility matrix.

## Release rule

Do not advertise ACID transactions, concurrent writer isolation, full SQL compatibility, or a production-hardened security boundary until the relevant implementation and failure-mode tests pass.
