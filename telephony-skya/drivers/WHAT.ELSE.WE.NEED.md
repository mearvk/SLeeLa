# WHAT ELSE WE NEED

## Purpose

This document is the completion roadmap for making the SLeeLa / Skya driver system **world-class, production-grade, hardware-working drivers**.

The codebase now has substantial shared infrastructure, product bindings, control/state paths, data-plane support, and Linux/Windows/macOS platform boundaries. The remaining work is primarily **hardware verification, transport completeness, reliability engineering, security, testing, packaging, and long-term device maintenance**.

A driver is not considered hardware-complete merely because its source compiles or its model is recognized.

---

## 1. Exact Hardware Identification

For every supported product and hardware revision we still need:

- Exact USB VID/PID where applicable.
- USB interface numbers and endpoint descriptors.
- HID interface and report descriptors.
- Bluetooth identifiers and service/characteristic information where applicable.
- Network/SIP capability identifiers for network telephones.
- Hardware revision identifiers.
- Bootloader identifiers.
- Firmware version ranges.
- Regional/product variants.
- Dock/base/headset relationships.
- Device serial-number handling where appropriate.

### Completion requirement

Every model record should identify the exact hardware evidence used to bind the driver.

---

## 2. Real Transport Implementations

The callback boundaries must be backed by real platform transports.

### Linux

- USB/HID transport.
- ALSA/PipeWire/PulseAudio integration where applicable.
- evdev/input integration where appropriate.
- Bluetooth transport.
- Network/SIP transport.
- udev discovery and hotplug.
- Permission/device-node handling.
- Kernel-driver interaction where required.

### Windows 10+

- WinUSB/HID transport where applicable.
- Windows HID APIs.
- WASAPI capture/render.
- MMDevice enumeration.
- Bluetooth APIs.
- Device arrival/removal notifications.
- SetupAPI/device-property discovery.
- Correct service/driver installation model.
- Code-signing and package installation.

### macOS

- IOKit/IOHIDManager where applicable.
- Core Audio.
- USB device discovery.
- Bluetooth integration where applicable.
- Device arrival/removal notifications.
- macOS permission requirements.
- Universal binary support where applicable.
- DriverKit/System Extension analysis where a kernel-level component is actually required.

---

## 3. Verified HID and Vendor Protocols

For each product that exposes hardware controls we need verified implementations for:

- Report descriptors.
- Input reports.
- Output reports.
- Feature reports.
- Report IDs.
- Report lengths.
- Bit fields.
- Byte ordering.
- Scaling and ranges.
- Vendor-specific commands.
- Response/error codes.
- Firmware query commands.
- LED/busy-light commands.
- Mute state.
- Volume state.
- Answer/end/hold controls.
- Hook/off-hook state.
- Headset/base state.
- Button events.
- Battery state where applicable.
- Dock state where applicable.

**Do not invent undocumented packet formats.**

Every hardware packet implementation should be tied to manufacturer documentation, captured descriptors, controlled device testing, or another reproducible evidence source.

---

## 4. Complete State Machines

Each device needs a deterministic state machine covering:

- disconnected
- discovered
- opening
- initializing
- ready
- active
- calling
- muted
- held
- suspended
- reconnecting
- firmware-querying
- error
- shutting-down

State transitions must be synchronized and observable.

Invalid transitions must return explicit errors rather than silently changing state.

---

## 5. Hotplug and Recovery

World-class drivers must survive real-world device changes.

Implement and test:

- USB unplug.
- USB replug.
- Bluetooth disconnect/reconnect.
- Dock disconnect/reconnect.
- Network interruption.
- SIP registration loss.
- Audio-device disappearance.
- Device reset.
- Firmware restart.
- Suspend/resume.
- Windows sleep/wake.
- macOS sleep/wake.
- Linux suspend/resume.

The driver must release stale handles and recreate transport state safely.

---

## 6. Concurrency and Memory Safety

Audit every driver for:

- Data races.
- Deadlocks.
- Lock ordering.
- Condition-variable correctness.
- Queue ownership.
- Reference lifetime.
- Use-after-free.
- Double-free.
- Buffer overrun.
- Integer overflow.
- Timeout correctness.
- Cancellation behavior.
- Shutdown while I/O is blocked.

