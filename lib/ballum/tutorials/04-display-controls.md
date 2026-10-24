# Lesson 4 — Brightness, Warmth, and Gamma

Ballum documents three whole-display controls: brightness, color warmth, and gamma. These controls affect the viewing experience and can vary significantly by backend and hardware. Use conservative values, inspect the result, and be prepared to restore the previous settings.

## Documented input ranges

| Control | Input range | Meaning |
|---|---:|---|
| Brightness | 0–100 | Percentage-like level |
| Warmth | 1000–10000 | Color temperature in kelvin |
| Gamma | 10–1000 | Gamma encoded as gamma × 100; `220` means `2.20` |

The capability bits are `4` for brightness, `8` for warmth, and `16` for gamma. Confirm the current package README and source before relying on these values in a different release.

## Understand the controls

**Brightness** changes the display's brightness level where the backend exposes a supported mechanism. A value of 70 represents 70 on the documented scale; it does not guarantee the same perceived luminance across different monitors.

**Warmth** is expressed as color temperature in kelvin. Lower values generally look warmer/yellower and higher values cooler/bluer. The API's supported range is a parameter contract, not a promise that every display can reproduce every temperature.

**Gamma** changes the mapping between input signal levels and displayed intensity. Ballum's integer representation is scaled by 100: 220 represents 2.20. Do not pass 2.2 directly if the API expects the encoded integer.

These controls are not interchangeable. Changing warmth does not mean the same thing as changing gamma, and neither should be described as a calibrated color-management system unless the implementation explicitly provides that guarantee.

## Safe control workflow

1. Enumerate displays and select the intended display by ID.
2. Read its capability mask.
3. Check the relevant bit before calling each control.
4. Validate the value against the documented range.
5. Make one change at a time.
6. Inspect the returned status and the actual visible result.
7. Record the original setting first if the backend provides a reliable way to read it.
8. Restore the original setting when the experiment is complete, if the API supports doing so.

Do not assume the package can read back the current value unless the API documents a getter. If the previous setting cannot be queried, choose a conservative test value and make sure you know how to restore the setting through the operating system's normal display controls.

## Capability does not guarantee execution

A capability flag means the selected display/backend advertises that operation. The native call can still fail because the backend cannot apply the requested value, the operating system denies access, or the display configuration changed. Always inspect the native status. Do not silently substitute a different value after a failure.

## Example

[03-display-controls.sleela](../examples/03-display-controls.sleela) demonstrates capability-gated requests for brightness 70, warmth 3400 K, and gamma 2.20 (encoded as 220). Run it only on a test system where changing the visible display is acceptable. Review the source and verify that the running build is connected to the real native bridge before interpreting the result as a hardware test.

## Troubleshooting

- **Unsupported capability:** do not retry repeatedly; skip the control on that display.
- **Invalid argument:** check both the numeric range and the expected encoding.
- **Permission denied:** use the documented platform permission or system configuration path; do not attempt to bypass security controls.
- **Status says OK but appearance does not change:** verify the backend path and documented operation semantics.
- **Unexpected appearance:** stop further changes and restore the prior setting using the supported API or operating-system controls.

**Checkpoint:** explain why gamma `220` means 2.20, why capability checks are necessary, and why a safe experiment should have a restoration plan.