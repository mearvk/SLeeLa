# Post-Quantum Integration Plan (ML-KEM / ML-DSA)

CNSA 2.0 names two NIST post-quantum standards:

- **ML-KEM** (FIPS 203) — key encapsulation, derived from CRYSTALS-Kyber.
- **ML-DSA** (FIPS 204) — digital signatures, derived from CRYSTALS-Dilithium.

Their stable C entry points are already declared in
[`include/slcrypto.h`](include/slcrypto.h) and defined in
[`src/pq.c`](src/pq.c), currently returning `SLCRYPTO_ERR_UNIMPL`.

## Why these are not hand-rolled here

A faithful, correct ML-KEM/ML-DSA implementation requires a substantial lattice
stack: polynomial arithmetic in R_q, the number-theoretic transform (NTT),
centered-binomial and rejection sampling, a SHAKE-128/256 XOF, and
byte-level packing exactly matching the FIPS test vectors. Getting any of this
subtly wrong produces output that validates against nothing and silently
weakens security. For a security-relevant primitive the responsible path is to
**vendor an audited reference implementation**, not to re-derive it.

## Recommended integration

1. Vendor the reference code from one of:
   - **pq-crystals/kyber** and **pq-crystals/dilithium** (the standards'
     reference implementations), or
   - **liboqs** (Open Quantum Safe), which packages both behind one API.
2. Add it under `crypto/pq/` with its license preserved.
3. Implement the `slc_mlkem_*` / `slc_mldsa_*` functions in `src/pq.c` as thin
   adapters mapping the `slc_*_param` enums to the vendored parameter sets and
   buffer sizes.
4. Add FIPS 203 / FIPS 204 Known-Answer Tests to `test/` and wire them into
   `make test`.
5. Note the SHAKE XOF: FIPS 203/204 use Keccak; the Keccak-f[1600] permutation
   already present in [`src/sha3.c`](src/sha3.c) can be factored out to provide
   SHAKE-128/256 for the sampler, avoiding a second Keccak core.

Until step 3 lands, callers receive `SLCRYPTO_ERR_UNIMPL` and must not treat the
PQ entry points as available.
