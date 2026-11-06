# Registry 3.0 — Modium Catalog Contract

## Responsibility split

Modium owns storage operations and artifact-integrity verification. Registry owns durable catalog identity, classification, lifecycle, and relationships describing those resources.

Registry never mounts a volume, claims a device, formats storage, reads or writes a storage region, or restores data. It stores stable record IDs and descriptive locator metadata. Modium resolves the current host location and returns bounded VM-local handles.

## Region record

A Modium region is recorded as:
- namespace: sleela.modium
- kind: region
- stable 32-hex record ID;
- region type: file-backed or removable-volume;
- descriptor format version;
- capacity in bytes, bounded to 256 MiB through 64 GiB;
- owner label;
- descriptive locator (not the identity);
- device seal for removable volume registrations;
- integrity state and lifecycle;
- typed stores relationships to VM-version or artifact record IDs;
- provenance and last-verification evidence supplied by Modium.

The native registration API accepts already-inspected metadata only. For removable media it requires a 64-hex device seal. Registration starts unverified and registered. It does not create or initialize storage.

## Lifecycle

Expected transitions:
- discovered -> inspected -> registered;
- registered -> verified only after a Modium verification result;
- verified -> available only while the current resolution and verification remain valid;
- available -> offline or quarantined when a volume disappears, is foreign-sealed, or fails integrity;
- quarantined -> verified only after explicit review and successful fresh verification;
- deprecated -> retired after dependents are accounted for.

The native record engine enforces allowed state edges and refuses verified/available transitions for a Modium region unless its recorded integrity state is intact.

Important limit: the current in-process C API does not authenticate which caller supplied an integrity status. A program with direct access to the setter can report an arbitrary status. Therefore, the lifecycle check is semantic validation, not a complete security boundary. Production integration must place integrity updates behind a trusted Modium adapter and operating-system/application policy, and should persist verifier evidence before enabling VM-facing mutation.

## Read-only discovery

Discovery and inspection are observational. They can create a volume-observation record with discovered/inspected status, but do not create or initialize a storage region. The separate Modium claim operation remains governed by lib/vm/MODIUM-STORAGE-REGION.md.

Registry registration accepts a stable caller-supplied record ID and inspected descriptor fields. It does not call a Modium claim operation. A duplicate ID fails rather than silently rebinding the record to a different device.

## Availability is not permanent

An available flag is only the last recorded state. Every actual region open/access must ask Modium to resolve the current region and verify the current descriptor/device seal. If the device is offline, foreign, unreadable, or altered, the runtime receives an explicit failure. It must not fall back to a different device with the same mount path or label.

## Required integration tests

- discovery and inspection perform no writes;
- a removable record without device seal is rejected;
- sizes below 256 MiB or above 64 GiB are rejected;
- a newly registered region begins unverified;
- a valid fresh verification is required before verified/available transitions;
- unavailable/foreign devices cannot be used as available storage;
- Modium resolution returns only bounded VM-local handles;
- the same behavior holds on Linux, macOS, and Windows.
