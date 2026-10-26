# Lesson 2 — Discover Displays

Display discovery should come before drawing or changing settings. A program must know which displays exist, which one it intends to address, the dimensions it can safely use, and which operations the host says are supported.

The [screen demo](../screen-demo.sleela) shows the package's primary-display selection pattern. The [inspection example](../examples/01-screen-inspection.sleela) is the focused starting point for collecting display IDs, dimensions, primary flags, and capability masks.

## A disciplined discovery sequence

1. Call `uiBallumEnumerate()`.
2. Check the returned status before trusting any count or output.
3. Read the reported number of displays and handle zero as a normal result.
4. Iterate over valid enumeration positions only.
5. For each entry, obtain its display ID, width, height, primary indicator, and capability mask.
6. Select the target only after confirming the entry is present and usable.
7. Recheck the relevant capability immediately before an operation that depends on it.

Use the exact method signatures and output conventions in the package source; do not assume a return value and output parameter work the same way across every wrapper.

## Index is not identity

An **enumeration index** is the position of an entry in the current list. A **display ID** identifies the display in the native interface. They serve different purposes. Do not store index 0 and later assume it still refers to the same physical display. The order can vary with backend, system configuration, docking, or display changes.

The primary display is also not guaranteed to be index 0. Read the primary flag instead of relying on ordering. If the user chooses a display, preserve or validate the actual ID according to the API's contract rather than silently substituting another display.

## Geometry and coordinate space

A display's reported width and height define the dimensions that later coordinate checks should use. If width is `w` and height is `h`, the valid local pixel coordinate ranges are `0` through `w - 1` and `0` through `h - 1`, respectively. Do not assume all monitors have the same dimensions.

A multi-monitor desktop can have a global desktop coordinate space that differs from the local coordinate space of a single display. Unless the Ballum API explicitly documents global coordinates for a particular call, treat coordinates as belonging to the selected display and use its reported geometry. Do not infer scaling, rotation, or physical size from width and height alone.

## Capability masks

A capability mask is a set of flags, not a general guarantee that every operation will succeed. The package documents these bits:

| Bit value | Capability |
|---:|---|
| 1 | Per-pixel operation |
| 2 | Input |
| 4 | Brightness |
| 8 | Color warmth |
| 16 | Gamma |

To test a bit, use the bitwise operation supported by the SLeeLa version and source in this repository. Conceptually, a capability is present when `(mask & bit) != 0`. If a capability is absent, skip that operation and report why. If it is present, still check the result of the actual native call: permissions, driver limitations, or a changing host state may cause failure.

## Common mistakes

- Treating no displays as a fatal program error in a headless test.
- Selecting the first display without checking the primary flag or user choice.
- Confusing an enumeration index with a persistent display ID.
- Assuming capability bits are identical to runtime success.
- Using dimensions from one display to validate coordinates for another.
- Continuing to read output fields after enumeration has failed.

## Practice

Run or review [01-screen-inspection.sleela](../examples/01-screen-inspection.sleela). Extend it, if needed, to print a clear message for an empty list and to mark unsupported capabilities explicitly. Do not make display changes in this lesson.

**Checkpoint:** you should be able to name the selected display ID, explain its dimensions, state whether it is primary, and identify which capabilities are advertised before attempting an operation.