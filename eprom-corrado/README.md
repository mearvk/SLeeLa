# eprom-corrado — EPROM programmer driver family

A SLeeLa device-specific driver family for the **USB → EPROM** programmer path:
reading, writing, verifying, and checksumming the engine-ECU EPROM of a
Volkswagen Corrado (1990–1996) via a MiniPRO TL866-family USB chip programmer.

This directory is the SLeeLa-side **reference and architecture record** for that
backend. It conforms to the repository-level `/drivers` architecture and the
`DRIVER.COMPLETENESS.md` status model, exactly as `telephony-skya/` does for the
Skya driver family. It is the authoritative *SLeeLa-side* location for the EPROM
family's hardware mappings, supported/unsupported operations, and evidence.

## Relationship to the external backend

The working implementation of this backend lives **outside SLeeLa**, in the
`mearvk/Corrado` repository, which provides:

- an OS-independent C image/checksum library and the per-OS libusb drivers;
- a Java port of the OS-independent core (`java/com/corrado/eprom/`);
- a SLeeLa-style **Connector/Control series** for USB → EPROM
  (`sleela/java/com/mearvk/sleela/eprom/`) with Direct, Process (native CLI),
  and HTTP transports — all behind one `EpromConnector` contract.

That Connector/Control series was itself modelled on SLeeLa's `CONNECTOR.md` and
`DRIVERS.md`. This family record brings it back into SLeeLa by **reference and
congruence**: SLeeLa documents the device family and its contract conformance
here; Corrado remains the authoritative implementation. Code is not duplicated
into SLeeLa.

> Why reference, not copy: SLeeLa's own `drivers/README.md` states that
> repository-level `/drivers` is the shared architecture boundary and that
> device-specific families are their own authoritative location. Keeping the
> implementation in Corrado and the family record here honours that boundary
> and avoids two diverging copies of the same driver.

## Layout

```
eprom-corrado/
├── README.md                 # this file
├── drivers/
│   ├── include/
│   │   └── eprom_corrado_profile.h   # device/profile mappings (header only)
│   └── model-records/
│       └── 27C256.md                 # per-device record (the Corrado ECU chip)
└── docs/
    └── COMPLETENESS.md       # audit status against DRIVER.COMPLETENESS.md
```

## Contract conformance

The family exposes the SLeeLa driver responsibilities (`DRIVERS.md`) through the
Corrado `EpromControl` contract: `open`, `read`, `write`, `blankCheck`, `erase`,
with explicit failure reasons. The `EpromConnector` contract supplies the
Java-side integration surface (`invoke`, `health`, `isHealthy`, `close`), with
three interchangeable transports. See `docs/COMPLETENESS.md` for the audit
status and the Corrado repo for the running code.

## Safety

Reprogramming an engine ECU can make a vehicle unsafe or non-compliant. The
family's documented posture (from the backend): **back up the stock image first
and verify every write.** Destructive verbs (`write`, `erase`) must never be
exposed over an unauthenticated transport.
