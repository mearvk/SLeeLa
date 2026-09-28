# AE6E66 Integrity Manifest

The production manifest is intentionally not generated from a mutable working tree by the module itself. A release process must generate a complete SHA-256 manifest from a reviewed checkout, review it, and bind it to the exact approved commit/release.

The manifest must not be treated as trusted merely because it is present on disk. Release provenance and the pinned source identity are part of the trust decision.
