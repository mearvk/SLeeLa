# Global Ring of Trust for OS Developers

## Meaning of the concept

The **Global Ring of Trust (GRT)** is a proposed, interoperable governance and evidence model for people and organizations that develop operating-system components, user-mode system libraries, drivers, and related tooling. It is a framework for making claims and evidence inspectable; it is not a global authority, a root certificate, a security certification, or proof that software is safe.

“Global” describes the intended ability to exchange verifiable evidence across jurisdictions and operating systems. It does not imply universal membership, universal legal recognition, or that one organization controls the trust roots. Participation must remain plural, transparent, and independently reviewable.

This proposal concerns accountable development and distribution. It does **not** authorize access to a machine, bypass OS permission boundaries, or make privileged system operations safe by virtue of a signature or membership.

## Design principles

1. **Evidence, not rank.** Trust is a set of scoped, time-bounded claims backed by evidence—not a single score, badge, or permanent tier.
2. **Plural roots.** No single maintainer, vendor, government, or foundation is the global trust anchor. Roots and policy changes require transparent, independently governed processes.
3. **Least authority.** A trust statement never grants runtime privileges. OS authorization remains with the host OS, administrator, user, and application policy.
4. **Separate identities.** Developer identity, organization identity, source repository, build process, artifact digest, and runtime publisher are distinct claims and must not be conflated.
5. **Reproducibility and provenance.** Prefer reproducible builds and verifiable source-to-artifact provenance. When builds are not reproducible, disclose the limitation and retain auditable build records.
6. **Revocability and expiry.** Membership, signing credentials, and endorsements have defined scopes and expiration. Revocation is discoverable, prompt, and does not erase historical records.
7. **Privacy by default.** Collect only information necessary to validate a claim. Publish redacted evidence where personal or operational data is not required.
8. **Interoperability without forced uniformity.** Shared evidence formats should allow OS-specific policy and legal requirements to remain explicit.
9. **Human accountability.** Automated checks assist review but do not replace accountable maintainers, independent security review, disclosure processes, or appeal.
10. **Fail closed for verification.** Missing, malformed, expired, or unverifiable evidence is reported as unknown or invalid—not silently treated as trusted.

## Proposed rings (descriptive, not a hierarchy of authority)

- **Observer:** publishes a project identity and contact/security policy; no assurance is implied.
- **Contributor:** demonstrates control of a project identity and follows its disclosed contribution and review practices.
- **Verified maintainer:** has a verified role in a project, uses protected release credentials, and publishes release provenance and a vulnerability-reporting path.
- **Independently reviewed project:** publishes an independent assessment with scope, date, assessor, findings, and limitations. The assessment is not a guarantee of absence of defects.
- **Trust-root operator:** operates an openly governed identity or transparency service under published controls, audit requirements, conflict-of-interest rules, and a defined removal/appeal process. This role does not confer authority over operating systems or users.

These labels must never be reduced to a universal trust score. A project may have different assurance for different release channels, components, platforms, and dates.

## Minimum trust statement

A machine-readable statement should identify, at minimum:

- subject and issuer identifiers, with identifier type and verification method;
- component, repository, release channel, and supported platform scope;
- artifact digest and, where available, source revision and build provenance;
- claim type, evidence references, assessment scope, and known limitations;
- issue and issuance times, expiry, policy/version, and revocation-check location;
- signature algorithm/key identifier and an independently discoverable transparency record, where used.

A signature proves only that a key signed bytes. Identity binding, key custody, build integrity, review quality, and software behavior require separate evidence and review.

## OS-level-support boundary

This repository's `os-level-support` is a user-mode portability library. The GRT may describe its maintainers, source, releases, provenance, and test evidence. It must not be represented as a kernel authorization mechanism.

The library does not implement or endorse system-call interception, kernel hooks, kernel memory/object access, driver installation/removal, stealth, security-control bypass, or trust-based privilege elevation. Any future privileged OS integration requires a separate proposal, threat model, platform-specific review, explicit administrator consent, and native OS authorization; GRT membership is never sufficient authorization.

## Governance questions before production use

The GRT remains a concept until stakeholders define its independent governance body (if any), policy-change quorum, root onboarding/offboarding, conflict resolution and appeals, key compromise response, revocation propagation targets, privacy/data retention, audit funding, jurisdictional review, and interoperable statement format. No trust root or certification should be deployed based solely on this document.

## Status

**Concept / discussion draft.** This document creates no trust roots, credentials, endorsements, verification service, legal status, or certification. It makes no claim that any developer or project is currently a member.
