# Native window minimum and maximum size

The SLeeLa native UI toolkit exposes pixel-based window size limits in the C ABI,
SLVM built-ins, and the `SLWindow` standard-library wrapper.

## Native C/C++ API

```c
slui_window_set_min_size(window, width, height);
slui_window_set_max_size(window, width, height);
slui_window_set_size_limits(window, min_width, min_height, max_width, max_height);
slui_window_get_min_size(window, &size);
slui_window_get_max_size(window, &size);
```

Setters return `SLUI_OK` (0) on success or a negative `SLUIStatus` on error.
Updates are validated atomically. Negative dimensions are rejected, and a
maximum below the effective minimum is rejected. The automatic minimum includes
the measured root content plus 10 px padding on every edge. A maximum width or
height of `0` means that dimension is unbounded; setting all four values to
zero restores automatic minimum sizing and an unbounded maximum.

## SLeeLa built-ins

| Built-in | Arguments | Result |
| --- | --- | --- |
| `uiWindowSetMinSize(window, width, height)` | 3 integers | status |
| `uiWindowSetMaxSize(window, width, height)` | 3 integers | status |
| `uiWindowSetSizeLimits(window, minWidth, minHeight, maxWidth, maxHeight)` | 5 integers | status |
| `uiWindowMinWidth(window)` / `uiWindowMinHeight(window)` | window handle | pixels, or -1 on error |
| `uiWindowMaxWidth(window)` / `uiWindowMaxHeight(window)` | window handle | pixels; 0 means unbounded, -1 on error |

## SLWindow

`SLWindow` provides matching methods: `setMinSize`, `setMaxSize`,
`setSizeLimits`, `minWidth`, `minHeight`, `maxWidth`, and `maxHeight`.
The setters return the native status code; getter errors return `-1`.

## Example and verification

Run `demo/gui-window-size-limits.sleela` on a desktop session to see the
actual minimum and maximum read back and enforced during interactive resizing.
In a headless environment it exits cleanly after confirming the built-ins are
available at compile time. The native C API regression cases live in
`user-interface/tests/smoke.c`.
