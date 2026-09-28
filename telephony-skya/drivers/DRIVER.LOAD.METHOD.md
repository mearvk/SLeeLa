# Skya™ Driver Load Method

## Purpose

This document defines how Skya™ Telephony Drivers are loaded on Linux, Windows 10+, and macOS.

The load method is distinct from `DRIVER.SCHEDULE.md`: the schedule defines implementation/testing stages; this document defines how a completed driver becomes available to the Skya runtime.

Architecture:

```
Skya GUI / SLeeLa / Guia™
        -> Driver Registry
        -> Discovery / Identity / Capability Check
        -> Platform Driver Layer
        -> Runtime Device
```

A catalog entry alone MUST NOT cause a driver to be considered loaded. Loading requires a compatible implementation, successful registration, and positive device/endpoint matching.

## 1. Common Load Contract

Every platform follows:

1. **Locate** the built-in or explicitly installed driver.
2. **Validate** platform, architecture, ABI/version, and dependencies.
3. **Register** the driver with the Skya registry.
4. **Probe** connected hardware or network endpoints.
5. **Match** vendor, model, transport, firmware family, and capabilities.
6. **Activate** the runtime device instance.
7. **Verify** audio, HID/call-control, or SIP/network capability.
8. **Publish** the device to Skya/SLeeLa.
9. **Monitor** removal, reconnect, and failure.

Recommended states:

`DISCOVERED` -> `VALIDATING` -> `REGISTERING` -> `PROBING` -> `MATCHED` -> `ACTIVATING` -> `READY`

Failure/removal states: `FAILED`, `REMOVED`, `RECONNECTING`.

## 2. Linux Load Method

### Preferred platform path

Use existing Linux interfaces before custom kernel code:

1. USB/Bluetooth discovery.
2. ALSA device discovery/capability inspection.
3. PipeWire/PulseAudio application routing where present.
4. HID discovery for call-control peripherals.
5. SIP/network discovery for VoIP desk phones.
6. Vendor-specific controls only where standard interfaces are insufficient.
7. Hotplug and reconnect monitoring.

Skya should normally remain a user-space application. A custom kernel module is not required merely because a device appears in the catalog.

### Built-in load

```
libskya-drivers.a
  -> skya_driver_registry
  -> vendor probe
  -> ALSA / HID / network discovery
  -> skya_driver_device
```

Startup sequence:

1. Initialize registry.
2. Register built-in vendor adapters.
3. Enumerate devices/endpoints.
4. Probe candidates.
5. Select only a matching driver.
6. Create runtime device.
7. Publish capabilities.

### External modules

Future separately installed Linux modules MUST validate architecture, ABI, dependencies, permissions, package/integrity policy, and controlled search path. A filename or catalog name is never sufficient for loading.

### Removal/reload

On removal, stop I/O, release ALSA/HID/network handles, mark the device `REMOVED`, retain the logical driver registration, wait for re-enumeration, and probe again.

## 3. Windows 10+ Load Method

### Preferred platform path

Use Windows user-mode APIs and Microsoft class drivers whenever they provide the required capability:

1. Plug-and-Play enumeration.
2. Audio endpoint discovery.
3. WASAPI endpoint activation.
4. USB Audio class support.
5. HID discovery/control.
6. Vendor SDK/control interface only when necessary.
7. Device notification and re-enumeration.

Custom kernel-mode drivers should be reserved for capabilities that cannot be provided through supported user-mode/class-driver interfaces.

### Load sequence

```
PnP enumeration
  -> USB / Audio / HID identity
  -> Skya vendor/model probe
  -> WASAPI + HID capability check
  -> registry activation
  -> Skya runtime device
```

Validate Windows version, architecture, device instance identity, USB VID/PID where applicable, audio endpoint identity, HID collections, driver ABI/version, and dependencies.

A vendor's Windows driver package is not itself the Skya logical driver. The boundary remains:

```
Windows class/vendor driver
  -> Skya vendor adapter
  -> Skya Driver API
  -> SLeeLa / Guia™
```

### Removal/reload

On device invalidation, stop I/O, release WASAPI/HID resources, mark `REMOVED`, wait for re-enumeration, re-probe, and recreate the runtime device.

## 4. macOS Load Method

### Preferred platform path

Use Apple-supported user-space facilities first:

1. Core Audio discovery.
2. Audio endpoint/stream inspection.
3. USB/HID discovery where required.
4. Vendor control interfaces where required.
5. DriverKit/AudioDriverKit only where genuinely necessary.
6. Device arrival/removal monitoring.

