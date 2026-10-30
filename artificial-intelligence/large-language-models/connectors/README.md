# Connectors

Connectors adapt a data source to the LLM data reader. The stable C callback contract is declared in `../large_language_models_data.h`.

## Built-in connector

- Local files: `sllmd_file_open`, `sllmd_file_read`, and `sllmd_file_close`.

## Custom connectors

Implement `sllmd_read_fn` for a database cursor, HTTP response body, object-storage stream, memory buffer, or application-owned source. Each read returns a bounded byte chunk; return `SLLMD_OK` with zero bytes at end-of-stream. Keep connector state alive until `sllmd_jsonl_read` returns.

Network connectors should enforce TLS verification, authentication outside source code, response-size limits, timeouts, and trusted-host policies. The core library intentionally does not embed a network client or credentials.
