# AMR-NB Codec

**Codec id:** `SLEELA_CODEC_AMR_NB` · **State:** Backend · **Extensions:** `.amr` · **MIME:** `audio/amr`

Adaptive Multi-Rate Narrowband speech. Identified by the '#!AMR\n' magic; encode/decode require an approved AMR-NB backend.

## Files

| File | Language | Role |
|------|----------|------|
| `amr_nb.c` | C | Plugin vtable (`sleela_codec_plugin_amr_nb`), state + dispatch. |
| `amr_nb_detail.cpp` | C++ | Content signature probe (`sleela_amr_nb_probe_bytes`). |
| `amr_nb_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