Required tooling should include, where supported:

- AddressSanitizer.
- UndefinedBehaviorSanitizer.
- ThreadSanitizer.
- Static analysis.
- Compiler warnings at high levels.
- Fuzzing.

---

## 7. Audio Engineering

For audio products, implement and test:

- Capture.
- Playback.
- Full duplex.
- Sample-rate negotiation.
- Channel negotiation.
- Format negotiation.
- Buffer sizing.
- Clock drift handling.
- Underrun handling.
- Overrun handling.
- Device latency measurement.
- Mute synchronization.
- Volume synchronization.
- Device-default changes.
- Multiple simultaneous audio devices.

Audio controls must not merely report capability; they must be connected to the actual audio/control path.

---

## 8. Telephony Semantics

For telephone products:

- Call lifecycle.
- Answer.
- Reject/end.
- Hold.
- Resume.
- Transfer where supported.
- DTMF.
- Dialpad.
- Hook state.
- Line state.
- Call-waiting state.
- Multiple-call state.
- Mute.
- Volume.
- Busy light.
- Presence.
- SIP/network status.

Unsupported functions must be reported explicitly.

---

## 9. Firmware

Implement a disciplined firmware layer:

- Firmware version discovery.
- Hardware/firmware compatibility matrix.
- Minimum supported firmware.
- Maximum tested firmware.
- Firmware capability discovery.
- Safe firmware-update detection.
- Update transport where legitimately supported.
- Interrupted-update recovery.
- Firmware integrity verification.
- Rollback/recovery behavior.

Never silently assume a firmware version is compatible.

---

## 10. Security

Every driver boundary needs:

- Input validation.
- Bounds checking.
- Integer-overflow protection.
- Privilege separation.
- Least-privilege device access.
- Signed release artifacts.
- SHA-256/SHA-512 release hashes.
- Reproducible build information.
- Secure firmware verification.
- Secure update policy.
- Audit logging.
- No embedded credentials.
- No uncontrolled shell execution.
- No arbitrary device-command injection.

---

## 11. Platform Packaging

Produce proper packages/installers rather than requiring source-tree execution.

### Linux

- Debian/Ubuntu packages.
- RPM-family package support where useful.
- udev rules.
- system integration.
- package signing.
- uninstall/upgrade paths.

### Windows

- Signed installer.
- Driver/package separation where possible.
- Device installation metadata.
- Upgrade and rollback.
- Uninstall cleanup.
- Windows 10 and newer compatibility matrix.

### macOS

- Signed application/library package.
- Notarization.
- Universal builds where supported.
- Proper system-extension/DriverKit packaging where necessary.
- Upgrade/uninstall cleanup.

---

## 12. Automated Test Lab

Create a hardware test matrix for every supported product.

Each model should be tested against:

- Linux versions.
- Windows 10+ versions.
- Current supported Windows releases.
- Supported macOS releases.
- USB versions.
- Bluetooth variants.
- Firmware versions.
- Hardware revisions.
- Sleep/wake.
- Hotplug.
- Long-duration operation.

Record:

- device identification
- firmware
- operating system
- driver version
- transport
- test date
- test result
- logs
- packet/descriptor evidence where permitted
- known limitations

---

## 13. Protocol and Hardware Fixtures

Build reusable test fixtures for:

- HID report replay.
- Descriptor replay.
- USB transport simulation.
- Bluetooth event simulation.
- Audio stream simulation.
- SIP event simulation.
- Hotplug simulation.
- Fault injection.
- Timeout injection.
- Corrupt packet injection.
- Device reset simulation.

This allows most regression tests to run without physical hardware.

---

## 14. Fuzzing

Fuzz:

- HID reports.
- Vendor packets.
- Descriptor parsers.
- Firmware responses.
- Network messages.
- SIP messages.
- Device metadata.
- Model identifiers.
- Configuration files.
- Driver-runtime messages.

The fuzz harness must verify that malformed input cannot crash the driver or corrupt driver state.

---

## 15. Observability

Every production driver should provide structured diagnostics:

- device identity
- hardware revision
- firmware
- transport
- connection state
- audio state
- call state
- queue depth
- dropped messages
- transport errors
- retry counts
- reconnect counts
- last successful operation
- last error
- timestamps

