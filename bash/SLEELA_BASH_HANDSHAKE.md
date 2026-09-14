# SLeeLa ↔ Bash Native Pixel Handshake

## Purpose

SLeeLa can execute selected programs through Bash. The native pixel-terminal addition now has an explicit capability handshake so SLeeLa can determine whether the Bash environment understands the pixel-granularity terminal interface before sending pixel commands.

The handshake is deliberately small, versioned, and additive. Ordinary Bash execution does not depend on it.

## Protocol identity

The current protocol identity is:

```text
SLEELA-BASH/1 PIXEL_TERMINAL/1 VARIANT=NATIVE
```

The fields mean:

- `SLEELA-BASH/1` — Bash integration protocol major version.
- `PIXEL_TERMINAL/1` — pixel-terminal interface version.
- `VARIANT=NATIVE` — the Bash endpoint is paired with the native SLeeLa pixel-terminal implementation.

A future incompatible protocol increments the `SLEELA-BASH` major version. Additive pixel-terminal features may increment the pixel-terminal version while retaining compatibility rules.

## Handshake exchange

SLeeLa should begin a Bash session by requesting the capability handshake:

```text
sleeLa-pixel-terminal HELLO SLEELA-BASH/1 PIXEL_TERMINAL/1 VARIANT=NATIVE
```

The Bash-side helper answers with:

```text
sleeLa-pixel-terminal READY SLEELA-BASH/1 PIXEL_TERMINAL/1 VARIANT=NATIVE
```

It then advertises capabilities:

```text
sleeLa-pixel-terminal CAPS PIXEL_GRANULARITY NATIVE_FRAME RESIZE_EVENTS
```

A bridge may treat `READY` as the compatibility acknowledgement and `CAPS` as the authoritative feature list.

## Bash helper

The shell interface exposes:

```bash
pixel_terminal_handshake
```

It emits the versioned `HELLO`, `READY`, and `CAPS` records for a SLeeLa-aware Bash environment.

The implementation also exports these environment variables for child programs:

```text
SLEELA_BASH_PROTOCOL=1
SLEELA_PIXEL_TERMINAL_VERSION=1
SLEELA_PIXEL_TERMINAL_VARIANT=NATIVE
```

A child program may therefore inspect the environment without parsing terminal output.

## Capability meaning

`PIXEL_GRANULARITY` means a program may address the terminal frame using `(x,y)` coordinates.

`NATIVE_FRAME` means the SLeeLa native C++ layer owns the frame geometry and pixel buffer rather than treating the pixel commands as ordinary character output.

`RESIZE_EVENTS` means terminal resize changes can be observed through the Bash `SIGWINCH` path and the native `pollResize()` path.

Physical pixel dimensions remain optional. Character-cell dimensions are the portability fallback, while a pixel-capable bridge may supply an actual raster size.

## Compatibility rule

SLeeLa should not send pixel-frame commands until the handshake has returned `READY` with a compatible major version and the `PIXEL_GRANULARITY` capability.

If the handshake is unavailable, an ordinary Bash program remains valid. SLeeLa should simply use its normal non-pixel execution path.

## Architecture

```text
             SLeeLa
                |
                | HELLO
                v
             Bash
                |
       pixel_terminal_handshake
                |
        +-------+-------+
        |               |
      READY            CAPS
        |               |
        +-------+-------+
                |
                v
     Native pixel-terminal layer
                |
       frame + geometry + resize
                |
                v
       SLeeLa terminal bridge
```

The handshake establishes capability awareness; it does not itself render a frame. Rendering remains the responsibility of the terminal/frame bridge.
