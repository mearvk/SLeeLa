# Lesson 3 — Pixels and Bounds

Pixel-level operations are useful for small diagnostic marks, simple overlays, or controlled drawing tasks. They are also an easy place to introduce off-by-one errors. Always establish the target display's geometry and verify the relevant capability before making a write.

Ballum's documented local coordinate convention uses a top-left origin. For a display of width `w` and height `h`, valid coordinates satisfy:

- `0 <= x < w`
- `0 <= y < h`

The first pixel is `(0, 0)`. The last valid pixel is `(w - 1, h - 1)`. A coordinate of `(w, h)` is outside the display because valid indices stop one short of the dimensions.

## Recommended sequence

1. Enumerate displays and verify that the operation succeeded.
2. Choose a real display ID from the returned list.
3. Read that display's dimensions and capability mask.
4. Check the per-pixel capability bit (`1`).
5. Validate coordinates before calling the bridge.
6. Issue the write using the documented argument format.
7. Inspect the returned status and log failures accurately.

A conceptual guard looks like this:

```text
if width <= 0 or height <= 0:
    do not draw
if x < 0 or y < 0 or x >= width or y >= height:
    report out of bounds
if pixel capability is absent:
    report unsupported capability
otherwise:
    call pixel operation
    inspect returned status
```

This is pseudocode, not a claim about exact SLeeLa syntax. Use the language's supported operators and the actual Ballum method signature when translating the sequence into code.

## Validate the whole input

Bounds are only one part of validation. Check that the display ID is valid, the pixel operation is supported, and any color/channel values fit the API's documented representation. Avoid implicit narrowing conversions that could turn a negative value into a large unsigned value. Reject invalid inputs before they cross the VM/native boundary, while still preserving native status codes in case the backend rejects a request.

## Keep drawing predictable

- Start with a known coordinate such as `(0, 0)`, after confirming the dimensions are nonzero.
- Do not assume a write is atomic, visible immediately, or persistent unless the native API guarantees that behavior.
- Do not draw in a loop that has no upper bound or that can overflow coordinate arithmetic.
- For a rectangle, compute the end coordinates carefully and avoid adding width to an origin without checking for overflow.
- Keep pixel operations small during initial testing. A full-screen loop can be slow and can obscure whether an individual call is failing.
- If an operation fails, stop or report the failure; do not print “success” merely because the call was attempted.

## Example

Use [02-safe-pixel.sleela](../examples/02-safe-pixel.sleela). It is designed to inspect the primary display, check the pixel capability, attempt a boundary-safe write, and submit an invalid coordinate to demonstrate rejection. Review the source before running it, and verify its syntax against your installed compiler/runtime.

The deliberately invalid request is a negative test: it is useful only if the program recognizes and reports the rejection. Do not treat rejection of the invalid coordinate as a failure of the test itself.

## Troubleshooting

- **Out of bounds:** re-read the target display's dimensions and inspect the exact coordinate. Remember that the upper bound is exclusive.
- **Unsupported capability:** the selected backend or display does not advertise pixel operations; skip the write.
- **Backend unavailable:** investigate bridge linkage and native backend selection, not coordinate math.
- **Backend error or permission denial:** preserve the native status and investigate host-specific requirements.
- **No visible change but status is OK:** verify that the code is reaching the real bridge/backend and that the operation's documented semantics promise a visible effect.

**Checkpoint:** explain why `x == width` is invalid and why a valid coordinate alone does not guarantee a successful write.