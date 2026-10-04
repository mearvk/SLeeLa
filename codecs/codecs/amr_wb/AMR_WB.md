# AMR-WB Codec

**Codec id:** `SLEELA_CODEC_AMR_WB` · **State:** Backend · **Extensions:** `.awb` · **MIME:** `audio/amr-wb`

Adaptive Multi-Rate Wideband speech. Identified by the '#!AMR-WB' magic; encode/decode require an approved AMR-WB backend.

## Files

| File | Language | Role |
|------|----------|------|
| `amr_wb.c` | C | Plugin vtable (`sleela_codec_plugin_amr_wb`), state + dispatch. |
| `amr_wb_detail.cpp` | C++ | Content signature probe (`sleela_amr_wb_probe_bytes`). |
| `amr_wb_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
