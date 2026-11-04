# Global Ring of Trust — Evidence and Conformance Checklist

This checklist is a proposal for future implementers. It does not certify this repository or any OS developer.

## A. Claim clarity

- [ ] Every claim has a named subject, issuer, type, scope, issue time, expiry, and policy version.
- [ ] Project identity, maintainer identity, source revision, artifact digest, build provenance, and independent assessment are represented separately.
- [ ] Claims state evidence, assurance level, limitations, and the method used to verify identity.
- [ ] “Trusted” is never used without explaining what was checked and when.

## B. Release integrity

- [ ] Release digests are computed with a current, explicitly named cryptographic hash.
- [ ] A signature is bound to the exact manifest bytes and a declared signing-key identifier.
- [ ] Key lifecycle documentation covers custody, access control, rotation, revocation, and compromise response.
- [ ] Build provenance identifies source revision, builder identity, toolchain, build parameters, and output digest.
- [ ] Reproducibility results and exceptions are published with their exact scope.

## C. Governance and review

- [ ] Root and policy governance is plural, transparent, and has documented quorum and change history.
- [ ] Reviewers disclose conflicts and publish assessment scope and limitations.
- [ ] Membership suspension, removal, correction, and appeal have documented procedures.
- [ ] Revocations are timestamped, publicly discoverable, and monitored for propagation.
- [ ] No member or trust-root operator can grant OS-level privilege through membership.

## D. Privacy and accessibility

- [ ] Personal data collection is minimized and retention is specified.
- [ ] Public evidence is redacted where disclosure is unnecessary for verification.
- [ ] Users can understand the difference between identity, provenance, assessment, and runtime permission.
- [ ] Verification failures, unknown status, and service outages are displayed distinctly.

## E. OS-level-support implementation gate

Before adding any trust-statement code to the C/C++ library:

- [ ] Choose a published, interoperable data format and supported signature profiles; do not invent cryptography.
- [ ] Define canonicalization and exact signed bytes; reject ambiguous encodings and duplicate fields.
- [ ] Add size, depth, and resource limits for untrusted input.
- [ ] Separate parsing, cryptographic signature checking, identity binding, revocation lookup, and policy evaluation into distinct result states.
- [ ] Make network access, if ever required, explicit and caller-controlled.
- [ ] Do not download/install trust roots automatically.
- [ ] Do not execute code, load drivers, install hooks, change system settings, or elevate privileges based on a validated statement.
- [ ] Test malformed inputs, truncation, expiry, revocation, key rotation, wrong scope, replay, and platform-independent behavior.
- [ ] Obtain independent security review before using the result to influence software distribution or execution decisions.

## Current status

All items above are proposed requirements, not completed implementation or certification. The Global Ring of Trust remains a concept pending independent governance and standards selection.
