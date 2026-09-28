# SleelaTerminal™ — Phraign™ Frame-Based Terminal Pixel Control System

> **SleelaTerminal™** is SLeeLa's terminal product. It is **built on GNU Bash**
> (GPL, © Free Software Foundation). SleelaTerminal™ branding applies only to
> SLeeLa's own additions — the Phraign™ pixel-terminal work and the `model-3D/`
> application — and never to the vendored GNU Bash sources, whose GPL headers,
> copyright, and third-party GNU project names are left intact.

**Phraign™** is the SleelaTerminal™ frame-based, pixel-accurate terminal control
system. It lets a program treat the terminal output surface as an addressable
**planar map of pixels** rather than only a character grid, and to specify
control of that surface at pixel granularity.

Phraign™ is the umbrella name for the SleelaTerminal™ pixel-terminal work living
in `bash/`:

| Component | File | Role |
|-----------|------|------|
| Overview | `PHRAIGN.md` | This document — the Phraign™ system summary. |
| Interface spec | `PIXEL_TERMINAL.md` | Coordinate model, frame semantics, sizing, resize events. |
| Handshake spec | `SLEELA_BASH_HANDSHAKE.md` | Versioned capability negotiation between SLeeLa and Bash. |
| Shell interface | `pixel-terminal.sh` | Bash text-protocol functions (`pixel_terminal_*`). |
| Native layer | `pixel_terminal.hpp` / `pixel_terminal.cpp` | C++ `sleela::terminal::PixelTerminal` owning frame + geometry. |
| Smoke test | `pixel_terminal_smoke.cpp` | Native layer assertions. |
| Application | `model-3D/` | Phraign™ City 3D — a ~4000-block city rendered onto the Phraign frame per pixel (see `model-3D/README.md`). |

## Why "SleelaTerminal™" and "Phraign™"

**SleelaTerminal™** is the SLeeLa-facing product/distribution name for the whole
terminal effort. **Phraign™** is the frame-based control system inside it, and
`pixel_terminal` remains the concrete module/API name. Naming the product and
the system separately lets SLeeLa refer to the whole frame-based terminal
capability — spec, handshake, shell interface, and native layer — as one thing,
while the code-level identifiers stay descriptive and the underlying execution
engine (GNU Bash) keeps its own name.

## What Phraign™ gives the user

- **Pixel accuracy.** The terminal surface is modeled as a frame of pixels with
  an origin at the upper-left corner. A program addresses any pixel by integer
  `(x, y)` coordinates, where `0 <= x < width` and `0 <= y < height`.
- **A pixel map / planar surface.** The frame is a raster plane the program
  fills, clears, or sets pixel-by-pixel, then submits for presentation.
- **Terminal-following geometry.** Phraign™ prefers the terminal's physical
  pixel size and falls back to character-cell size, and it derives the frame
  center from the current dimensions.
- **Resize awareness.** `SIGWINCH` and the native `pollResize()` path let a
  program observe terminal size changes without discarding frame content.
- **Capability negotiation.** A versioned handshake
  (`SLEELA-BASH/1 PHRAIGN/1 VARIANT=NATIVE`) lets SLeeLa confirm the Bash
  environment understands Phraign™ before sending frame commands.

## System shape

```text
SLeeLa program
      |
      v
     Bash  --- Phraign™ handshake (HELLO / READY / CAPS)
      |
      v
pixel_terminal_* shell interface   (pixel-terminal.sh)
      |
      v
Native Phraign™ layer              (sleela::terminal::PixelTerminal)
      |
   frame + geometry + resize
      |
      v
SLeeLa terminal/frame bridge
      |
      v
pixel-addressable terminal frame
```

Bash stays responsible for command execution; Phraign™ owns the frame and
geometry abstraction; the SLeeLa bridge owns actual raster presentation.

## Compatibility

SleelaTerminal™ (via Phraign™) is additive. Ordinary Bash commands, standard
I/O, POSIX shell behavior, and normal terminal operation are unchanged. A
terminal without the Phraign™ bridge may ignore the pixel commands without
affecting ordinary Bash operation. See `PIXEL_TERMINAL.md` and
`SLEELA_BASH_HANDSHAKE.md` for details.

## Attribution

SleelaTerminal™ is built on **GNU Bash**, which is free software licensed under
the **GNU General Public License** and copyright the **Free Software
Foundation**. The vendored Bash sources in `bash/` retain their original GPL
license text, copyright notices, and references to GNU projects unchanged;
SleelaTerminal™ / Phraign™ branding covers only SLeeLa's own additive files.
