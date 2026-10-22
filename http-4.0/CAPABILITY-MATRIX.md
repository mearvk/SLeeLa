# HTTP 4.0 Capability Matrix

Status terms: **Modelled** means a design/data type exists; **Implemented** means code exists; **Tested** means the named test has actually been run and recorded; **Production-ready** requires cross-platform, security, resource, and recovery evidence. A source file alone does not prove a capability.

| Capability | Current evidence in repository | Status | Remaining gate |
|---|---|---|---|
| 32-byte big-endian frame codec | http4_protocol.c and protocol self-test | Implemented; latest edits not run here | Run full tests and golden-byte vectors |
| Version/type/length validation | http4_protocol.c | Implemented; unknown flag rejection added | Test all flag/type boundaries |
| Stream state and sequencing | http4_session.c | Implemented as bounded reference model | Integrate with server path; define both directions |
| Stream/queue/flow-control enforcement | WINDOW frame taxonomy only; no integrated credit scheduler | Partial | Enforce credit and global buffered-byte limits in production path |
| Segmentation/reassembly | http4_segmentation.c and http4_reassembly.c | Implemented | Fuzz and bound aggregate active reassembly memory |
| DOWNLOAD metadata parsing | http4_server.c | Strict parser changes added | Run regression tests; authenticated resume store and digest checks remain |
| Persistent resume checkpoints | RESUME documentation only | Not implemented | Authenticated durable store, expiry, crash consistency |
| Peer authentication / transport security | SECURITY.md | External contract only | Authenticated TLS/QUIC adapter and integration evidence |
| Frame MAC / key rotation | Not present in native frame codec | Not implemented | Only add if needed above authenticated carrier; independently specify and review |
| Capability negotiation | HTTP header negotiation and capability documentation | Partial | Bind offers/selection to authenticated transcript; prevent downgrade |
| C/SLeeLa parity | SLeeLa model files under lib/http-4.0 | Modelled | Shared vectors and actual compiler runs |
| Fuzz harness | http4_fuzz.c + optional Makefile target | Added, not run | Run with Clang libFuzzer and retain corpus/results |
| Linux/macOS/Windows CI | No verified CI evidence recorded in this directory | Not verified | CI matrix, sanitizers, artifacts |
| Formal/security review | SPEC.md and CONFORMANCE.md define requirements | Not performed | Independent review and release evidence |
| IETF HTTP/3/QUIC carrier | Current server uses HTTP/1.x over TCP | Not implemented | Implement and test a real adapter; never infer from protocol name |

## Release rule

Do not label HTTP 4.0 production-ready until all mandatory conformance tests pass on supported platforms, resource limits are enforced on the real server path, carrier authentication is demonstrated, resume integrity is verified end-to-end, and an independent security review is complete.
