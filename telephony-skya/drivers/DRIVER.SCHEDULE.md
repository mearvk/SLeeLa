# Skya Telephony Driver Schedule

**Scope:** Linux, Windows 10+, and macOS  
**Location:** `telephony-skya/drivers/`

This schedule defines the implementation order for Skya's physical telephony and audio-device drivers. It is deliberately staged so that standard operating-system class support is used before vendor-specific code.

## 1. Driver architecture

The common C/C++ layer is authoritative:

```
Skya / SLeeLa / Guia™
        |
        v
Skya Driver Registry
        |
        +-- Device Discovery
        +-- Device Identity
        +-- Capability Probe
        +-- Audio Endpoint
        +-- HID Call Control
        +-- Vendor Adapter
        |
        +-- Linux
        +-- Windows 10+
        +-- macOS
```

Vendor adapters must not alter the Guia™ protocol or the SLeeLa application contract.

## 2. Platform strategy

### Linux

**Primary path**

1. USB/Bluetooth discovery through the Linux device stack.
2. Audio discovery through ALSA.
3. Prefer the installed PipeWire/PulseAudio environment for application-level routing when present.
4. HID discovery for headset buttons, hooks, mute, volume, and call-control functions.
5. SIP/network discovery for IP phones.
6. Vendor-specific interfaces only when standard class interfaces are insufficient.
7. Hot-plug and removal notifications.
8. Device recovery after disconnect/reconnect.

ALSA is the kernel's established sound subsystem, including USB sound support. The Skya driver should therefore integrate with the existing Linux audio stack rather than replacing it. citeturn0search23turn0search17

**Linux implementation phases**

| Phase | Work | Output |
|---|---|---|
| L1 | USB/audio discovery | Device inventory |
| L2 | ALSA input/output | Working microphone/speaker |
| L3 | PipeWire/PulseAudio routing | Application endpoint selection |
| L4 | HID call controls | Answer/end/mute/volume |
| L5 | SIP phone discovery | Network-phone inventory |
| L6 | Vendor adapters | Model-specific controls |
| L7 | Hot-plug/recovery | Stable runtime behavior |
| L8 | Packaging/tests | Linux driver test suite |

### Windows 10+

**Primary path**

1. Enumerate audio endpoints using Windows audio/device APIs.
2. Use WASAPI for audio streaming.
3. Use Windows' USB Audio class support for compliant USB audio devices.
4. Use Windows HID APIs for headset buttons and vendor HID interfaces.
5. Identify VID/PID, manufacturer, product and serial information where available.
6. Implement vendor-specific controls only when standard HID/class mechanisms are insufficient.
7. Handle device invalidation and re-enumeration.
8. Test Windows 10 and newer Windows releases separately.

Windows provides an in-box USB Audio class driver for compliant USB audio devices, while WASAPI provides application access to audio endpoint streams. citeturn0search8turn0search1

Windows HID APIs provide device discovery, attributes, strings and data movement; USB and Bluetooth HID transports are supported by the Windows HID stack. citeturn0search2turn0search3

**Windows implementation phases**

| Phase | Work | Output |
|---|---|---|
| W1 | PnP/audio enumeration | Device inventory |
| W2 | WASAPI render/capture | Working microphone/speaker |
| W3 | USB Audio validation | Class-compliant device support |
| W4 | HID discovery | Button/control inventory |
| W5 | HID call controls | Answer/end/mute/volume |
| W6 | Vendor adapters | Model-specific controls |
| W7 | Device invalidation/recovery | Stable reconnect behavior |
| W8 | Installer/signing/tests | Windows driver package/test suite |

The initial Skya implementation should remain user-mode wherever the Windows class drivers expose the required functionality. A custom kernel driver should be considered only when a device requires behavior that cannot be provided through the supported user-mode/class interfaces.

### macOS

**Primary path**

1. Discover standard audio devices through Core Audio.
2. Use Core Audio for application-level audio input/output.
3. Discover USB/HID devices through the supported macOS device interfaces.
4. Use vendor control interfaces only when standard mechanisms do not provide the required capability.
5. Use DriverKit/AudioDriverKit for a genuine custom system driver when required.
6. Handle device arrival/removal and endpoint changes.
7. Test Intel and Apple Silicon separately.

Core Audio is Apple's framework for interacting with audio hardware. Apple also provides AudioDriverKit for user-space audio drivers. citeturn0search6turn0search0

Apple's DriverKit supports many USB and HID devices, while audio-specific and wireless device cases have distinct framework requirements. The Skya implementation should therefore avoid assuming that a generic USB DriverKit extension is the correct solution for every headset or phone. citeturn0search10turn0search11

