# VM Version 008 — Portability / platform-parity readout

*Idea System(TM) / Idea(TM). From Author; distributive.*

## Change (built on 007)
Added `vm_platform.{h,c}` + test over the core's `sleela_platform` API: detects
the host (Win32/POSIX/Apple) + version/arch, and reports **parity** per
capability — is it supported by the OS AND permitted by config? A capability a
user permits but the OS doesn't support emits one typed UNAVAILABLE warning
(via v003), and `vm_platform_report` prints the full parity table.

## Rationale
The VM is cross-platform (Windows 10+, macOS, Linux). 008 makes the gap between
"what I permitted" and "what this OS actually provides" visible, so cross-OS
surprises surface early instead of as a runtime failure.

## Carried forward
v001–v007.

## Verification
Builds warning-free standalone and platform-wired (`-DVM_HAVE_PLATFORM` +
`sleela_platform.c`). On this host it detected `linux`, reported the real kernel
version/arch via `slplatform_detect` (Linux 6.1.x x86_64), and showed all
capabilities at parity=ok with 0 mismatches.

## Next (009)
Audit trail: a structured, bounded event log (`vm_audit`) that records config
changes, denials, pressure/thrash events, and saturation — one tamper-evident
record tying the prior layers together.