Sensitive information must not be logged unnecessarily.

---

## 16. Performance

Measure:

- command latency.
- HID latency.
- audio latency.
- callback latency.
- queue latency.
- reconnect time.
- CPU consumption.
- memory consumption.
- long-duration memory stability.
- high-event-rate behavior.

Define measurable budgets rather than relying on subjective responsiveness.

---

## 17. Cross-Platform API Contract

The public Skya API should expose the same conceptual operations on Linux, Windows, and macOS while allowing platform-specific capabilities.

The contract should clearly distinguish:

- supported
- unsupported
- unavailable
- not connected
- not implemented
- permission denied
- hardware error
- transport error
- timeout
- invalid state

Do not collapse these into a generic success/failure result.

---

## 18. Model Database

Create a machine-readable model registry containing:

- manufacturer
- model
- family
- hardware revision
- firmware range
- VID/PID
- transport
- capabilities
- descriptors
- protocol version
- platform support
- evidence level
- test status
- known limitations

This registry should drive discovery and driver selection.

---

## 19. Certification Levels

Use explicit completion levels:

### Level 0 — Metadata

Model is identified.

### Level 1 — Generic Runtime

Shared driver infrastructure operates.

### Level 2 — Transport

Actual platform transport operates.

### Level 3 — Device Control

Verified device controls operate.

### Level 4 — Integrated

Audio, controls, events, recovery, and platform integration operate together.

### Level 5 — Hardware Verified

The exact hardware/firmware/platform combination has been exercised and documented.

### Level 6 — Release Qualified

Automated regression, security, packaging, upgrade, recovery, and long-duration testing have passed for the declared support matrix.

A model must never be represented as Level 5 or Level 6 without the corresponding evidence.

---

## 20. Documentation

For every product provide:

- README.
- Hardware profile.
- Supported firmware matrix.
- Supported operating systems.
- Transport documentation.
- Control documentation.
- HID/protocol documentation where legally and technically appropriate.
- Installation instructions.
- Troubleshooting guide.
- Diagnostics guide.
- Test record.
- Known limitations.
- Security notes.
- Release history.

---

## 21. Legal and Evidence Boundary

Manufacturer specifications, public documentation, USB descriptors, and controlled measurements must be distinguished.

Record:

- source.
- date.
- hardware revision.
- firmware.
- measurement method.
- evidence level.
- reproduction procedure.

Do not turn an assumption into a hardware fact merely because it is plausible.

---

## 22. World-Class Definition

For this project, **World Class Working Driver** means:

> A driver that can reliably identify its declared hardware, establish the correct transport, perform its declared operations against real hardware, maintain correct state under concurrency and failure, recover from normal device interruptions, integrate with its operating system's native device/audio facilities, protect against malformed input, provide useful diagnostics, and pass a documented hardware/software test matrix.

Source-code presence alone is not sufficient.

Compilation alone is not sufficient.

Model recognition alone is not sufficient.

A capability flag alone is not sufficient.

A successful simulated test alone is not sufficient.

The final standard is **repeatable operation on the declared real hardware and platform matrix with evidence and regression coverage**.

---

## 23. Immediate Engineering Priorities

The remaining work should proceed in this order:

1. Complete the shared Linux/Windows/macOS transport implementations.
2. Build the machine-readable product/hardware registry.
3. Obtain and record exact device identifiers.
4. Implement verified HID/vendor protocols model by model.
5. Complete state machines and hotplug/recovery.
6. Build simulated protocol fixtures.
7. Build physical hardware test fixtures.
8. Add sanitizers, static analysis, and fuzzing.
9. Add cross-platform CI.
10. Build signed packages/installers.
11. Establish hardware/firmware compatibility matrices.
12. Run long-duration and failure-injection testing.
13. Assign evidence-based certification levels.
14. Publish per-model release and limitation records.

---

## Final Completion Rule

The goal is not simply to have a large collection of driver source files.

The goal is a **maintainable, testable, secure, cross-platform driver system whose declared hardware support can be demonstrated and reproduced**.

Until that evidence exists, the driver should retain its documented provisional status rather than claiming hardware completeness.