**macOS implementation phases**

| Phase | Work | Output |
|---|---|---|
| M1 | Core Audio enumeration | Device inventory |
| M2 | Core Audio input/output | Working microphone/speaker |
| M3 | USB/HID discovery | Device/control inventory |
| M4 | Call-control HID | Answer/end/mute/volume |
| M5 | Vendor adapters | Model-specific controls |
| M6 | DriverKit/AudioDriverKit where necessary | Custom-driver support |
| M7 | Hot-plug/recovery | Stable reconnect behavior |
| M8 | Signing/install/test | macOS driver package/test suite |

## 3. Cross-platform schedule

### Stage A — Common foundation

- [x] Driver directory
- [x] Common device structure
- [x] Capability structure
- [x] Driver registry
- [x] Built-in vendor probe layer
- [x] Brand/model catalog
- [ ] Common discovery result object
- [ ] Common driver diagnostics
- [ ] Common hot-plug event model
- [ ] Driver test harness

### Stage B — Standard audio devices

Implement class-compliant audio first.

- [ ] Linux USB Audio
- [ ] Windows USB Audio
- [ ] macOS Core Audio
- [ ] Input endpoint selection
- [ ] Output endpoint selection
- [ ] Sample-rate negotiation
- [ ] Channel negotiation
- [ ] Device mute
- [ ] Device volume
- [ ] Disconnect/reconnect

### Stage C — Call-control HID

- [ ] Answer
- [ ] End call
- [ ] Mute
- [ ] Hold
- [ ] Volume up/down
- [ ] Hook/off-hook
- [ ] Ring indication
- [ ] LED/status indication where supported

HID behavior must be discovered from the actual device rather than inferred from its brand or model name.

### Stage D — Vendor drivers

Implement in this order as hardware becomes available for testing:

1. Yealink
2. Poly
3. Jabra
4. Grandstream
5. EPOS / Sennheiser
6. Logitech
7. Fanvil
8. Snom
9. Cisco

The order is an implementation sequence, not a ranking of manufacturers.

### Stage E — VoIP desk phones

VoIP phones are different from USB audio peripherals.

The driver layer should separately support:

- SIP endpoint identification
- IP address
- MAC address
- model
- firmware
- SIP registration state
- call state
- handset/headset state
- network reachability
- supported codecs
- supported call-control features

The network/SIP implementation must remain separate from the USB/HID audio driver implementation.

## 4. Driver versioning

Each hardware record should ultimately contain:

```text
vendor
model
hardware revision
firmware family
transport
OS
driver version
capabilities
known limitations
test status
```

Firmware-specific behavior must never be silently generalized to every revision of a product family.

Recommended status values:

- `CATALOGED`
- `DISCOVERABLE`
- `AUDIO_WORKING`
- `HID_WORKING`
- `CALL_CONTROL_WORKING`
- `VENDOR_FEATURES_WORKING`
- `TESTED`
- `CERTIFIED`

`CERTIFIED` should only be assigned after an actual reproducible test procedure has been completed.

## 5. Test matrix

Every driver test should record:

| Field | Required |
|---|---|
| OS | Yes |
| OS version/build | Yes |
| CPU architecture | Yes |
| Manufacturer | Yes |
| Model | Yes |
| Hardware revision | If available |
| Firmware | If available |
| USB VID/PID | If applicable |
| Bluetooth identity | If applicable |
| Transport | Yes |
| Input device | Yes |
| Output device | Yes |
| Call controls | Yes |
| Hot-plug | Yes |
| Reconnect | Yes |
| Failure behavior | Yes |
| Driver version | Yes |

## 6. Safety and installation policy

Skya should prefer existing operating-system class drivers whenever they provide the required functionality.

The Skya driver program should **not**:

- replace a working system USB audio driver without reason;
- silently install kernel/system extensions;
- silently update device firmware;
- bypass operating-system security controls;
- claim support from catalog presence alone;
- enable unsupported vendor controls.

Custom drivers, DriverKit extensions, kernel components, or privileged services require explicit installation, appropriate signing, and a documented rollback/uninstall procedure.

## 7. Milestone definition

A platform driver milestone is complete only when:

1. The device can be discovered.
2. Identity is stable.
3. Input and output can be selected.
4. Supported call controls work.
5. Disconnect/reconnect is handled.
6. Unsupported features are reported.
7. Logs identify the device and driver version.
8. The test matrix has been recorded.
9. Installation and removal are documented.

This schedule intentionally separates **catalogued hardware**, **implemented drivers**, and **tested/certified hardware** so the Skya project does not confuse a listed model with verified functionality.
