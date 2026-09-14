# Phraign™ — Bash Pixel Terminal Interface

> Phraign™ is SLeeLa's frame-based, pixel-accurate terminal control system.
> See `PHRAIGN.md` for the system overview. This document is the Phraign™
> pixel-terminal interface specification.

## Purpose

SLeeLa may execute selected programs through Bash. This document defines the
Phraign™ pixel-granularity terminal interface available to those programs.

The terminal is treated as a **frame of pixels**, rather than only a character grid. A program may address a pixel by integer coordinates and construct a frame for presentation by the terminal.

## Coordinate model

The logical frame uses an origin at the upper-left corner:

```text
(0,0) --------------------> X
  |
  |
  |
  v
  Y
```

A frame has:

- `width` — number of addressable pixels horizontally.
- `height` — number of addressable pixels vertically.
- `(x,y)` — pixel coordinate, where `0 <= x < width` and `0 <= y < height`.
- `pixel(x,y)` — the pixel value at that coordinate.

Pixels outside the frame are not addressable.

## Terminal dimensions

The Bash layer now captures terminal-size information so the frame can follow the terminal window.

The preferred measurement is the terminal's **physical pixel size**. The Bash helper requests the common `CSI 14 t` window-size report and accepts the `CSI 4 ; height ; width t` response when the terminal supports it.

The interface emits:

```text
sleeLa-pixel-terminal PIXEL_SIZE WIDTH HEIGHT
sleeLa-pixel-terminal CENTER X Y
```

The center is derived from the current frame dimensions:

```text
center_x = width / 2
center_y = height / 2
```

Integer division is used, so the center identifies the upper-left pixel of the central pair when a dimension is even.

If physical pixel reporting is unavailable, the Bash layer reports character-cell dimensions instead:

```text
sleeLa-pixel-terminal CELL_SIZE COLUMNS ROWS
```

This fallback is deliberately labeled differently. The SLeeLa bridge can therefore distinguish physical pixel dimensions from terminal character dimensions.

## Size events

Bash receives `SIGWINCH` when its controlling terminal changes window size. `pixel_terminal_watch_resize` installs a `WINCH` trap that refreshes the current dimensions whenever the event occurs.

```bash
pixel_terminal_watch_resize
# ... Bash program continues running ...
pixel_terminal_unwatch_resize
```

A resize therefore follows this path:

```text
Terminal window changes
        |
        v
      SIGWINCH
        |
        v
   Bash WINCH trap
        |
        v
pixel_terminal_size
        |
   +----+----+
   |         |
   v         v
PIXEL_SIZE  CELL_SIZE
   |
   v
 CENTER
   |
   v
SLeeLa frame dimensions
```

The size event updates dimensions; it does not automatically discard or redraw the frame. A SLeeLa program can decide whether to resize, re-center, re-render, or preserve existing content.

## Bash interface

The Bash-side interface is exposed as shell functions. It is intentionally text-protocol based so a Bash program can generate a frame without requiring a special shell parser.

```bash
pixel_terminal_begin WIDTH HEIGHT
pixel_terminal_set X Y VALUE
pixel_terminal_fill VALUE
pixel_terminal_clear
pixel_terminal_show
pixel_terminal_end
pixel_terminal_cell_size
pixel_terminal_pixel_size
pixel_terminal_size
pixel_terminal_watch_resize
pixel_terminal_unwatch_resize
```

`VALUE` is an implementation-defined terminal pixel value. The initial protocol accepts a textual value, allowing the SLeeLa terminal bridge to define RGB, indexed-color, grayscale, or other pixel encodings without changing the Bash command interface.

Example:

```bash
pixel_terminal_begin 800 600
pixel_terminal_watch_resize
pixel_terminal_clear
pixel_terminal_set 10 10 '#ffffff'
pixel_terminal_set 11 10 '#ffffff'
pixel_terminal_set 12 10 '#ffffff'
pixel_terminal_show
pixel_terminal_end
pixel_terminal_unwatch_resize
```

## Frame semantics

`pixel_terminal_begin` creates a new frame.

`pixel_terminal_set` changes one addressable pixel in the current frame.

`pixel_terminal_fill` assigns one value to the complete frame.

`pixel_terminal_clear` resets the current frame to the terminal's default empty value.

`pixel_terminal_show` submits the current frame for presentation. It does not imply that the terminal has been resized or that the frame owns the physical display.

`pixel_terminal_end` releases the current frame.

A program may build a frame incrementally and submit it repeatedly for animation or interactive output.

## SLeeLa relationship

Bash is an execution target for selected SLeeLa programs. The intended Phraign™ architecture is:

```text
SLeeLa program
      |
      v
     Bash
      |
      v
pixel_terminal_* interface
      |
      v
SLeeLa terminal/frame bridge
      |
      v
pixel-addressable terminal frame
```

This keeps Bash responsible for command execution while SLeeLa owns the terminal/frame abstraction.

## Compatibility

The interface is additive. Ordinary Bash commands, standard input/output, POSIX shell behavior, and normal terminal operation remain unchanged.

A terminal that does not provide the SLeeLa pixel bridge may reject the pixel commands without altering ordinary Bash operation.

## Pixel-map relationship

The terminal frame is a raster presentation surface. It is not itself the canonical geometric Pixel Map object. A future bridge may render a Pixel Map into this frame while preserving the project's distinction between a canonical map, derived raster, and serialization/codec representation.