### Load sequence

```
Core Audio / device discovery
  -> USB / HID identity
  -> Skya vendor/model probe
  -> capability validation
  -> registry activation
  -> Skya runtime device
```

Validate macOS version, Intel/Apple Silicon architecture, device identity, Core Audio endpoint identity, USB/HID identity where applicable, ABI/version, permissions, and any required DriverKit component.

DriverKit/AudioDriverKit is a separate system-driver boundary. It must not be introduced simply because a device has a vendor-specific feature. Where required, the component must be separately packaged, signed, installed, approved, and removable under macOS security rules.

### Removal/reload

Stop I/O, release Core Audio/HID resources, mark `REMOVED`, retain logical registration, wait for arrival, re-probe, recreate the runtime device, and report restored/unavailable capabilities.

## 5. Vendor Loading Rule

The brand/version catalog is a discovery target, not blanket authorization to load code.

```
Catalog Entry
  -> Physical / Network Identity
  -> Transport Match
  -> Capability Match
  -> OS Compatibility
  -> Driver Load
```

Record vendor, model, family, hardware revision, firmware, transport, OS, architecture, driver version, capabilities, limitations, load result, and failure diagnostics.

## 6. Built-in vs External Drivers

### Built-in

Built-in drivers are compiled into the Skya driver library and registered at application startup. This provides deterministic ABI/versioning and reproducible deployment.

Current foundation:

- `telephony-skya/drivers/src/skya_driver_registry.cpp`
- `telephony-skya/drivers/src/skya_builtin_drivers.cpp`
- `telephony-skya/drivers/include/skya_phone_driver.h`

### External

External drivers may be added for hardware requiring separate packaging. They MUST use explicit installation and controlled loading with ABI, platform, architecture, dependency, and integrity/signature validation as applicable.

Never load an arbitrary driver from the current working directory by default.

## 7. Security

Skya MUST:

- prefer OS class drivers;
- avoid unnecessary kernel/system extensions;
- never silently replace an OS driver;
- never silently update firmware;
- never bypass OS security controls;
- report permission failures;
- support rollback/removal for separately installed components.

Firmware updates are separate operations requiring explicit authorization.

## 8. Diagnostics

Each load attempt should record:

```
timestamp
os
architecture
vendor
model
hardware_revision
firmware
transport
device_id
driver_name
driver_version
registry_state
probe_result
capabilities
error_code
error_message
```

Example:

```
SKYA DRIVER LOAD
vendor=Yealink
model=MP45
transport=USB_AUDIO
os=linux
state=PROBING
audio_input=yes
audio_output=yes
hid=no
result=READY
```

## 9. Relationship to DRIVER.SCHEDULE.md

```
DRIVER.SCHEDULE.md
  -> implementation + test
  -> build / install / register
  -> DRIVER.LOAD.METHOD.md
  -> probe / activate / monitor
```

A scheduled driver is not automatically loaded. A loaded driver is not automatically certified. Certification follows the applicable test matrix and platform requirements.

## 10. Platform Completion Criteria

### Linux
- [ ] Registry initialization
- [ ] USB/Bluetooth discovery
- [ ] ALSA endpoint discovery
- [ ] PipeWire/PulseAudio routing where applicable
- [ ] HID discovery
- [ ] Vendor probe
- [ ] Device activation
- [ ] Hotplug/reconnect
- [ ] Diagnostics

### Windows 10+
- [ ] Registry initialization
- [ ] PnP enumeration
- [ ] WASAPI endpoint discovery
- [ ] USB Audio class integration
- [ ] HID discovery
- [ ] Vendor probe
- [ ] Device activation
- [ ] Device notification/reconnect
- [ ] Diagnostics

### macOS
- [ ] Registry initialization
- [ ] Core Audio discovery
- [ ] USB/HID discovery
- [ ] Vendor probe
- [ ] Device activation
- [ ] DriverKit boundary documented where required
- [ ] Arrival/removal handling
- [ ] Intel + Apple Silicon validation
- [ ] Diagnostics

## 11. Normative Principle

**Skya loads a logical telephony driver only after the operating system exposes a compatible device or endpoint and the driver registry positively matches its identity and capabilities.**

The catalog identifies intended support. The operating system supplies the device interface. The adapter connects those layers. SLeeLa and Guia™ remain authoritative for application behavior, commands, events, sessions, and GUI semantics.
