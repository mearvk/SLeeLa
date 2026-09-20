# The `.ledger` class

A `.ledger` file is a **tamper-evident chain of records**, one record per source
file, that binds together:

- a **QR insignia** (an SVG QR code encoding the record's identity),
- **chain management** — a **SHA-256** of the file, linked to the previous
  record's hash (a hash chain), and
- an **International Timestamp** — a UTC **ISO-8601** timestamp (`…Z`).

It is produced two ways, from the **same core logic**:

1. **Standalone** — `ledger/ledger.py` (and the C++ `ledger/ledger_tool`) walk a
   set of files and emit a `.ledger`.
2. **By the compiler** — the Nordshrift driver emits a `.ledger` next to each
   `.sleela` artifact it produces (see `impl/nordshrift/`).

## On "International Timestamp" (honest scope)

A fully notarized international timestamp is an **RFC 3161** token from a Time
Stamping Authority, which requires network access. This environment has none, so
the timestamp here is a **UTC ISO-8601** reading of the local clock — the
international *format*, not a notarized TSA token. Each record carries a
`timestamp_kind` field naming which it is (`"utc-iso8601"` here), and the format
reserves an optional `rfc3161` field where a real TSA token attaches in a
networked deployment. This is stated plainly rather than overclaimed.

## Record format

A `.ledger` is **JSON Lines**: the first line is a header, then one JSON object
per file record, in chain order.

Header line:

```json
{"ledger":"sleela","version":1,"algo":"sha256","chain":"prev-hash","genesis":"0000…0000"}
```

Record line (fields in a fixed order for stable hashing):

```json
{
  "seq": 1,
  "path": "http-3.0/http3_mac.c",
  "sha256": "<64 hex: SHA-256 of the file bytes>",
  "prev": "<64 hex: sha256 of the previous record's canonical bytes, or genesis>",
  "timestamp_utc": "2026-09-20T00:00:00Z",
  "timestamp_kind": "utc-iso8601",
  "record_hash": "<64 hex: SHA-256 over the canonical record without this field>",
  "qr_svg": "ledger/insignia/http-3.0/http3_mac.c.svg"
}
```

### Chain rule

- `genesis` is 64 zero hex chars.
- For record *n*, `prev` = `record_hash` of record *n−1* (or `genesis` for the
  first).
- `record_hash` = SHA-256 of the record's **canonical bytes** — the JSON object
  with keys in the order above, **excluding** `record_hash` itself, serialized
  compactly (no spaces, `sort_keys=false`). Altering any earlier record's
  content, hash, timestamp, or order changes its `record_hash`, which breaks
  every subsequent `prev` link — so the chain is tamper-evident.

### QR insignia

For each record a QR code is emitted as SVG at `qr_svg`. It encodes a compact
identity string:

```text
sleela-ledger:1|<seq>|<path>|<sha256>|<timestamp_utc>
```

The QR is a byte-mode QR symbol rendered as scalable SVG modules (dependency-free
encoder in `ledger/qr.py`). It is an *insignia* — a scannable restatement of the
record's identity — not a separate source of truth; the JSON fields are
authoritative.

## Verifying a `.ledger`

`ledger/ledger.py --verify <file.ledger>` recomputes each `record_hash` and
checks every `prev` link against the previous record, reporting the first break.
This confirms the chain has not been reordered or edited.
