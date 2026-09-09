<!-- @MT:SLE -->
# SEAL — Integrity Metalayer for Documents

## Series MT-SEAL-0001

| Field | Value |
|-------|-------|
| Document Series | MT-SEAL-0001 |
| Document Status | Proposal (first draft) |
| Revision | 0.1.0 |
| Date | September 2026 |
| Governing Tool | `seal` ([`impl/seal/seal.py`](impl/seal/seal.py)) |
| Companion Documents | `METATAGS.md` (MT-META-0001), `SHEET.sheet` |
| Algorithm | HMAC-SHA256, keyed by a password |

> **Normative status.** **must / must not / shall / required / forbidden** are
> normative; **should / may / recommended / optional** are guidance. This first
> draft is a *proposal*; the normative words describe the design as proposed.

---

## 1. What a seal is

A **seal** is a tamper-evidence **metalayer** bound to a source or compiled
document. It answers one question — *is this document still in order?* — and it
answers it only to someone who asks carefully and knows the word.

Three properties define it:

1. **Mainly non-editable.** The seal is a keyed HMAC. You cannot alter the
   document and re-forge a matching seal without the password. Any edit — one
   byte — breaks the mark.
2. **Not visible.** The seal is **never written into the document's visible
   bytes**. Opening the file in `vim` or `gedit` shows nothing added; the text is
   byte-for-byte what it always was. The seal lives in a sidecar the editors
   never open (§4).
3. **Revealed only through its lip.** The seal is dormant. It discloses nothing
   until it is loaded through its **lip** — a careful, neat call that supplies
   the *exact path* and the *exact password* (§5). A careless or wrong call
   reveals nothing.

On a correct, careful load of an unmodified document the seal reports:

> **IN ORDER — careful until tine.**

"Tine" is the point of the mark — the meaning of the marksman: the document
holds true up to the point the mark was set.

## 2. The solve matrix — the machine natures of the code and its numbers

Each seal carries a **solve matrix**: integer rows that name the document's
*machine natures* and its *numbers*. The matrix is derived deterministically from
the document and recomputed at reveal time, so it both fingerprints the file and
describes it. Rows:

| Row | Meaning |
|-----|---------|
| `bytes` | canonical byte length (CRLF/CR normalized to LF) |
| `lines` | LF-delimited line count |
| `words` | whitespace-delimited token count |
| `numbers` | `[count of numeric literals, sum mod 3024, max]` — the document's *numbers* |
| `mt` | 16-int presence vector over the MT-META-0001 alphabet `PMRTCSXLGAWNVDHE` |
| `shape` | `[distinct byte values, non-ASCII byte count]` — its *machine nature* |
| `spine` | first 8 bytes of `SHA-256(content)` as integers — the numeric spine |

The `numbers` modulus and the matrix's ties to the metatag alphabet are not
arbitrary: **3024** is the System depth invariant from `SHEET.sheet` (§0), and
the `mt` row binds the seal to the file's MT-META-0001 metatags. A sealed file
therefore carries, in one place, *what it is* (metatags), *what it's made of*
(shape/numbers), and *that it is intact* (spine + binding).

## 3. The binding (mainly non-editable)

```
key      = SHA-256("MT-SEAL:" + password)
message  = canonical_content ‖ 0x00 ‖ serialize(solve_matrix)
binding  = HMAC-SHA256(key, message)          # stored in the seal
```

At reveal, the tool recomputes the matrix and the binding from the *current*
document and the *supplied* password and compares (constant-time) against the
stored binding. Because the password is the HMAC key and is **never stored**, the
seal cannot be re-forged from the seal alone, and the verdict cannot be produced
without the word.

## 4. Where the seal lives (invisible to vim/gedit)

The seal is stored in a **sidecar registry**, not in the document:

```
.mt/seals/<relative-path>.seal      # one JSON record per sealed document
```

The document file is not modified when it is sealed (verified: its SHA-256 is
identical before and after). Editors open the document, not the sidecar, so the
seal is invisible in normal viewing.

> **Optional platform hardening.** Where the filesystem supports it, the same
> binding **may** additionally be mirrored into an OS extended attribute
> (`user.mt.seal` on Linux, `com.apple.metadata` on macOS). On filesystems
> without xattr support (as in some sandboxes/CI), the sidecar registry is the
> portable primary store and is sufficient on its own.

The seal record stores the matrix, the binding, the algorithm, and a `lip`
hint — **never the password**.

## 5. The lip — a careful, neat load

The **lip** is the seal's only opening. It parts only for a load that is *careful
and neat*: the exact target path **and** the exact password.

```sh
# affix a dormant seal (document is left untouched)
impl/seal/seal.py seal   README.md --pass '<the word>'

# the lip: careful load -> verdict (+ solve matrix if in order)
impl/seal/seal.py reveal README.md --pass '<the word>'

# quiet check for CI (exit code only)
impl/seal/seal.py verify README.md --pass '<the word>'

# list sealed files (names only; discloses no contents)
impl/seal/seal.py list
```

### Verdicts

| Situation | Message | Exit |
|-----------|---------|------|
| Correct word, document intact | `IN ORDER — careful until tine.` + solve matrix | 0 |
| Wrong word (document intact) | `SEALED — the lip does not part. (careful, neat, and the word.)` | 1 |
| Document modified since sealing | `TAMPERED — the mark is broken.` | 3 |
| No seal present (dormant/absent) | `SEALED — the lip does not part.` | 1 |

A wrong word and a missing seal give the *same* closed response, so the seal
never reveals whether it exists to someone without the word — it only opens for
the careful, neat, correct load.

## 6. Verification performed

Against `README.md` in this repo (password withheld here):

- **Invisibility:** `sha256sum README.md` is identical before and after sealing;
  the seal appears only in `.mt/seals/README.md.seal`.
- **In order:** correct password → `IN ORDER — careful until tine.` with the full
  solve matrix (including `numbers.max = 3024` and the `mt` vector matching the
  file's `@MT:SXG` metatags).
- **Wrong word:** → `SEALED — the lip does not part.`
- **Tamper:** appending one line then revealing with the correct password →
  `TAMPERED — the mark is broken.`; restoring the file returns it to `IN ORDER`.

## 7. Relationship to MT-META-0001

The seal reuses the metatag alphabet (the `mt` matrix row) and the `.mt/`
namespace introduced by [`METATAGS.md`](METATAGS.md). A document's metatags say
*what it is*; its seal says *that it is still that, unaltered, until tine*.

## 8. Open questions (for reviewers)

- Should compiled artifacts carry the seal in a reserved, digest-excluded trailer
  (for formats that allow it) in addition to the sidecar?
- Multi-party seals: allow N passwords (M-of-N) so a document is "in order" only
  when several marksmen agree?
- Should `reveal` optionally emit the solve matrix as machine-readable JSON for
  downstream verification pipelines?
