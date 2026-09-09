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
and neat*: the exact target path **and** the word(s).

```sh
# affix a dormant seal (document left untouched; stored in the sidecar)
impl/seal/seal.py seal   README.md --pass '<the word>'

# the lip: careful load -> verdict (+ solve matrix if in order)
impl/seal/seal.py reveal README.md --pass '<the word>'

# quiet check (exit code only); add --json for a machine-readable report
impl/seal/seal.py verify README.md --pass '<the word>' --json

# list sealed files (names only; discloses no contents)
impl/seal/seal.py list
```

### Verdicts

| Situation | Message | Exit |
|-----------|---------|------|
| Enough correct words, document intact | `IN ORDER — careful until tine.` + solve matrix | 0 |
| Too few / wrong words (document intact) | `SEALED — the lip does not part.` | 1 |
| Document modified since sealing | `TAMPERED — the mark is broken.` | 3 |
| No seal present (dormant/absent) | `SEALED — the lip does not part.` | 1 |

A wrong word and a missing seal give the *same* closed response, so the seal
never reveals whether it exists to someone without the word — it only opens for
the careful, neat, correct load.

## 6. M-of-N marksmen (multi-party seals)

A seal may bind **several marksmen**, each with their own word, and require a
**threshold M** of them to agree before it reads *in order*:

```sh
# three marksmen; any two of them must agree
impl/seal/seal.py seal build.out --pass alpha --pass bravo --pass charlie --threshold 2

impl/seal/seal.py reveal build.out --pass alpha --pass bravo   # 2-of-3 -> IN ORDER
impl/seal/seal.py reveal build.out --pass alpha                # 1-of-3 -> the lip stays shut
```

The seal stores **one HMAC binding per marksman** plus a short public
`marksman id` (a non-reversing SHA-256 prefix) so `reveal` can report *which* and
*how many* marksmen agreed — **without ever storing a word**. A single-`--pass`
seal is simply the `1-of-1` case, so existing seals keep working unchanged.

## 7. Trailer mode (compiled / appendable artifacts)

For artifacts where a sidecar is inconvenient, the seal can be embedded as a
**trailer that is excluded from its own digest**:

```sh
impl/seal/seal.py seal artifact.bin --pass '<word>' --trailer
```

The trailer is a single fenced line — `<!-- MT-SEAL-0001:BEGIN {…} MT-SEAL-0001:END -->` —
appended after the body. The binding is computed over the body **before** the
trailer, so the trailer never seals itself: re-sealing an unchanged artifact
produces the identical binding (idempotent), and any change to the *body* still
reads `TAMPERED`. `reveal`/`verify` auto-detect a trailer and fall back to the
sidecar otherwise.

## 8. CI gate — passwordless integrity

`seal audit` verifies that **every sealed document still matches its recorded
solve matrix**. Because the matrix pins each document's shape, this catches
tampering **without any password**, so no secret is needed in CI:

```sh
impl/seal/seal.py audit            # exit 0 = all intact; exit 3 = a seal broke
impl/ci/mt-check.sh                # metatag scan + seal audit, one gate
```

The bundled [`impl/ci/mt-check.sh`](impl/ci/mt-check.sh) runs the MT-META-0001
`metatag scan` and the MT-SEAL-0001 `seal audit` together, and the GitHub Actions
workflow [`.github/workflows/mt-check.yml`](.github/workflows/mt-check.yml) runs
it on every push and pull request.

> **Note on storage.** Where the filesystem supports it, the binding **may** also
> be mirrored into an OS extended attribute (`user.mt.seal` on Linux,
> `com.apple.metadata` on macOS). On filesystems without xattr support (as in
> some sandboxes/CI), the sidecar registry is the portable primary store and is
> sufficient on its own. The seal record stores the matrix, the bindings, the
> algorithm, and a `lip` hint — **never any password**.

## 9. Verification performed

- **Invisibility (sidecar):** `sha256sum README.md` is identical before and after
  sealing; the seal appears only in `.mt/seals/README.md.seal`.
- **In order:** enough correct words → `IN ORDER — careful until tine.` with the
  full solve matrix (`numbers.max = 3024`; `mt` vector matching the file's
  metatags).
- **M-of-N:** 1-of-3 → the lip stays shut; 2-of-3 → `IN ORDER`; one correct plus
  one wrong word → still shut (only distinct marksmen count).
- **Trailer:** reveal via trailer → `IN ORDER`; re-seal is idempotent (identical
  binding — the trailer is excluded from its own digest); a body edit → `TAMPERED`.
- **JSON:** `reveal --json` / `verify --json` emit verdict + marksman tally + matrix.
- **CI:** `mt-check.sh` is green on the clean tree; tampering a sealed file →
  `INTEGRITY FAILURE` (exit 1); an out-of-order metatag → scan failure; restoring
  returns to green.

## 10. Relationship to MT-META-0001

The seal reuses the metatag alphabet (the `mt` matrix row) and the `.mt/`
namespace introduced by [`METATAGS.md`](METATAGS.md). A document's metatags say
*what it is*; its seal says *that it is still that, unaltered, until tine*.
