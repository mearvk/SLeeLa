# Sleelavirin™ Package Specification

## Package identity

- Package name: **Sleelavirin™**
- SLeeLa location: `/antivirus`
- Package role: security/antivirus co-package
- Ownership: declared by the package author
- Primary purpose: attach security concerns and verification material to a deliverable

## Co-package rule

The author may ship Sleelavirin material with an ordinary SLeeLa package. The antivirus co-package is scoped to that deliverable unless an explicit shared security package is declared.

## Required metadata

Each production Sleelavirin package should identify:

- package identity and version;
- protected artifact(s);
- supported platforms;
- rule/signature set versions;
- digest algorithm and artifact digest;
- scanner/provider dependencies;
- policy requirements;
- provenance;
- update date;
- test status.

## Separation of concerns

Detection, policy, and remediation are separate stages. A detection result must not silently imply deletion, quarantine, blocking, or privilege escalation.
