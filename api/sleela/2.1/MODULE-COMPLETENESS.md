# API Module Completeness Matrix

This matrix describes the current repository shape, not a claim that every listed feature has passed CI. A directory may be a documentation facade, declarative schema, native source module, or aggregate build. The owning source and real build/test target take precedence over directory naming.

| API area | Entry point / source | Build integration observed | Completeness action |
|---|---|---|---|
| AI | `ai/README.md`, `ai/API.html`, XML and examples | No standalone Makefile identified in the API directory | Tie model/provider claims to concrete implementations and runnable tests. |
| Audio mixer | `audio-mixer/README.md`, JSON/XML/text examples | Configuration examples present; no standalone Makefile identified | State which formats are parsed and which live/file mixing operations are implemented. |
| BODI | `bodi/README.md`, `bodi/API.html` | Module Makefile present | Keep schema validation, execution and evidence generation separately testable. |
| Data analytics | `data-analytics/README.md`, `API.html` | Module Makefile present | Record numeric boundaries and test commands. |
| Database | `database/README.md`, `API.html` | Module Makefile present | Document optional driver dependencies and platform-specific behavior. |
| Email | `email/README.md`, `API.html` | Module Makefile present | Keep SMTP/TLS behavior tied to adapter implementation and tests. |
| HTTP multiplexing | `http-multiplexing/README.md` | Module Makefile present | Document protocol limits and test the actual multiplexing path. |
| Listener | `listener/README.md`, `sleela_listener.c/.h` | Built by aggregate `posting/Makefile` | Keep validation distinct from opening a network socket; test null, invalid and valid descriptors. |
| Router | `router/README.md`, `sleela_router.c/.h` | Built by aggregate `posting/Makefile` | Contract currently validates descriptors and performs exact method/path matching; do not imply wildcard routing. |
| Posting | `posting/API.html`, `sleela_post.c/.h` | Aggregate Makefile builds posting, listener and router static libraries | Test validation and bounded write behavior; distinguish formatting a request from network transport. |
| Media | `media/README.md`, `API.html` | No standalone Makefile identified in this API directory | Mark individual codecs/transforms/providers implemented, partial, or planned. |
| National relay | `national-relay/README.md`, C source | Module Makefile present | Keep identity helpers distinct from a complete relay service. |
| Native runtime | `native/runtime/README.md`, `BUILD.md`, C++ source | Build instructions documented | Record actual compiler/platform validation separately from adapter coverage. |
| Native memory | `native/memory/README.md`, C++ source | No standalone Makefile identified in this API directory | Preserve allocator ABI boundaries and add targeted lifecycle/limit tests through its owning build. |
| Nordshrift | `nordshrift/API.html`, `README.md` | Reference/schema surface; no standalone Makefile identified | Keep declarative semantics separate from side-effecting execution. |
| Regex | `regex/README.md`, `API.html` | Native C foundation referenced under `impl/core/` | Preserve the documented draft/implementation boundary and unsupported levels. |
| Server/webserver | Existing API pages and Makefiles | Module build files present | State exactly what is packaged and which OS targets have actually passed. |
| Sleela-complete | `sleela-complete/README.md` and class specifications | Specification/class layer | Do not present designer-facing class descriptions as production runtime implementations. |

## Rules for closing a gap

1. Inspect source and parent build files before creating a new Makefile. Reuse aggregate builds when they already own the module.
2. Add tests that execute the public C ABI or language interface, not just syntax or header inclusion.
3. Record the exact command, compiler, platform, exit status, and output when validation is performed.
4. Separate implemented behavior from partial facades, design-only surfaces, and future work.
5. Update this matrix, the focused module page, `api/API-CATALOG.md`, `api/README.md`, and `api/index.html` when public contracts change.
6. Regenerate `API.html` and `SOURCE-COVERAGE.html` only through a verified repository generator. Do not hand-edit a generated inventory and continue to label it exhaustive.
