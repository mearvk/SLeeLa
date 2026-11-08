# advantage/examples — trade examples (consuming the C/C++ model)

`trade_examples` is a single C++ program that **consumes the advantage model as
a whole** and produces worked trade material.

## What it consumes

- `../advantage.c` + `../advantage.hpp` — the C core advantage math
  (`advantage_value` / `advantage_mean`) via its C++ wrapper
  (`advantage::value` / `advantage::mean`).
- `../transport/digest.cpp` + `../transport/digest.h` — the real SHA-256
  content-addressing (`advantage::sha256_hex`), the same code the acquisition
  layer uses to key data.

Both are compiled and linked as-is by this folder's `Makefile` (digest needs
OpenSSL).

## What it produces

1. **Positive trade examples** from **presumed / illustrative** internet
   sources. The URLs are placeholders (`presumed.example/...`), **not** live
   feeds — each is content-addressed with the digest code so it has a stable
   object id, and each trade's reward series is scored against a benchmark
   baseline by the advantage math. All four examples come out positive.

2. **A relevance & importance sequence toward a Trading Method** for four
   audiences, using the presumed framing **TV = a Source, Entertainment = a
   Captain, Health = a Brand**:

   | # | Audience | Relevance | Framing | Role |
   |--:|----------|:---------:|---------|------|
   | 1 | Traders | 10/10 | Entertainment as a Captain | originate and steer the method |
   | 2 | Consumers | 8/10 | TV as a Source | receive and validate demand |
   | 3 | Helpers | 6/10 | TV as a Source + Captain's relay | distribute and support execution |
   | 4 | Healers | 7/10 | Health as a Brand | repair trust and certify outcomes |

3. **Eight models ranked A..D**, where **D = most Cement** (foundational,
   concrete) and **A = most Brand** (elevated, identity-level):

   | Tier | Meaning | Models |
   |:----:|---------|--------|
   | D | most Cement (foundational / concrete) | `settlement-rail`, `custody-vault` |
   | C | structural (concrete made useful) | `liquidity-plumbing`, `risk-ledger` |
   | B | market-facing (productized / sold) | `retail-storefront`, `advisory-desk` |
   | A | most Brand (elevated / identity-level) | `trust-mark`, `care-seal` |

   The advantage math shows the Cement→Brand climb (≈0.05 at D up to ≈1.10 at A).

## Build & run

```sh
make        # builds ./trade_examples (links advantage.c + digest.cpp, needs OpenSSL)
make run    # builds and runs
make clean
```

## Honest notes

- The internet sources are **presumed / illustrative**, explicitly labelled as
  such — this folder does not fetch live market data. The digest code is real;
  the data behind the URLs is not.
- `advantage/examples/` lives outside `lib/`, so it is **not** covered by the
  SHA-256 manifest gate or the library-inventory gate (those cover `lib/**` and
  impl source only); no manifest/inventory updates are needed for this folder.
- The A↔D ranking uses the exact semantics requested: **D is the most Cement**
  (most foundational), **A is the most Brand** (most elevated).
