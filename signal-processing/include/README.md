<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# Public headers

- sleela_signal_processing.h is the stable C ABI. It defines sample/complex
  types, status codes, version macros, and pointer/length/capacity-based calls.
- sleela_signal_processing.hpp is the C++17 convenience layer, using vectors
  and documented exception behavior.

Keep this layer small and source-compatible. New C entry points should include
precise length, overlap, capacity, normalization, and failure contracts. Do not
expose compiler-specific types or internal implementation details in the ABI.

See ../docs/API-CONTRACTS.md for the current conventions.
