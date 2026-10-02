# SLVM I/O Heuristic Layer

The I/O heuristic layer sits between VM requests and platform adapters. It evaluates request shape rather than declaring program intent.

Signals include request bursts, request size, aggregate bytes, file-load activity, network activity, dynamic instantiation, and resource-control operations.

Decisions are ALLOW, MONITOR, THROTTLE, or BLOCK. Large transfers and dynamic instantiation receive enhanced observation. Elevated or restricted security states influence the decision.

The design is suitable for later integration with SLeeLa HTTP/server paths and the `/resolver` capability without bypassing capability authorization.
