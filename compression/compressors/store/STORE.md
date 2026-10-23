# Store (raw)

**Algorithm id:** `SLEELA_COMPRESSION_STORE` · **State:** Native · **Extensions:** `.bin` · **MIME:** `application/octet-stream`

The identity transform: "compress" copies the input verbatim and "decompress"
copies it back. It is the raw/store boundary every other algorithm is measured
against, and a guaranteed-available fallback. There is no header, so `probe()`
never positively claims arbitrary bytes.

## Files

| File | Language | Role |
|------|----------|------|
| `store.c` | C | Plugin vtable (`sleela_compressor_plugin_store`), copy compress/decompress. |

## Capabilities

- **compress / decompress** — lossless identity copy into a plugin-owned buffer.
- **probe** — always `SLEELA_COMPRESSION_ERR_FORMAT`; Store is selected
  explicitly, not auto-detected.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
